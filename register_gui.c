/*
 * Author: 张青龙
 * Date: 2019/8/28
 * 
 */

#include "register_gui.h"

static void register_handle(GtkWidget *button, gpointer data) {
    const gchar *username_text = gtk_entry_get_text(GTK_ENTRY(username_input));
    const gchar *password_text = gtk_entry_get_text(GTK_ENTRY(password_input));
    const gchar *again_password_text = gtk_entry_get_text(GTK_ENTRY(again_password_input));

    g_print("账号: %s 密码: %s\n", username_text, password_text);

    GdkColor green = {0, 0, 0xffff, 0}, red = {0, 0xffff, 0, 0};
    gboolean register_success = !strcmp(password_text, again_password_text);

    gtk_label_set_text(GTK_LABEL(register_label), register_success ? "注册成功！" : "注册失败，请重试！");
    gtk_widget_modify_fg(register_label, GTK_STATE_NORMAL, register_success ? &green : &red);
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