/*
 * Author: 张青龙
 * Date: 2019/8/29
 * 
 */

#ifndef DATA_H
#define DATA_H

#define APP_NAME "linpop"
#define DB_NAME "linpop_database"
#define USER_INFO_COLLECTION "user_info"

#include <mongoc/mongoc.h>
#include <gtk/gtk.h>

static mongoc_uri_t *uri;
static mongoc_client_t *client;
static mongoc_database_t *database;
static mongoc_collection_t *collection;

void connect_database();

void insert_user_info_to_collection(const char *username, const char *password);
gboolean verify_user_info_in_database(const char *username, const char *password);
gboolean find_user_in_database(const char *username);

void destory_database_connection();

#endif