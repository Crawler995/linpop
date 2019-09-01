#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <gtk/gtk.h>

static GtkWidget *window;
static GtkTextBuffer *buffer_input;
static GtkTextBuffer *buffer_output;
static GtkWidget *fixed;
static GtkWidget *create_window;
static gchar *text;
static gchar user_id[50];

static bool _is_server;
static const char *_friend_name;
static const char *_friend_ip;
static bool continue_recv_message;

static int listenfd ;
static int sockfd;
static int connfd;

static const char *exit_message;
static void stop_connection();

static GtkWidget* create_frame_chat_window(gchar*user_id);

static void *recv_message_client(void *fd);
static void server_chat_frame();
static void server_send();

static void *recv_message_client(void *fd);
static void client_chat_frame();
static void client_send();

void create_chat_window(const char *self_name, const char *friend_name, const char *friend_ip, bool is_server);

static void append_self_message_record_frame(const gchar *message);
static void append_friend_message_record_frame(const char *friend_name, const char *message);

static void button_ok_close(GtkWidget *widget,gpointer*data);
static void button_event_send(GtkWidget *wiget,GtkWidget *user_id);
static void button_event_cancel(GtkWidget *widget,gpointer*data);
static void text_input();
static void text_output();
static void button_send_cancel(gchar *user_id);
static GtkWidget * create_imagetext_hbox(const char *image_path);
static GtkWidget * create_image_button(const char *image_path);
static void right_label();
static void top_label(gchar*user_id);

#endif