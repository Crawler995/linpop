/*
 * Author: 张青龙
 * Date: 2019/8/29
 * 
 */

#ifndef STRING_LINKED_LIST_H
#define STRING_LINKED_LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct string_node {
    char *string;
    int len;
    struct string_node *next;
} string_node;

string_node* create_string_node(char *string);
void delete_string_linked_list(string_node *head);
void append_string_node(string_node *head, char *string);

#endif // STRING_LINKED_LIST_H