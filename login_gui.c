/*
 * Author: 张青龙
 * Date: 2019/8/27
 * 
 */

#include <gtk/gtk.h>

#define USERNAME "crawler"
#define PASSWORD "1234"

static GtkWidget *layout;
static GtkWidget *login_label;
static GtkWidget *username_label;
static GtkWidget *username_input;
static GtkWidget *password_label;
static GtkWidget *password_input;
static GtkWidget *login_button;
static GtkWidget *register_button;

static void login_handle(GtkWidget *button, gpointer data) {
    const gchar *username_text = gtk_entry_get_text(GTK_ENTRY(username_input));
    const gchar *password_text = gtk_entry_get_text(GTK_ENTRY(password_input));

    g_print("账号: %s\n密码: %s\n", username_text, password_text);

    gboolean login_success = !strcmp(username_text, USERNAME) && !strcmp(password_text, PASSWORD);

    const gchar *login_status_tip = login_success ? "登录成功！" : "登录失败，请重试！";
    GdkColor green = {0, 0, 0xffff, 0}, red = {0, 0xffff, 0, 0};
    GdkColor color = login_success ? green : red;

    g_print("%s\n", login_status_tip);
    
    gtk_label_set_text(GTK_LABEL(login_label), login_status_tip);
    gtk_widget_modify_fg(login_label, GTK_STATE_NORMAL, &color);
}

static void register_handle(GtkWidget *button, gpointer data) {
    g_print("注册\n");
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
    gtk_init(&argc, &argv);
    GtkWidget *window = initWindow();

    gtk_widget_show_all(window);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
    return 0;
}