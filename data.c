/*
 * Author: 张青龙
 * Date: 2019/8/29
 * 
 */

#include "data.h"
#include "network.h"
#include "login_gui.h"
#include "friend_linked_list.h"

void connect_database() {
    mongoc_init();
    const char *cloud_mongo_uri = "mongodb+srv://crawler995:crawler995@crawler-x83be.mongodb.net/test?retryWrites=true&w=majority";
    const char *local_mongo_uri = "mongodb://localhost:27017/?appname=linpop_database";
    client = mongoc_client_new(local_mongo_uri);
    database = mongoc_client_get_database(client, DB_NAME);
    collection = mongoc_client_get_collection (client, DB_NAME, USER_INFO_COLLECTION);
    talk_collection = mongoc_client_get_collection (client, DB_NAME, TALK_COLLECTION);
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
    BSON_APPEND_UTF8(doc, "group", "");
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
    bson_t *doc;
    char *str;

    while (mongoc_cursor_next (cursor, &doc)) {
        bson_iter_t it;
        bson_iter_init(&it, doc);
        uint32_t len;
        if(bson_iter_find(&it, "ip_address")) {
            return bson_iter_utf8(&it, &len);
        }
    } 
}

const char *get_user_group(const char *username) {
    int i, j;
    int copy = 0;
    char *res = (char*)malloc(sizeof(char) * 50);
    for(i = 0; i < strlen(username); i++) {
        if(username[i] == '|') {
            copy = 1;
            j = 0;
            continue;
        }
        if(!copy) continue;
        *(res + j) = username[i];
        j++;
    }
    *(res + j) = '\0';
    return res;
}

const char* get_correct_name(const char *friend_name) {
    int i;
    char *res = (char*)malloc(sizeof(char) * 50);
    for(i = 0; i < strlen(friend_name); i++) {
        if(friend_name[i] == '|') break;
        *(res + i) = friend_name[i];
    }
    *(res + i) = '\0';
    return res;
}

friend_node* get_user_friend_list() {
    bson_t *query = BCON_NEW("username", user_name);
    const char *opt_string = "{\"projection\":{\"friend_list\":1, \"_id\": 0}}";
    bson_t *opt = bson_new_from_json((const uint8_t*)opt_string, -1, NULL);
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, opt, NULL);
    const bson_t *doc;
    char *str;

    friend_node *head = create_friend_node(NULL, NULL, false, NULL);

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
                const char *friend_name = get_correct_name(bson_iter_utf8(&fit, &len));
                const char *friend_ip = get_user_ip_address(friend_name);
                bool friend_is_online = get_user_is_online(friend_name);
                const char *group = get_user_group(bson_iter_utf8(&fit, &len));

                append_friend_node(head, friend_name, friend_ip, friend_is_online, group);
            }
        }
    }

    bson_destroy(opt);
    bson_destroy(doc);
    bson_destroy(query);
    mongoc_cursor_destroy (cursor);

    return head;
}

bool add_user_friend_list(const char *friend_username, const char *group) {
    if(!find_user_in_database(friend_username)) {
        return false;
    }

    bson_t *doc;
    doc = bson_new();
    const bson_t child;

    BSON_APPEND_ARRAY_BEGIN(doc, "friend_list", &child);
    friend_node *head = get_user_friend_list(), *p = head->next;

    uint32_t i = 0;
    char buf[16];
    const char *key;
    size_t keylen;

    while(p) {
        keylen = bson_uint32_to_string (i, &key, buf, sizeof buf);

        char _name[50];
        strcpy(_name, p->name);
        char _group[50];
        strcpy(_group, p->group);
        strcat(_name, "|");
        strcat(_name, _group);

        bson_append_utf8(&child, key, (int) keylen, _name, -1);
        p = p->next;
        i++;
    }
    keylen = bson_uint32_to_string (i++, &key, buf, sizeof buf);
    char _name[50];
    strcpy(_name, friend_username);
    char _group[50];
    strcpy(_group, group);
    strcat(_name, "|");
    strcat(_name, _group);
    bson_append_utf8(&child, key, (int) keylen, _name, -1);

    bson_append_array_end(doc, &child);
    
    bson_t *query = BCON_NEW("username", user_name);
    bson_t *update = BCON_NEW("$set", "{", "friend_list", BCON_ARRAY(&child), "}");

    mongoc_collection_update_one(collection, query, update, NULL, NULL, NULL);

    bson_destroy(doc);
    bson_destroy(query);
    bson_destroy(update);

    delete_friend_linked_list(head);

    return true;
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
        if(bson_iter_find(&it, "is_online")) {
            bson_destroy(query);
            mongoc_cursor_destroy (cursor);
            return bson_iter_bool(&it);
        }
    }

    bson_destroy(query);
    bson_destroy(opt);
    mongoc_cursor_destroy(cursor);
}

void set_user_online(bool online) {
    bson_t *query = BCON_NEW("username", user_name);
    bson_t *update = BCON_NEW("$set", "{", "is_online", BCON_BOOL(online), "}");
    bson_error_t error;
    mongoc_collection_update_one(collection, query, update, NULL, NULL, &error);

    bson_destroy(query);
    bson_destroy(update);
}

const char* get_self_is_requested_talked() {
    bson_t *query = BCON_NEW("talk", "default");
    const char *opt_string = "{\"projection\":{\"talk_list\":1, \"_id\": 0}}";
    bson_t *opt = bson_new_from_json((const uint8_t*)opt_string, -1, NULL);
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(talk_collection, query, opt, NULL);
    const bson_t *doc;
    char *str;

    while (mongoc_cursor_next (cursor, &doc)) {
        bson_iter_t it;
        bson_iter_init(&it, doc);
        uint8_t *arr;
        if(bson_iter_find(&it, "talk_list")) {
            uint32_t len;
            const uint8_t *arr;
            bson_iter_array(&it, &len, &arr);
            bson_t *list = bson_new_from_data(arr, len);
            bson_iter_t fit;
            bson_iter_init(&fit, list);

            while(bson_iter_next(&fit)) {
                uint32_t len;
                const char *per_talk = bson_iter_utf8(&fit, &len);
                const char *is_requested_talk_user = get_user_group(per_talk);

                if(!strcmp(is_requested_talk_user, user_name)) {
                    return get_correct_name(per_talk);
                }
            }
        }
    }

    bson_destroy(opt);
    bson_destroy(doc);
    bson_destroy(query);
    mongoc_cursor_destroy (cursor);

    return NULL;
}

void add_talk_request(const char *friend_name) {
    bson_t *doc;
    doc = bson_new();
    const bson_t child;

    BSON_APPEND_ARRAY_BEGIN(doc, "talk_list", &child);

    uint32_t i = 0;
    char buf[16];
    const char *key;
    size_t keylen;

    bson_t *query = BCON_NEW("talk", "default");
    const char *opt_string = "{\"projection\":{\"talk_list\":1, \"_id\": 0}}";
    bson_t *opt = bson_new_from_json((const uint8_t*)opt_string, -1, NULL);
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(talk_collection, query, opt, NULL);
    const bson_t *doc1;
    char *str;

    while (mongoc_cursor_next (cursor, &doc1)) {
        bson_iter_t it;
        bson_iter_init(&it, doc1);
        uint8_t *arr;
        if(bson_iter_find(&it, "talk_list")) {
            uint32_t len;
            const uint8_t *arr;
            bson_iter_array(&it, &len, &arr);
            bson_t *list = bson_new_from_data(arr, len);
            bson_iter_t fit;
            bson_iter_init(&fit, list);

            while(bson_iter_next(&fit)) {
                keylen = bson_uint32_to_string (i, &key, buf, sizeof buf);
                uint32_t len;
                const char *per_talk = bson_iter_utf8(&fit, &len);

                bson_append_utf8(&child, key, (int) keylen, per_talk, -1);
                i++;
            }
            
        }
    }

    keylen = bson_uint32_to_string (i++, &key, buf, sizeof buf);
    char _name[50];
    strcpy(_name, user_name);
    char _group[50];
    strcpy(_group, friend_name);
    strcat(_name, "|");
    strcat(_name, _group);
    bson_append_utf8(&child, key, (int) keylen, _name, -1);

    bson_append_array_end(doc, &child);

    bson_t *update = BCON_NEW("$set", "{", "talk_list", BCON_ARRAY(&child), "}");

    mongoc_collection_update_one(talk_collection, query, update, NULL, NULL, NULL);

    bson_destroy(doc);
    bson_destroy(doc1);
    bson_destroy(query);
    bson_destroy(update);
}

void delete_talk_request(const char *request_user_name) {
    bson_t *doc;
    doc = bson_new();
    const bson_t child;

    char talk_record[50];
    strcpy(talk_record, request_user_name);
    strcat(talk_record, "|");
    strcat(talk_record, user_name);

    BSON_APPEND_ARRAY_BEGIN(doc, "talk_list", &child);

    uint32_t i = 0;
    char buf[16];
    const char *key;
    size_t keylen;

    bson_t *query = BCON_NEW("talk", "default");
    const char *opt_string = "{\"projection\":{\"talk_list\":1, \"_id\": 0}}";
    bson_t *opt = bson_new_from_json((const uint8_t*)opt_string, -1, NULL);
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(talk_collection, query, opt, NULL);
    const bson_t *doc1;
    char *str;

    while (mongoc_cursor_next (cursor, &doc1)) {
        bson_iter_t it;
        bson_iter_init(&it, doc1);
        uint8_t *arr;
        if(bson_iter_find(&it, "talk_list")) {
            uint32_t len;
            const uint8_t *arr;
            bson_iter_array(&it, &len, &arr);
            bson_t *list = bson_new_from_data(arr, len);
            bson_iter_t fit;
            bson_iter_init(&fit, list);

            while(bson_iter_next(&fit)) {
                keylen = bson_uint32_to_string (i, &key, buf, sizeof buf);
                uint32_t len;
                const char *per_talk = bson_iter_utf8(&fit, &len);
                if(!strcmp(per_talk, talk_record)) continue;
                bson_append_utf8(&child, key, (int) keylen, per_talk, -1);
                i++;
            }
        }
    }


    bson_append_array_end(doc, &child);

    bson_t *update = BCON_NEW("$set", "{", "talk_list", BCON_ARRAY(&child), "}");

    mongoc_collection_update_one(talk_collection, query, update, NULL, NULL, NULL);

    bson_destroy(doc);
    bson_destroy(doc1);
    bson_destroy(query);
    bson_destroy(update);
}

void destory_database_connection() {
    mongoc_collection_destroy(collection);
    mongoc_database_destroy(database);
    mongoc_client_destroy(client);
    mongoc_cleanup();
}