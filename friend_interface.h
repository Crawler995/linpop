#ifndef FRIEND_INTERFACE_H
#define FRIEND_INTERFACE_H

#include<gtk/gtk.h>
#include<stdlib.h>
#include "login_gui.h"
#include <stdbool.h>

GtkTextBuffer *buffer_input_friend1;
GtkTextBuffer *buffer_input_friend2;
GtkTextBuffer *buffer_input_tree;
GtkTreeViewColumn *col;
GtkCellRenderer *renderer;

GtkTreeModel *model;
GtkTreeStore *treestore;
GtkTreeIter toplevel;
static GtkWidget  *window;
static GtkWidget *view;
static GtkWidget *vbox;
static GtkTreeSelection *selection;
static GtkWidget *statusbar;
static GtkWidget *head_button;
static GtkWidget *button_head;
GtkTreeStore *treestore;
GtkTreeIter child;

GtkTreeIter  toplevel_set[10];
static int tree=0;
static char group[20][50];
static bool listening = true;

enum
{
    COLUMN = 0,
    NUM_COLS
} ;

void create_friend_interface(int argc ,char **argv);

#endif