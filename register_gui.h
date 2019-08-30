/*
 * Author: 张青龙
 * Date: 2019/8/28
 * 
 */

#ifndef REGISTER_GUI_H
#define REGISTER_GUI_H

#include <gtk/gtk.h>

static GtkWidget *layout;
static GtkWidget *register_label;
static GtkWidget *username_label;
static GtkWidget *username_input;
static GtkWidget *password_label;
static GtkWidget *password_input;
static GtkWidget *again_password_label;
static GtkWidget *again_password_input;
static GtkWidget *register_button;

static gboolean password_security_pass(const gchar *password);
static void register_handle(GtkWidget *button, gpointer data);
static GtkWidget* initWindow();
void create_register_window(int argc, char **argv);

#endif