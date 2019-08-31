/*
 * Author: 张青龙
 * Date: 2019/8/29
 * 
 */

#ifndef FRIEND_LINKED_LIST_H
#define FRIEND_LINKED_LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct friend_node {
    const char *name;
    bool is_online;
    const char *ip;
    const char *group;

    struct friend_node *next;
} friend_node;

friend_node* create_friend_node(const char *friend_name, const char *friend_ip, bool friend_is_online, const char *group);
void delete_friend_linked_list(friend_node *head);
void append_friend_node(friend_node *head, const char *friend_name, const char *friend_ip, bool friend_is_online, const char *group);

#endif // FRIEND_LINKED_LIST_H