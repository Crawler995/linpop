#include "string_linked_list.h"

string_node* create_string_node(char *string) {
    string_node *node = (string_node*)malloc(sizeof(string_node));
    node->string = string;
    node->len = 0;

    return node;
}

void delete_string_linked_list(string_node *head) {
    string_node *p = head, *q = head;
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

void append_string_node(string_node *head, char *string) {
    while(head->next) {
        head = head->next;
    }

    head->next = create_string_node(string);
}