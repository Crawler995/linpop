#include<gtk/gtk.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

GtkTextBuffer*buffer_input;
GtkTextBuffer*buffer_output;
GtkWidget*fixed;
GtkWidget *create_window;
static void button_ok_close(GtkWidget *widget,gpointer*data)
{
    gtk_widget_destroy(create_window);
}
static void button_event_send(GtkWidget *wiget, gchar*user_id)
{
    gchar *text;
    gchar *space="              ";
    gchar *user_name="he ";
    printf("%s",user_name);
    gchar *user_says=" says:\n";
    gchar *wrap="\n\n";
    GtkTextIter start2,end2;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer_input),&start2,&end2);/*获得缓冲区开始和结束位置的Iter*/
    const GtkTextIter s=start2,e=end2;
    text=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer_input),&s,&e,FALSE);/*获得文本框缓冲区文本*/
    gtk_text_buffer_delete(GTK_TEXT_BUFFER(buffer_input),&start2,&end2);
    if(strcmp(text,"\0"))
    {
        gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer_output),&start2,&end2);
        gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&start2,user_name,strlen(user_name)); 
        gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&start2,user_says,strlen(user_says));
        gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&start2,space,strlen(space)); 
        gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&start2,text,strlen(text)); 
        gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_output),&start2,wrap,strlen(wrap)); 
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
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer_output),&start,&end);
    gtk_text_buffer_delete(GTK_TEXT_BUFFER(buffer_output),&start,&end);
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
static void button_send_cancel(gchar *user_id)
{
    static GtkWidget *button_send,*button_cancel;
    button_send=gtk_button_new_with_label("SEND");
    g_signal_connect(G_OBJECT(button_send), "clicked", G_CALLBACK(button_event_send), user_id);//confirm
   
    button_cancel=gtk_button_new_with_label("CANCEL");
    g_signal_connect(G_OBJECT(button_cancel), "clicked", G_CALLBACK(button_event_cancel), NULL);//cancel
    gtk_fixed_put(GTK_FIXED(fixed), button_send, 0,0);
    gtk_fixed_put(GTK_FIXED(fixed), button_cancel, 0,0);
    gtk_fixed_move(GTK_FIXED(fixed), button_send, 485, 800);
    gtk_fixed_move(GTK_FIXED(fixed), button_cancel,550,800);
}

static GtkWidget * create_imagetext_hbox_frame(const char *image_path)
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
static GtkWidget * create_image_button_frame(const char *image_path)
{   
        GtkWidget *button;
        GtkWidget *box;
        box = create_imagetext_hbox_frame(image_path);
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
    GtkWidget*  top_label_button=create_image_button_frame("1.jpg");
    GtkWidget* label_top=gtk_label_new(user_id);
    gtk_widget_set_size_request(GTK_BUTTON(top_label_button),60,60);
    //g_signal_connect(G_OBJECT(top_label_button), "clicked", G_CALLBACK(personal information), NULL);
    gtk_fixed_put(GTK_FIXED(fixed),top_label_button,20,10);
    gtk_fixed_put(GTK_FIXED(fixed),label_top,150,30);
}

static GtkWidget* create_frame_chat(gchar*user_id)
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


