/*
 * Author: 张青龙
 * Date: 2019/8/29
 * 
 */

#ifndef DATA_H
#define DATA_H

#define DB_NAME "linpop_database"
#define USER_INFO_COLLECTION "user_info"

#include <mongoc/mongoc.h>
#include <gtk/gtk.h>
#include "friend_linked_list.h"

static mongoc_client_t *client;
static mongoc_database_t *database;
static mongoc_collection_t *collection;

void connect_database();

void insert_user_info_to_collection(const char *username, const char *password);
gboolean verify_user_info_in_database(const char *username, const char *password);
gboolean find_user_in_database(const char *username);

void set_user_ip_address(const char *ip);
const char* get_user_ip_address(const char *username);

const char* get_user_group(const char *username);
const char* get_correct_name(const char *friend_name);

friend_node* get_user_friend_list();
bool add_user_friend_list(const char *friend_username, const char *group);

bool get_user_is_online(const char *username);
void set_user_online(bool online);

void destory_database_connection();

#endif