/*
 * Author: 张青龙
 * Date: 2019/8/28
 * 
 */

#ifndef LOGIN_GUI_H
#define LOGIN_GUI_H

#include <gtk/gtk.h>

#define USERNAME "crawler"
#define PASSWORD "1234"

static int program_argc;
static char **program_argv;

static GtkWidget *layout;
static GtkWidget *login_label;
static GtkWidget *username_label;
static GtkWidget *username_input;
static GtkWidget *password_label;
static GtkWidget *password_input;
static GtkWidget *login_button;
static GtkWidget *register_button;

static void login_handle(GtkWidget *button, gpointer data);
static void register_handle(GtkWidget *button, gpointer data);
static GtkWidget* initWindow();


#endif