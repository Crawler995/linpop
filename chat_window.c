#include<stdio.h>
#include<stdlib.h>
#include<gtk/gtk.h>
#include<string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#include "chat_window.h"

const int MAX_LINE = 2048;
const int PORT = 6001;
const int BACKLOG = 10;
const int LISTENQ = 6666;
const int MAX_CONNECT = 20;

static void stop_connection() {
    if(!is_connected) {
        printf("not con\n");
        gtk_widget_destroy(window);
        return;
    }
    if(!continue_recv_message) return;

    printf("try stop\n");
    if(_is_server) {
        send(connfd, exit_message, strlen(exit_message), 0);
    }
    else {
        send(sockfd, exit_message, strlen(exit_message), 0);
    }

    
}

static void *recv_message_server(void *fd)
{
	sockfd = *(int *)fd;
	while(continue_recv_message)
	{
		char buf[MAX_LINE];
		memset(buf , 0 , MAX_LINE);
		int n;
		if((n = recv(sockfd , buf , MAX_LINE , 0)) == -1)
		{
			perror("recv error.\n");
			exit(1);
		}//if
		buf[n] = '\0';		
		//若收到的是exit字符，则代表退出通信
		if(strcmp(buf , exit_message) == 0)
		{
			printf("Client closed.\n");
            send(connfd, exit_message, strlen(exit_message), 0);
			close(connfd);
            continue_recv_message = false;
            gdk_threads_enter();
            gtk_widget_destroy(window);
            gdk_threads_leave();
            pthread_exit(NULL);
            return;
			//exit(1);
		}//if
        else if(strcmp(buf, recv_file_message) == 0) {
            const char *save_file_url = "./ccc.docx";
            FILE *file = fopen(save_file_url, "wb");
            char buf[MAX_LINE];
            int count;

            while((count = recv(sockfd, buf, MAX_LINE, 0)) > 0) {
                if(strcmp(buf, finish_recv_file_message) == 0) {
                    break;
                }
                fwrite(buf, count, 1, file);
            }

            fclose(file);
            printf("finish file\n");
        }
		printf("\nClient: %s\n", buf);
        gdk_threads_enter();
        append_friend_message_record_frame(_friend_name, buf);
        gdk_threads_leave();
	}//while
    close(sockfd);
}

static void *recv_message_client(void *fd)
{
	sockfd = *(int *)fd;
	while(continue_recv_message)
	{
		char buf[MAX_LINE];
		memset(buf , 0 , MAX_LINE);
		int n;
		if((n = recv(sockfd , buf , MAX_LINE , 0)) == -1)
		{
			perror("recv error.\n");
			exit(1);
		}//if
		buf[n] = '\0';
		//若收到的是exit字符，则代表退出通信
		if(strcmp(buf , exit_message) == 0)
		{
			printf("Server is closed.\n");
            send(sockfd, exit_message, strlen(exit_message), 0);
			close(sockfd);
            continue_recv_message = false;
            gdk_threads_enter();
            gtk_widget_destroy(window);
            gdk_threads_leave();
            pthread_exit(NULL);
            return;
			//exit(0);
		}//if
        else if(strcmp(buf, recv_file_message) == 0) {
            const char *save_file_url = "./ccc.docx";
            FILE *file = fopen(save_file_url, "wb");
            char buf[MAX_LINE];
            int count;

            while((count = recv(sockfd, buf, MAX_LINE, 0)) > 0) {
                if(strcmp(buf, finish_recv_file_message) == 0) {
                    break;
                }
                fwrite(buf, count, 1, file);
            }

            fclose(file);
            printf("finish file\n");
        }
		printf("\nServer: %s\n", buf);
        gdk_threads_enter();
        append_friend_message_record_frame(_friend_name, buf);
        gdk_threads_leave();
	}//while

    close(sockfd);
}

static void server_chat_frame()
{
    //声明套接字

	socklen_t clilen;
	//声明线程ID
	pthread_t recv_tid , send_tid;
	//定义地址结构
	struct sockaddr_in servaddr , cliaddr;
	/*(1) 创建套接字*/
	if((listenfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
	{
		perror("socket error.\n");
		exit(1);
	}//if
    int on =1;
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))
    {
        perror("set failed\n");
        exit(1);
    }
	/*(2) 初始化地址结构*/
	bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	/*(3) 绑定套接字和端口*/
	if(bind(listenfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
	{
		perror("bind error.\n");
		exit(1);
	}//if
	/*(4) 监听*/
	if(listen(listenfd , LISTENQ) < 0)
	{
		perror("listen error.\n");
		exit(1);
	}//if
	/*(5) 接受客户请求，并创建线程处理*/
	clilen = sizeof(cliaddr);

	if((connfd = accept(listenfd , (struct sockaddr *)&cliaddr , &clilen)) < 0)
	{
        
		perror("accept error.\n");
		exit(1);
	}//if
    is_connected = true;
    printf("2\n");
	printf("server: got connection from %s\n", inet_ntoa(cliaddr.sin_addr));
	/*创建子线程处理该客户链接接收消息*/
	if(pthread_create(&recv_tid , NULL , recv_message_server, &connfd) == -1)
	{
		perror("pthread create error.\n");
		exit(1);
	}//if
	/*处理服务器发送消息*/
}

static void server_send()
{
    char msg[MAX_LINE];
	memset(msg , 0 , MAX_LINE);
    strcpy(msg,text);
    if(strcmp(msg,"\0")!=0)	
	{	
		if(strcmp(msg , exit_message) == 0)
		{
			printf("byebye.\n");
			// memset(msg , 0 , MAX_LINE);
			// strcpy(msg , "byebye.");
			send(connfd , msg , strlen(msg) , 0);
			close(connfd);
			//exit(0);
            gdk_threads_enter();
            gtk_main_quit();
            gdk_threads_leave();
		}//if
		if(send(connfd , msg , strlen(msg) , 0) == -1)
		{
			perror("send error.\n");
			exit(1);
		}//if		
	}//while
}

static void client_chat_frame()
{
    //int argc ;
     ///char **argv;
    /*声明套接字和链接服务器地址*/
	pthread_t recv_tid , send_tid;
    struct sockaddr_in servaddr;
    /*判断是否为合法输入*/
    /*if(argc!= 2)
    {
        perror("usage:tcpcli <IPaddress>");
        exit(1);
    }//if
    */
    /*(1) 创建套接字*/
    if((sockfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
    {
        perror("socket error");
        exit(1);
    }//if
    /*(2) 设置链接服务器地址结构*/
    bzero(&servaddr , sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if(inet_pton(AF_INET , "127.0.0.1" , &servaddr.sin_addr) < 0)
    {
        printf("inet_pton error for \n");
        exit(1);
    }//if
    
    /*(3) 发送链接服务器请求*/
    if( connect(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
    {
        
        perror("connect error");
        exit(1);
    }//if	
    is_connected = true;
	/*创建子线程处理该客户链接接收消息*/
	if(pthread_create(&recv_tid , NULL , recv_message_client, &sockfd) == -1)
	{
		perror("pthread create error.\n");
		exit(1);
	}//if	
	/*处理客户端发送消息*/
}
static void client_send()
{
    char msg[MAX_LINE];
	memset(msg , 0 , MAX_LINE);
    strcpy(msg,text);
	if(strcmp(msg,"\0")!=0)	
	{
		if(strcmp(msg , exit_message) == 0)
		{
			printf("byebye.\n");
			// memset(msg , 0 , MAX_LINE);
			// strcpy(msg , "byebye.");
			send(sockfd , msg , strlen(msg) , 0);
			close(sockfd);
            gdk_threads_enter();
            gtk_main_quit();
            gdk_threads_leave();
			//exit(0);
		}//if
		if(send(sockfd , msg , strlen(msg) , 0) == -1)
		{
			perror("send error.\n");
			exit(1);
		}//if
	}//while
}
static void button_ok_close(GtkWidget *widget,gpointer*data)
{
    gtk_widget_destroy(create_window);
}

static void append_self_message_record_frame(const gchar *message) {
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer_output, &end);

    gchar *space="              ";
    gchar *user_says=" says:\n";
    gchar *wrap="\n\n";

    gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&end,user_id,strlen(user_id)); 
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&end,user_says,strlen(user_says));
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&end,space,strlen(space)); 
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&end,message,strlen(message)); 
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&end,wrap,strlen(wrap)); 
}

static void append_friend_message_record_frame(const char *friend_name, const char *message) {
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer_output, &end);

    gchar *space="              ";
    gchar *user_says=" says:\n";
    gchar *wrap="\n\n";

    gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&end,friend_name,strlen(friend_name)); 
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&end,user_says,strlen(user_says));
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&end,space,strlen(space)); 
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&end,message,strlen(message)); 
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&end,wrap,strlen(wrap)); 
}

static void button_event_send(GtkWidget *wiget,GtkWidget *user_id)
{
    GtkTextIter start,end;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer_input),&start,&end);/*获得缓冲区开始和结束位置的Iter*/
    const GtkTextIter s=start,e=end;
    text=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer_input),&s,&e,FALSE);/*获得文本框缓冲区文本*/
    gtk_text_buffer_delete(GTK_TEXT_BUFFER(buffer_input),&start,&end);

    if(strcmp(text,"\0"))
    {
        append_self_message_record_frame(text);
        g_print("%s\n",text);
    }
    else{
        create_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_position(GTK_WINDOW(create_window),GTK_WIN_POS_CENTER);
        gtk_window_set_title(GTK_WINDOW(create_window),"Error Message");
        gtk_window_set_default_size(GTK_WINDOW(create_window),300,150);
        gtk_window_set_resizable(GTK_WINDOW(create_window),FALSE);
        GtkWidget *label_error=gtk_label_new("You can not input nothing!");
        GtkWidget *fixed_error=gtk_fixed_new();
        GtkWidget *button_ok;
        button_ok=gtk_button_new_with_label("OK");
        gtk_widget_set_size_request(GTK_BUTTON(button_ok),100,20);
        gtk_fixed_put(GTK_FIXED(fixed_error),button_ok,110,90);
        gtk_fixed_put(GTK_FIXED(fixed_error),label_error,70,40);
        g_signal_connect(G_OBJECT(button_ok),"clicked",G_CALLBACK(button_ok_close),NULL);
        gtk_container_add(GTK_CONTAINER(create_window),fixed_error);
        gtk_widget_show_all(create_window);
    }  
}
static void button_event_cancel(GtkWidget *widget,gpointer*data)
{
    GtkTextIter start,end;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer_input),&start,&end);
    gtk_text_buffer_delete(GTK_TEXT_BUFFER(buffer_input),&start,&end);
}
static void text_input()
{
    fixed=gtk_fixed_new();
    GtkWidget * text_view_input;
    GtkWidget*scrolled_window_input= gtk_scrolled_window_new(NULL,NULL);
    text_view_input=gtk_text_view_new();
    buffer_input=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_input));
    gtk_widget_set_size_request(text_view_input, 600, 160);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view_input),TRUE);
    
    gtk_widget_set_size_request(scrolled_window_input, 600, 160);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_input),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window_input),text_view_input);
    gtk_fixed_put(GTK_FIXED(fixed),scrolled_window_input,20,625);
}
static void text_output()
{
    GtkWidget*text_view_output;
    GtkWidget*scrolled_window_output=gtk_scrolled_window_new(NULL,NULL);
    text_view_output=gtk_text_view_new();
    buffer_output=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_output));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view_output), FALSE);
    gtk_widget_set_size_request(text_view_output, 600, 500);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view_output),TRUE);
    gtk_widget_set_size_request(scrolled_window_output, 600, 500);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_output),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window_output),text_view_output);
    gtk_fixed_put(GTK_FIXED(fixed),scrolled_window_output,20,100);
}

static void button_send_file_callback(GtkWidget *widget,gpointer*data) {
    printf("open file\n");
    const char *file_url = "./项目文档.docx";

    FILE *file = fopen(file_url, "rb");
    if(!file) {
        printf("open file failed\n");
        return;
    }
    char buf[MAX_LINE];
    strcpy(buf, recv_file_message);
    if(_is_server) {
        send(connfd, buf, MAX_LINE, 0);
        int count;
        while((count = fread(buf, 1, MAX_LINE, file)) > 0) {
            send(connfd, buf, MAX_LINE, 0);
        }
        strcpy(buf, finish_recv_file_message);
        send(connfd, buf, MAX_LINE, 0);
    }
    else {
        send(sockfd, buf, MAX_LINE, 0);
        int count;
        while((count = fread(buf, 1, MAX_LINE, file)) > 0) {
            send(sockfd, buf, MAX_LINE, 0);
        }
        strcpy(buf, finish_recv_file_message);
        send(sockfd, buf, MAX_LINE, 0);
    }

    fclose(file);
}

static void button_send_cancel(gchar *user_id)
{
    static GtkWidget *button_send,*button_cancel, *button_send_file;
    button_send=gtk_button_new_with_label("SEND");
    g_signal_connect(G_OBJECT(button_send), "clicked", G_CALLBACK(button_event_send), user_id);//confirm

    if(_is_server) {
        g_signal_connect(G_OBJECT(button_send), "clicked", G_CALLBACK(server_send), NULL);
    }
    else {
        g_signal_connect(G_OBJECT(button_send), "clicked", G_CALLBACK(client_send), NULL);
    }


    button_cancel=gtk_button_new_with_label("CANCEL");
    g_signal_connect(G_OBJECT(button_cancel), "clicked", G_CALLBACK(button_event_cancel), NULL);//cancel
    gtk_fixed_put(GTK_FIXED(fixed), button_send, 0,0);
    gtk_fixed_put(GTK_FIXED(fixed), button_cancel, 0,0);
    gtk_fixed_move(GTK_FIXED(fixed), button_send, 485, 800);
    gtk_fixed_move(GTK_FIXED(fixed), button_cancel,550,800);

    button_send_file=gtk_button_new_with_label("SEND FILE");
    g_signal_connect(G_OBJECT(button_send_file), "clicked", G_CALLBACK(button_send_file_callback), NULL);//cancel
    gtk_fixed_put(GTK_FIXED(fixed), button_send_file, 0,0);
    gtk_fixed_move(GTK_FIXED(fixed), button_send_file, 350, 800);
}

static GtkWidget * create_imagetext_hbox(const char *image_path)
   {
        GtkWidget *box;
        GtkWidget *image; 
        const GdkPixbuf *src_pixbuf;
        GdkPixbuf *dest_pixbuf; 
        src_pixbuf = gdk_pixbuf_new_from_file(image_path, NULL);  //将src_pixbuf设置成屏幕大小 
        dest_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, 60,60, GDK_INTERP_HYPER); //从dest_pixbuf中读取图片存于image中 
        image = gtk_image_new_from_pixbuf(dest_pixbuf); 
        box = gtk_hbox_new (FALSE, 0);
        gtk_container_set_border_width (GTK_CONTAINER (box), 2);
        gtk_box_pack_start (GTK_BOX (box), image, FALSE, FALSE, 3);
        gtk_widget_set_size_request(box,-1,60);
        gtk_widget_show_all (box);
        return box;
   }
static GtkWidget * create_image_button(const char *image_path)
{   
        GtkWidget *button;
        GtkWidget *box;
        box = create_imagetext_hbox(image_path);
        button = gtk_button_new ();
        gtk_container_add (GTK_CONTAINER (button), box);
        gtk_widget_show (button);
        return button;
}

static void right_label()
{
    GtkWidget *label=gtk_label_new("This is for id  adress");
    gtk_fixed_put(GTK_FIXED(fixed),label,700,400);
}

static void top_label(gchar*user_id)
{
    GtkWidget*  top_label_button=create_image_button("1.jpg");
    GtkWidget* label_top=gtk_label_new(user_id);
    gtk_widget_set_size_request(GTK_BUTTON(top_label_button),60,60);
    //g_signal_connect(G_OBJECT(top_label_button), "clicked", G_CALLBACK(personal information), NULL);
    gtk_fixed_put(GTK_FIXED(fixed),top_label_button,20,10);
    gtk_fixed_put(GTK_FIXED(fixed),label_top,150,30);
}

static GtkWidget* create_frame_chat_window(gchar*user_id)
{
    static GtkWidget *window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window),"chat frame");
    gtk_window_set_default_size(GTK_WINDOW(window),950,850);
    gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
   // gtk_container_set_border_width(GTK_CONTAINER(window) , 150);
    text_input();
    text_output();
    button_send_cancel(user_id);
    right_label();
    top_label(user_id);
    gtk_container_add(GTK_CONTAINER(window),fixed);
    return window;

}

void create_chat_window(const char *self_name, const char *friend_name, const char *friend_ip, bool is_server) {
    gdk_threads_init();
    gtk_init(1 , NULL);
    strcpy(user_id, self_name);
    _is_server = is_server;
    _friend_name = friend_name;
    _friend_ip = friend_ip;
    continue_recv_message = true;
    is_connected = false;

    exit_message = "b|y|e\n";
    recv_file_message = "f|i|l|e\n";
    finish_recv_file_message = "f|i|n|i|s|h|f|i|l|e\n";
    
    if(is_server) {
        // pthread_t server_thread;
        // pthread_create(&server_thread, NULL, server_chat_frame, NULL);
        printf("server\n");
        g_thread_create((GThreadFunc)server_chat_frame, NULL, FALSE, NULL);
        //server_chat_frame();
        window=create_frame_chat_window(friend_name);
        gtk_widget_show_all(window);
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
        g_signal_connect(window, "delete_event", G_CALLBACK(stop_connection), NULL);
        
        gtk_main();
        
    }
    else {
        printf("client\n");
        client_chat_frame();
        window=create_frame_chat_window(friend_name);
        gtk_widget_show_all(window);
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
        g_signal_connect(window, "delete_event", G_CALLBACK(stop_connection), NULL);
        
        gtk_main();
        
    }

}
