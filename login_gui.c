/*
 * Author: 张青龙
 * Date: 2019/8/27
 * 
 */

#include "login_gui.h"
#include "register_gui.h"
#include "data.h"
#include "network.h"
#include "string_linked_list.h"
#include <stdbool.h>

static void login_handle(GtkWidget *button, gpointer data) {
    const gchar *username_text = gtk_entry_get_text(GTK_ENTRY(username_input));
    const gchar *password_text = gtk_entry_get_text(GTK_ENTRY(password_input));

    g_print("账号：%s 密码：%s\n", username_text, password_text);

    gboolean login_success = verify_user_info_in_database(username_text, password_text);
    GdkColor green = {0, 0, 0xffff, 0}, red = {0, 0xffff, 0, 0};
    
    gtk_label_set_text(GTK_LABEL(login_label), login_success ? "登录成功！" : "登录失败，请重试！");
    gtk_widget_modify_fg(login_label, GTK_STATE_NORMAL, login_success ? &green : &red);

    if(!login_success) return;
    
    user_name = username_text;
    set_user_ip_address(get_self_ip_address());

    set_user_online(true);

    // add_user_friend_list("ccccc");
    // add_user_friend_list("ccc");

    // string_node *friend_list_head = get_user_friend_list(), *p = friend_list_head->next;
    // while(p) {
    //     printf("%s\n", p->string);
    //     p = p->next;
    // }
    // delete_string_linked_list(friend_list_head);
}

static void register_handle(GtkWidget *button, gpointer data) {
    g_print("打开注册界面\n");

    create_register_window(program_argc, program_argv);
}


static GtkWidget* initWindow() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "linpop");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 300);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 50);

    layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), layout);

    login_label = gtk_label_new("linpop用户登录");
    username_label = gtk_label_new("账号");
    username_input = gtk_entry_new();
    password_label = gtk_label_new("密码");
    password_input = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_input), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(password_input), '*');

    login_button = gtk_button_new_with_label("登录");
    g_signal_connect(G_OBJECT(login_button), "clicked", G_CALLBACK(login_handle), NULL);
    register_button = gtk_button_new_with_label("注册");
    g_signal_connect(G_OBJECT(register_button), "clicked", G_CALLBACK(register_handle), NULL);

    gtk_label_set_xalign(GTK_LABEL(username_label), 0);
    gtk_label_set_xalign(GTK_LABEL(password_label), 0);

    gtk_box_pack_start(GTK_BOX(layout), login_label, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(layout), username_label, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(layout), username_input, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(layout), password_label, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(layout), password_input, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(layout), login_button, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(layout), register_button, TRUE, TRUE, 5);

    return window;
}

int main(int argc, char **argv) {
    program_argc = argc;
    program_argv = argv;

    gtk_init(&argc, &argv);
    GtkWidget *window = initWindow();

    gtk_widget_show_all(window);

    connect_database();

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();

    destory_database_connection();

    return 0;
}