#include "talk_record.h"
// #include "login_gui.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>  
#include <sys/stat.h>  

const char *global_login_user_name = "zql";

void append_talk_record(const char *friend_name, const char *talk_record) {
    if(access("./talk_record/", F_OK)) {
        mkdir("talk_record", S_IRWXU); 
    }  
    char file_name[50];
    sprintf(file_name, "./talk_record/%s,%s.txt", global_login_user_name, friend_name);

    FILE *file = fopen(file_name, "a");
    fwrite(talk_record, strlen(talk_record) * sizeof(char), 1, file);
    fclose(file);
}

const char* get_talk_record(const char *friend_name) {
    char file_name[50];
    sprintf(file_name, "./talk_record/%s,%s.txt", global_login_user_name, friend_name);
    
    FILE *file = fopen(file_name, "r");
    if(!file) {
        printf("empty\n");
        return "";
    }
    
    fseek(file,0,SEEK_SET);
    fseek(file,0,SEEK_END);
    int size = ftell(file) * sizeof(char);
    printf("%d\n", size);
    char *res = (char*)malloc(size);
    fseek(file,0,SEEK_SET);
    fread(res, size, 1, file);
    fclose(file);
    printf("%s\n", res);

    return res;
}