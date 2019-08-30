/*
 * Author: 张青龙
 * Date: 2019/8/28
 * 
 */

#include "register_gui.h"
#include "data.h"

static gboolean password_security_pass(const gchar *password) {
    gboolean all_number = TRUE;
    gboolean all_lower_c = TRUE;
    gboolean all_upper_c = TRUE;
    gboolean length_not_enough = strlen(password) < 8;

    for(int i = 0; i < strlen(password); i++) {
        if(password[i] < '0' || password[i] > '9') {
            all_number = FALSE;
        }
        if(password[i] < 'a' || password[i] > 'z') {
            all_lower_c = FALSE;
        }
        if(password[i] < 'A' || password[i] > 'Z') {
            all_upper_c = FALSE;
        }
    }

    return !(all_number || all_lower_c || all_upper_c || length_not_enough);
}

static void register_handle(GtkWidget *button, gpointer data) {
    const gchar *username_text = gtk_entry_get_text(GTK_ENTRY(username_input));
    const gchar *password_text = gtk_entry_get_text(GTK_ENTRY(password_input));
    const gchar *again_password_text = gtk_entry_get_text(GTK_ENTRY(again_password_input));

    g_print("账号：%s 密码：%s\n", username_text, password_text);

    GdkColor green = {0, 0, 0xffff, 0}, red = {0, 0xffff, 0, 0};

    gboolean username_not_too_long = strlen(username_text) <= 10;
    gboolean password_same = !strcmp(password_text, again_password_text);
    gboolean username_not_empty = strcmp(username_text, "");
    gboolean security_pass = password_security_pass(password_text);
    gboolean username_not_exist = !find_user_in_database(username_text);

    gboolean register_success = password_same && security_pass && username_not_empty && username_not_exist;

    gchar register_status_tip[50] = "注册成功！";
    if(!username_not_empty) strcpy(register_status_tip, "用户名不能为空！");
    else if(!username_not_too_long) strcpy(register_status_tip, "用户名必须小于10个字符");
    else if(!username_not_exist) strcpy(register_status_tip, "用户名已存在！");
    else if(!password_same) strcpy(register_status_tip, "两次输入密码不一致！");
    else if(!security_pass) strcpy(register_status_tip, "密码强度过低！");
    
    gtk_label_set_text(GTK_LABEL(register_label), register_status_tip);
    gtk_widget_modify_fg(register_label, GTK_STATE_NORMAL, register_success ? &green : &red);
    

    if(register_success) {
        insert_user_info_to_collection(username_text, password_text);
    }
}

static GtkWidget* initWindow() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "linpop");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 300);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 50);

    layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), layout);

    register_label = gtk_label_new("linpop用户注册");
    username_label = gtk_label_new("账号");
    username_input = gtk_entry_new();
    password_label = gtk_label_new("密码");
    password_input = gtk_entry_new();
    again_password_label = gtk_label_new("再次输入密码");
    again_password_input = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_input), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(password_input), '*');
    gtk_entry_set_visibility(GTK_ENTRY(again_password_input), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(again_password_input), '*');

    register_button = gtk_button_new_with_label("注册");
    g_signal_connect(G_OBJECT(register_button), "clicked", G_CALLBACK(register_handle), NULL);

    gtk_label_set_xalign(GTK_LABEL(username_label), 0);
    gtk_label_set_xalign(GTK_LABEL(password_label), 0);
    gtk_label_set_xalign(GTK_LABEL(again_password_label), 0);

    gtk_box_pack_start(GTK_BOX(layout), register_label, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(layout), username_label, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(layout), username_input, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(layout), password_label, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(layout), password_input, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(layout), again_password_label, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(layout), again_password_input, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(layout), register_button, TRUE, TRUE, 5);

    return window;
}

void create_register_window(int argc, char **argv) {
    gtk_init(&argc, &argv);
    GtkWidget *window = initWindow();

    gtk_widget_show_all(window);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
}