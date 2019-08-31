#include<gtk/gtk.h>
#include<stdlib.h>
GtkTextBuffer *buffer_input_friend1;
GtkTextBuffer *buffer_input_friend2;
GtkTextBuffer *buffer_input_tree;
GtkTreeViewColumn *col;
GtkCellRenderer *renderer;

GtkTreeModel *model;
GtkTreeStore *treestore;
GtkTreeIter toplevel;
static  GtkWidget  *window;
static GtkWidget *view;
static GtkWidget *vbox;
static GtkTreeSelection *selection;
static GtkWidget *statusbar;
static GtkWidget *head_button;
static GtkWidget *button_head;
GtkTreeStore *treestore;
GtkTreeIter child;

GtkTreeIter  toplevel_set[10];
int tree=1;




enum
{
    COLUMN = 0,
    NUM_COLS
} ;


void button_add_friend_sure_clicked(GtkWidget *widgt,gpointer *date){
   gchar *textc;
   GtkTextIter start,end;
   gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer_input_friend1),&start,&end);/*获得缓冲区开始和结束位置的Iter*/
   textc=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer_input_friend1),&start,&end,FALSE);
   g_print("%s\n",textc);

    char *textc1;
    GtkTextIter start1,end1;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer_input_friend2),&start1,&end1);
    textc1=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer_input_friend2),&start1,&end1,FALSE);
   
    int tree_number;
    tree_number=atoi(textc1);

    
    gtk_tree_store_append(treestore, &child, &toplevel_set[tree_number-1]);
    gtk_tree_store_set(treestore, &child,COLUMN, textc,-1);

    gtk_text_buffer_delete(GTK_TEXT_BUFFER(buffer_input_friend1),&start,&end);
    gtk_text_buffer_delete(GTK_TEXT_BUFFER(buffer_input_friend2),&start1,&end1);
}

void creat_new_firend()
{}





void button_add_friend_clicked(){

    GtkWidget* window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);//出现时位于显示器中部
    gtk_window_set_title(GTK_WINDOW(window),"add friend");
    gtk_widget_set_size_request(window,300,100);
    gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
    GtkWidget *fixed_add_friend=gtk_fixed_new();

    GtkWidget* text = gtk_text_view_new();
    buffer_input_friend1=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
    gtk_fixed_put(GTK_FIXED(fixed_add_friend), text, 10,30);
    gtk_widget_set_size_request(GTK_WIDGET(text),120,30);


    GtkWidget* text2 = gtk_text_view_new();
    buffer_input_friend2=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text2));
    gtk_fixed_put(GTK_FIXED(fixed_add_friend), text2, 170,30);
    gtk_widget_set_size_request(GTK_WIDGET(text2),120,30);

    GtkWidget *button_add_friend_sure=gtk_button_new_with_label("确认添加");
    gtk_fixed_put(GTK_FIXED(fixed_add_friend),button_add_friend_sure , 140,70);
    g_signal_connect(G_OBJECT(button_add_friend_sure), "clicked",  G_CALLBACK(button_add_friend_sure_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(window),fixed_add_friend);
    gtk_widget_show_all(window);
}

void button_add_tree_sure_clicked(GtkWidget *widgt,gpointer *date){
   
    tree++;
    gchar *textc;

    GtkTextIter start,end;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer_input_tree),&start,&end);/*获得缓冲区开始和结束位置的Iter*/
    textc=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer_input_tree),&start,&end,FALSE);/*获得文本框缓冲区文本*/
    g_print("%s\n",textc);

    gtk_tree_store_append(treestore, &toplevel_set[tree], NULL);
    gtk_tree_store_set(treestore, &toplevel_set[tree] ,COLUMN,textc,-1);   

    gtk_text_buffer_delete(GTK_TEXT_BUFFER(buffer_input_tree),&start,&end);
}


void button_add_tree_clicked(){
   
    GtkWidget* window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);//出现时位于显示器中部
    gtk_window_set_title(GTK_WINDOW(window),"add tree");
    gtk_widget_set_size_request(window,300,100);
    gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
    GtkWidget *fixed_add_tree=gtk_fixed_new();
   
    GtkWidget* text1 = gtk_text_view_new();
    buffer_input_tree=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text1));
    gtk_fixed_put(GTK_FIXED(fixed_add_tree), text1, 10,30);  
    gtk_widget_set_size_request(GTK_WIDGET(text1),120,30);
    GtkWidget *button_add_tree_sure=gtk_button_new_with_label("确认添加");
    gtk_fixed_put(GTK_FIXED(fixed_add_tree),button_add_tree_sure , 140,30);
    g_signal_connect(G_OBJECT(button_add_tree_sure), "clicked",  G_CALLBACK(button_add_tree_sure_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(window),fixed_add_tree);
    gtk_widget_show_all(window);
}



void  on_changed(GtkWidget *widget, gpointer statusbar)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    char *value;
    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) {
        gtk_tree_model_get(model, &iter, COLUMN, &value,  -1);
        gtk_statusbar_push(GTK_STATUSBAR(statusbar), gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar),value), value);
        g_free(value);
    }
}




static GtkTreeModel *create_and_fill_model (void)
{
    
    treestore = gtk_tree_store_new(NUM_COLS,G_TYPE_STRING);
    
    gtk_tree_store_append(treestore, & toplevel_set[0], NULL);
    
    gtk_tree_store_set(treestore, & toplevel_set[0],COLUMN, "tree 1",-1);
    
    
    gtk_tree_store_append(treestore, &child, &toplevel_set[0]);
    gtk_tree_store_set(treestore, &child, COLUMN, "friend1 in tree1", -1);
    gtk_tree_store_append(treestore, &child, &toplevel_set[0]);
    gtk_tree_store_set(treestore, &child,COLUMN, "friend2 in tree1",-1);
    gtk_tree_store_append(treestore, &child, &toplevel_set[0]);
    gtk_tree_store_set(treestore, &child,COLUMN, "friend3 in tree1",-1);
    
    gtk_tree_store_append(treestore, &toplevel_set[1], NULL);
    gtk_tree_store_set(treestore, &toplevel_set[1], COLUMN, "tree 2", -1);
    gtk_tree_store_append(treestore, &child, &toplevel_set[1]);
    gtk_tree_store_set(treestore, &child,COLUMN, "friend1 in tree2", -1);
    gtk_tree_store_append(treestore, &child, &toplevel_set[1]);
    gtk_tree_store_set(treestore, &child,COLUMN, "friend2 in tree2", -1);
    gtk_tree_store_append(treestore, &child, &toplevel_set[1]);
    gtk_tree_store_set(treestore, &child,COLUMN, "friend3 in tree2",-1);
    return GTK_TREE_MODEL(treestore);
}
 



 GtkWidget * create_imagetext_hbox(const char *image_path)
   {
         GtkWidget *box;
         GtkWidget *image; 
         const GdkPixbuf *src_pixbuf;
         GdkPixbuf *dest_pixbuf; 
          src_pixbuf = gdk_pixbuf_new_from_file(image_path, NULL); 
     //将src_pixbuf设置成屏幕大小 
         dest_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, 120,120, GDK_INTERP_HYPER);
     //从dest_pixbuf中读取图片存于image中 
         image = gtk_image_new_from_pixbuf(dest_pixbuf); 
         box = gtk_hbox_new (FALSE, 0);
         gtk_container_set_border_width (GTK_CONTAINER (box), 2);
         gtk_box_pack_start (GTK_BOX (box), image, FALSE, FALSE, 3);
         gtk_widget_set_size_request(box,-1,100);

         gtk_widget_show_all (box);
        return box;
}
GtkWidget * create_image_button(const char *image_path)
{        GtkWidget *button;
         GtkWidget *box;


         box = create_imagetext_hbox(image_path);
 
         button = gtk_button_new ();
         gtk_container_add (GTK_CONTAINER (button), box);
         gtk_widget_show (button);
         return button;
}
 



 int main ( int argc ,char **argv){ //?
 
  
    gtk_init(&argc,&argv);

    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);//出现时位于显示器中部
    gtk_window_set_title(GTK_WINDOW(window),"linpop from 54 ");
    gtk_widget_set_size_request(window,300,700);
    gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
    GtkWidget *fixed_head=gtk_fixed_new();



    vbox = gtk_vbox_new(FALSE, 2);
    gtk_fixed_put(GTK_FIXED(fixed_head), vbox, 0,200);
    gtk_widget_set_size_request(GTK_WIDGET(vbox),300,500);
    view = gtk_tree_view_new();
    col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(col, "friend tree");
    gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer,"text", COLUMN);

    model = create_and_fill_model();

    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
    g_object_unref(model);

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
    gtk_box_pack_start(GTK_BOX(vbox), view, TRUE, TRUE, 1);
    statusbar = gtk_statusbar_new();
    gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 1);    //liebiao

     
       //左上头像
     

    button_head=gtk_button_new_with_label("touxiang ");
    button_head=create_image_button(  "1.jpg");
    gtk_fixed_put(GTK_FIXED(fixed_head), button_head, 10,30);  //buttons position
    gtk_widget_set_size_request(GTK_WIDGET(button_head),100,70); //button size
     GtkWidget *button_add_tree=gtk_button_new_with_label("add tree ");
    gtk_fixed_put(GTK_FIXED(fixed_head),button_add_tree , 140,170);
    GtkWidget *button_add_friend=gtk_button_new_with_label("add friend ");
    gtk_fixed_put(GTK_FIXED(fixed_head),button_add_friend , 220,170);
    g_signal_connect(G_OBJECT(button_add_tree), "clicked",  G_CALLBACK(button_add_tree_clicked), NULL);
    g_signal_connect(G_OBJECT(button_add_friend), "clicked",  G_CALLBACK(button_add_friend_clicked), NULL);
    GtkWidget * label_name=gtk_label_new("name");
    gtk_fixed_put(GTK_FIXED(fixed_head), label_name, 160,30);
    gtk_widget_set_size_request(label_name,10,40);
    GtkWidget * label_signature=gtk_label_new("signature**********");  //yinggai huanhangh
    gtk_label_set_line_wrap(GTK_LABEL(label_signature),TRUE);
    gtk_fixed_put(GTK_FIXED(fixed_head), label_signature, 160,80);
    gtk_widget_set_size_request(label_signature,10,40);
    gtk_container_add(GTK_CONTAINER(window),fixed_head);
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();
    
return 0;

    

     
 }
