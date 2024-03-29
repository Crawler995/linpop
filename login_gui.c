/*
 * Author: 张青龙
 * Date: 2019/8/27
 * 
 */

#include "login_gui.h"
#include "register_gui.h"
#include "global_info.h"
#include "data.h"
#include "network.h"
#include "friend_linked_list.h"
#include "friend_interface.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/stat.h>  

static void login_handle(GtkWidget *button, gpointer data) {
    const gchar *username_text = gtk_entry_get_text(GTK_ENTRY(username_input));
    const gchar *password_text = gtk_entry_get_text(GTK_ENTRY(password_input));

    g_print("账号：%s 密码：%s\n", username_text, password_text);

    gboolean login_success = verify_user_info_in_database(username_text, password_text);
    GdkColor green = {0, 0, 0xffff, 0}, red = {0, 0xffff, 0, 0};
    
    gtk_label_set_text(GTK_LABEL(login_label), login_success ? "登录成功！" : "登录失败，请重试！");
    gtk_widget_modify_fg(login_label, GTK_STATE_NORMAL, login_success ? &green : &red);

    if(!login_success) return;
    
    global_login_user_name = (char*)malloc(sizeof(char) * 50);
    strcpy(global_login_user_name, username_text);

    user_ip = (char*)malloc(sizeof(char) * 50);
    strcpy(user_ip, get_self_ip_address());

    set_user_ip_address(user_ip);

    set_user_online(true);

    if(access("./head_png/", F_OK)) {
        mkdir("head_png", S_IRWXU); 
    }
    char cmd[100];
    sprintf(cmd, "./create_head md5 \"%s\" \"./head_png/%s.png\"", username_text, username_text);
    system(cmd);
    //gdk_threads_enter();
    //g_thread_create((GThreadFunc)create_friend_interface, NULL, FALSE, NULL);
    gtk_widget_hide(GTK_WIDGET(window));
    destroy_window();
    create_friend_interface();
    //gdk_threads_leave();
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

void destroy_window() {
    gtk_widget_destroy(window);
}

int main(int argc, char **argv) {
    program_argc = argc;
    program_argv = argv;
    gdk_threads_init();
    gtk_init(&argc, &argv);
    window = initWindow();

    gtk_widget_show_all(window);

    connect_database();

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();


    return 0;
}