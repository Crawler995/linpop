/*
 * Author: cjx
 * Date: 2019/8/31
 * 
 */
#include "information.h"

GdkPixbuf *create_pixbuf(const gchar *filename)
{
    GdkPixbuf *pixbuf;
    GError *error=NULL;
    pixbuf=gdk_pixbuf_new_from_file(filename,&error);
    if(!pixbuf)
    {
        fprintf(stderr,"%s\n",error->message);
        g_error_free(error);
    }
    return pixbuf;
}

void window_delete_event()
{
    gtk_widget_hide_all(window);
}

void edit_window_delete_event()
{
    gtk_widget_hide_all(edit_window);
    gtk_widget_show_all(window);
}

void edit_event()
{
    edit_initWindow();
    gtk_widget_hide_all(window);
}

void save_event(GtkWidget *widget,gpointer *data)
{
    gtk_widget_hide_all(edit_window);

    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(user_name_buffer),&start,&end);
    user_name_text=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(user_name_buffer),&start,&end,FALSE);
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(user_id_buffer),&start,&end);
    user_id_text=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(user_id_buffer),&start,&end,FALSE);
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(user_sex_buffer),&start,&end);
    user_sex_text=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(user_sex_buffer),&start,&end,FALSE);
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(user_age_buffer),&start,&end);
    user_age_text=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(user_age_buffer),&start,&end,FALSE);
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(user_birthday_buffer),&start,&end);
    user_birthday_text=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(user_birthday_buffer),&start,&end,FALSE);
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(user_zodiac_buffer),&start,&end);
    user_zodiac_text=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(user_zodiac_buffer),&start,&end,FALSE);
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(user_mailbox_buffer),&start,&end);
    user_mailbox_text=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(user_mailbox_buffer),&start,&end,FALSE);
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(user_address_buffer),&start,&end);
    user_address_text=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(user_address_buffer),&start,&end,FALSE);
    
    user_name_label_text=g_string_append(user_name_label_text,user_name_text );
    user_id_label_text=g_string_append( user_id_label_text,user_id_text );
    user_sex_label_text=g_string_append( user_sex_label_text,user_sex_text );
    user_age_label_text=g_string_append( user_age_label_text,user_age_text );
    user_birthday_label_text=g_string_append( user_birthday_label_text,user_birthday_text );
    user_zodiac_label_text=g_string_append( user_zodiac_label_text,user_zodiac_text );
    user_mailbox_label_text=g_string_append(user_mailbox_label_text,user_mailbox_text );
    user_address_label_text=g_string_append( user_address_label_text,user_address_text );
    gtk_label_set_text(user_name_window_label,user_name_label_text->str);
    gtk_label_set_text(user_id_window_label,user_id_label_text->str);
    gtk_label_set_text(user_sex_window_label,user_sex_label_text->str);
    gtk_label_set_text(user_age_window_label,user_age_label_text->str);
    gtk_label_set_text(user_birthday_window_label,user_birthday_label_text->str);
    gtk_label_set_text(user_zodiac_window_label,user_zodiac_label_text->str);
    gtk_label_set_text(user_mailbox_window_label,user_mailbox_label_text->str);
    gtk_label_set_text(user_address_window_label,user_address_label_text->str);

    gtk_widget_show_all(window);
}

void edit_initWindow()
{
    edit_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(edit_window),"Edit Personal Information");
    gtk_window_set_position(GTK_WINDOW(edit_window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(edit_window), 50);
    gtk_window_set_resizable(GTK_WINDOW(edit_window),FALSE);
    gtk_window_set_icon(GTK_WINDOW(window),create_pixbuf("./images/head_portrait.jpg"));
    g_signal_connect_swapped(G_OBJECT(edit_window),"destroy",G_CALLBACK(edit_window_delete_event),NULL);
    table = gtk_table_new(3,2,FALSE);
    gtk_container_add(GTK_CONTAINER(edit_window),table);

    user_name_editwindow_label=gtk_label_new("Name");
    gtk_table_attach(GTK_TABLE(table),user_name_editwindow_label,0,1,0,1,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_id_editwindow_label=gtk_label_new("Id");
    gtk_table_attach(GTK_TABLE(table),user_id_editwindow_label,0,1,1,2,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_sex_editwindow_label=gtk_label_new("Sex");
    gtk_table_attach(GTK_TABLE(table),user_sex_editwindow_label,0,1,2,3,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_age_editwindow_label=gtk_label_new("Age");
    gtk_table_attach(GTK_TABLE(table),user_age_editwindow_label,0,1,3,4,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_birthday_editwindow_label=gtk_label_new("Birthday");
    gtk_table_attach(GTK_TABLE(table),user_birthday_editwindow_label,0,1,4,5,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_zodiac_editwindow_label=gtk_label_new("Zodiac");
    gtk_table_attach(GTK_TABLE(table),user_zodiac_editwindow_label,0,1,5,6,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_mailbox_editwindow_label=gtk_label_new("Mailbox");
    gtk_table_attach(GTK_TABLE(table),user_mailbox_editwindow_label,0,1,6,7,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_address_editwindow_label=gtk_label_new("Address");
    gtk_table_attach(GTK_TABLE(table),user_address_editwindow_label,0,1,7,8,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    save_information_button = gtk_button_new_with_label("Save");
    gtk_table_attach(GTK_TABLE(table),save_information_button,0,2,10,11,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    g_signal_connect(G_OBJECT(save_information_button),"clicked",G_CALLBACK(save_event),NULL);

    user_name_textbox=gtk_text_view_new();
    user_name_buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(user_name_textbox));
    gtk_widget_set_size_request(user_name_textbox,100,25);
    gtk_table_attach(GTK_TABLE(table),user_name_textbox,1,2,0,1,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_id_textbox=gtk_text_view_new();
    user_id_buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(user_id_textbox));
    gtk_widget_set_size_request(user_id_textbox,100,25);
    gtk_table_attach(GTK_TABLE(table),user_id_textbox,1,2,1,2,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_sex_textbox=gtk_text_view_new();
    user_sex_buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(user_sex_textbox));
    gtk_widget_set_size_request(user_sex_textbox,100,25);
    gtk_table_attach(GTK_TABLE(table),user_sex_textbox,1,2,2,3,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_age_textbox=gtk_text_view_new();
    user_age_buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(user_age_textbox));
    gtk_widget_set_size_request(user_age_textbox,100,25);
    gtk_table_attach(GTK_TABLE(table),user_age_textbox,1,2,3,4,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_birthday_textbox=gtk_text_view_new();
    user_birthday_buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(user_birthday_textbox));
    gtk_widget_set_size_request(user_birthday_textbox,100,25);
    gtk_table_attach(GTK_TABLE(table),user_birthday_textbox,1,2,4,5,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_zodiac_textbox=gtk_text_view_new();
    user_zodiac_buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(user_zodiac_textbox));
    gtk_widget_set_size_request(user_zodiac_textbox,100,25);
    gtk_table_attach(GTK_TABLE(table),user_zodiac_textbox,1,2,5,6,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_mailbox_textbox=gtk_text_view_new();
    user_mailbox_buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(user_mailbox_textbox));
    gtk_widget_set_size_request(user_mailbox_textbox,100,25);
    gtk_table_attach(GTK_TABLE(table),user_mailbox_textbox,1,2,6,7,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_address_textbox=gtk_text_view_new();
    user_address_buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(user_address_textbox));
    gtk_widget_set_size_request(user_address_textbox,100,25);
    gtk_table_attach(GTK_TABLE(table),user_address_textbox,1,2,7,8,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);

    gtk_widget_show_all(edit_window);
}

GtkWidget *initWindow()
{
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Personal Information");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 90);
    gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
    gtk_window_set_icon(GTK_WINDOW(window),create_pixbuf("./images/head_portrait.jpg"));
    g_signal_connect_swapped(G_OBJECT(window),"destroy",G_CALLBACK(window_delete_event),NULL);
    table = gtk_table_new(3,2,FALSE);
    gtk_container_add(GTK_CONTAINER(window),table);

    user_name_window_label=gtk_label_new("Name");
    gtk_table_attach(GTK_TABLE(table),user_name_window_label,0,1,0,1,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_id_window_label=gtk_label_new("Id");
    gtk_table_attach(GTK_TABLE(table),user_id_window_label,0,1,2,3,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_sex_window_label=gtk_label_new("Sex");
    gtk_table_attach(GTK_TABLE(table),user_sex_window_label,0,1,3,4,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_age_window_label=gtk_label_new("Age");
    gtk_table_attach(GTK_TABLE(table),user_age_window_label,0,1,4,5,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_birthday_window_label=gtk_label_new("Birthday");
    gtk_table_attach(GTK_TABLE(table),user_birthday_window_label,0,1,5,6,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_zodiac_window_label=gtk_label_new("Zodiac");
    gtk_table_attach(GTK_TABLE(table),user_zodiac_window_label,0,1,6,7,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_mailbox_window_label=gtk_label_new("Mailbox");
    gtk_table_attach(GTK_TABLE(table),user_mailbox_window_label,0,1,7,8,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    user_address_window_label=gtk_label_new("Address");
    gtk_table_attach(GTK_TABLE(table),user_address_window_label,0,1,8,9,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    edit_information_button = gtk_button_new_with_label("Edit");
    gtk_table_attach(GTK_TABLE(table),edit_information_button,0,2,10,11,GTK_FILL | GTK_SHRINK,GTK_FILL | GTK_SHRINK,5,5);
    g_signal_connect(G_OBJECT(edit_information_button),"clicked",G_CALLBACK(edit_event),NULL);

    user_name_label_text = g_string_new("Name:");
    user_id_label_text = g_string_new("Id:");
    user_sex_label_text = g_string_new("Sex:");
    user_age_label_text = g_string_new("Age:");
    user_birthday_label_text = g_string_new("Birthday:");
    user_zodiac_label_text = g_string_new("Zodiac:");
    user_mailbox_label_text = g_string_new("Mailbox:");
    user_address_label_text = g_string_new("Address:");

    gtk_widget_show_all(window);
    return window;
}

int main(int argc,char **argv)
{
    program_argc = argc;
    program_argv = argv;

    gtk_init(&argc,&argv);
    window = initWindow();
    gtk_main();
    return 0;
}