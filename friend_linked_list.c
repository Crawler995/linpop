/*
 * Author: 张青龙
 * Date: 2019/8/29
 * 
 */

#include "friend_linked_list.h"
#include "stdbool.h"

friend_node* create_friend_node(const char *friend_name, const char *friend_ip, bool friend_is_online) {
    friend_node *node = (friend_node*)malloc(sizeof(friend_node));
    node->name = friend_name;
    node->ip = friend_ip;
    node->is_online = friend_is_online;

    node->next = NULL;

    return node;
}

void delete_friend_linked_list(friend_node *head) {
    friend_node *p = head, *q = head;
    if(!p->next) {
        free(p);
        return;
    }

    while(1) {
        if(!p->next) {
            free(p);
            return;
        }
        q = p->next;
        free(p);
        p = q;
    }
}

void append_friend_node(friend_node *head, const char *friend_name, const char *friend_ip, bool friend_is_online) {
    while(head->next) {
        head = head->next;
    }

    head->next = create_friend_node(friend_name, friend_ip, friend_is_online);
}