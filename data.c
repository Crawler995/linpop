/*
 * Author: 张青龙
 * Date: 2019/8/29
 * 
 */

#include "data.h"

void connect_database() {
    const char *uri_string = "mongodb://localhost:27017/?appname=linpop";

    mongoc_init();

    uri = mongoc_uri_new(uri_string);
    client = mongoc_client_new_from_uri(uri);
    database = mongoc_client_get_database(client, DB_NAME);
    collection =  mongoc_client_get_collection (client, DB_NAME, USER_INFO_COLLECTION);
}

void insert_user_info_to_collection(const char *username, const char *password) {
    bson_t *doc = bson_new();
    bson_oid_t oid;
    bson_error_t error;

    bson_oid_init(&oid, NULL);

    BSON_APPEND_OID(doc, "_id", &oid);
    BSON_APPEND_UTF8(doc, "username", username);
    BSON_APPEND_UTF8(doc, "password", password);

    mongoc_collection_insert_one(collection, doc, NULL, NULL, &error);

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

void destory_database_connection() {
    mongoc_collection_destroy(collection);
    mongoc_database_destroy(database);
    mongoc_uri_destroy(uri);
    mongoc_client_destroy(client);
    mongoc_cleanup();
}