/*
 * Author: 张青龙
 * Date: 2019/8/29
 * 
 */

#include "data.h"
#include "network.h"
#include "login_gui.h"
#include "string_linked_list.h"

void connect_database() {
    mongoc_init();
    const char *cloud_mongo_uri = "mongodb+srv://crawler995:crawler995@crawler-x83be.mongodb.net/test?retryWrites=true&w=majority";
    client = mongoc_client_new("mongodb://localhost:27017/?appname=linpop_database");
    database = mongoc_client_get_database(client, DB_NAME);
    collection = mongoc_client_get_collection (client, DB_NAME, USER_INFO_COLLECTION);
}

void insert_user_info_to_collection(const char *username, const char *password) {
    bson_t *doc = bson_new();
    bson_oid_t oid;
    bson_error_t error;

    bson_oid_init(&oid, NULL);

    BSON_APPEND_OID(doc, "_id", &oid);
    BSON_APPEND_UTF8(doc, "username", username);
    BSON_APPEND_UTF8(doc, "password", password);

    bson_t child;
    BSON_APPEND_ARRAY_BEGIN(doc, "friend_list", &child);
    bson_append_array_end(doc, &child);

    BSON_APPEND_UTF8(doc, "ip_address", "");
    BSON_APPEND_BOOL(doc, "is_online", false);

    mongoc_collection_insert_one(collection, doc, NULL, NULL, &error);
    // get_self_ip_address();

    bson_destroy(doc);
}

gboolean verify_user_info_in_database(const char *username, const char *password) {
    bson_t *query = bson_new();
    BSON_APPEND_UTF8(query, "username", username);
    BSON_APPEND_UTF8(query, "password", password);

    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    char *str;
    const bson_t *doc;

    gboolean res;
    if(mongoc_cursor_next(cursor, &doc)) {
        res = TRUE;
    }
    else {
        res = FALSE;
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);

    return res;
}

gboolean find_user_in_database(const char *username) {
    bson_t *query = bson_new();
    BSON_APPEND_UTF8(query, "username", username);

    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    const bson_t *doc;

    gboolean res;
    if(mongoc_cursor_next(cursor, &doc)) {
        res = TRUE;
    }
    else {
        res = FALSE;
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);

    return res;
}

void set_user_ip_address(const char *ip) {
    bson_t *query = BCON_NEW("username", user_name);
    bson_t *update = BCON_NEW("$set", "{", "ip_address", BCON_UTF8(ip), "}");

    mongoc_collection_update_one(collection, query, update, NULL, NULL, NULL);

    bson_destroy(query);
    bson_destroy(update);
}

const char* get_user_ip_address(const char *username) {
    bson_t *query = BCON_NEW("username", username);
    const char *opt_string = "{\"projection\":{\"ip_address\":1, \"_id\": 0}}";
    bson_t *opt = bson_new_from_json((const uint8_t*)opt_string, -1, NULL);
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, opt, NULL);
    const bson_t *doc;
    char *str;

    while (mongoc_cursor_next (cursor, &doc)) {
        bson_iter_t it;
        bson_iter_init(&it, doc);
        uint8_t *arr;
        if(bson_iter_find(&it, "ip_address")) {
            bson_destroy(query);
            mongoc_cursor_destroy (cursor);
            uint32_t len;
            return bson_iter_utf8(&it, &len);
        }
    }
}

string_node* get_user_friend_list() {
    bson_t *query = BCON_NEW("username", user_name);
    const char *opt_string = "{\"projection\":{\"friend_list\":1, \"_id\": 0}}";
    bson_t *opt = bson_new_from_json((const uint8_t*)opt_string, -1, NULL);
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, opt, NULL);
    const bson_t *doc;
    char *str;

    string_node *head = create_string_node(NULL);

    while (mongoc_cursor_next (cursor, &doc)) {
        bson_iter_t it;
        bson_iter_init(&it, doc);
        uint8_t *arr;
        if(bson_iter_find(&it, "friend_list")) {
            uint32_t len;
            const uint8_t *arr;
            bson_iter_array(&it, &len, &arr);
            bson_t *list = bson_new_from_data(arr, len);
            bson_iter_t fit;
            bson_iter_init(&fit, list);

            while(bson_iter_next(&fit)) {
                uint32_t len;

                append_string_node(head, bson_iter_utf8(&fit, &len));
            }
        }
    }

    bson_destroy(query);
    mongoc_cursor_destroy (cursor);

    return head;
}

void add_user_friend_list(const char *friend_username) {
    bson_t *doc;
    doc = bson_new();
    const bson_t child;

    BSON_APPEND_ARRAY_BEGIN(doc, "friend_list", &child);
    string_node *head = get_user_friend_list(), *p = head->next;

    uint32_t i = 0;
    char buf[16];
    const char *key;
    size_t keylen;

    while(p) {
        keylen = bson_uint32_to_string (i, &key, buf, sizeof buf);
        bson_append_utf8(&child, key, (int) keylen, p->string, -1);
        p = p->next;
        i++;
    }
    keylen = bson_uint32_to_string (i++, &key, buf, sizeof buf);
    bson_append_utf8(&child, key, (int) keylen, friend_username, -1);

    bson_append_array_end(doc, &child);
    
    bson_t *query = BCON_NEW("username", user_name);
    bson_t *update = BCON_NEW("$set", "{", "friend_list", BCON_ARRAY(&child), "}");

    mongoc_collection_update_one(collection, query, update, NULL, NULL, NULL);

    bson_destroy(doc);
    bson_destroy(query);
    bson_destroy(update);

    delete_string_linked_list(head);
}

bool get_user_is_online(const char *username) {
    bson_t *query = BCON_NEW("username", username);
    const char *opt_string = "{\"projection\":{\"is_online\":1, \"_id\": 0}}";
    bson_t *opt = bson_new_from_json((const uint8_t*)opt_string, -1, NULL);
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, opt, NULL);
    const bson_t *doc;
    char *str;

    while (mongoc_cursor_next (cursor, &doc)) {
        bson_iter_t it;
        bson_iter_init(&it, doc);
        uint8_t *arr;
        if(bson_iter_find(&it, "ip_address")) {
            bson_destroy(query);
            mongoc_cursor_destroy (cursor);
            return bson_iter_bool(&it);
        }
    }
}

void set_user_online(bool online) {
    bson_t *query = BCON_NEW("username", user_name);
    bson_t *update = BCON_NEW("$set", "{", "is_online", BCON_BOOL(online), "}");
    bson_error_t error;
    mongoc_collection_update_one(collection, query, update, NULL, NULL, &error);

    bson_destroy(query);
    bson_destroy(update);
}

void destory_database_connection() {
    mongoc_collection_destroy(collection);
    mongoc_database_destroy(database);
    mongoc_client_destroy(client);
    mongoc_cleanup();
}