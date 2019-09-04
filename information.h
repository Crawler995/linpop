/*
 * Author: cjx
 * Date: 2019/8/31
 * 
 */
#ifndef information_H
#define information_H
#include <gtk/gtk.h>

static int program_argc;
static char **program_argv;

static GtkWidget *window,*edit_window,*table;
static GtkWidget *user_name_window_label,*user_id_window_label,*user_sex_window_label,
                    *user_age_window_label,*user_birthday_window_label,*user_zodiac_window_label,
                    *user_mailbox_window_label,*user_address_window_label,
                    *user_name_editwindow_label,*user_id_editwindow_label,*user_sex_editwindow_label,
                    *user_age_editwindow_label,*user_birthday_editwindow_label,*user_zodiac_editwindow_label,
                    *user_mailbox_editwindow_label,*user_address_editwindow_label;
static GtkWidget  *user_name_textbox,*user_id_textbox,*user_sex_textbox,
                    *user_age_textbox,*user_birthday_textbox,*user_zodiac_textbox,
                    *user_mailbox_textbox,*user_address_textbox;
static GtkTextBuffer *user_name_buffer,*user_id_buffer,*user_sex_buffer,
                    *user_age_buffer,*user_birthday_buffer,*user_zodiac_buffer,
                    *user_mailbox_buffer,*user_address_buffer;                  
static GtkWidget *edit_information_button,*save_information_button,*head_portrait_image;
static GtkTextIter *Iter,start,end;
static gchar *user_name_text,*user_id_text,*user_sex_text,
                    *user_age_text,*user_birthday_text,*user_zodiac_text,
                    *user_mailbox_text,*user_address_text;
static GString *user_name_label_text,*user_id_label_text,*user_sex_label_text,
                    *user_age_label_text,*user_birthday_label_text,*user_zodiac_label_text,
                    *user_mailbox_label_text,*user_address_label_text;

static GdkPixbuf *create_pixbuf(const gchar *filename);//头像
static void window_delete_event();
static void edit_window_delete_event();
static void edit_event();
static void save_event(GtkWidget *widget,gpointer *data);
static void edit_initWindow();
static GtkWidget* initWindow();

#endif