#pragma once
#include "file.h"

int dataWidth(const char* filename)
{
    FILE* f=fopen(filename, "r");
    if(f==NULL){
        printf("There is no file to be opened.\n");
        return 0;
    }
    char* line=NULL;
    size_t dur=0;
    ssize_t len;
    int count=0;
    while( (len=getline(&line, &dur, f))!=-1 ){
        char *str, *token;
        str=strdup(line);
        while( (token=strsep(&str, ",")) ){
            if(strcmp(token, "")==0) continue;
            count++;
            if ( token[strlen(token)-1]=='\n' )
            {
                return count;
            }
        }
    }
    return count;
}

int dataDepth(const char* filename)
{
    FILE* f=fopen(filename, "r");
    if(f==NULL){
        printf("There is no file to be opened.\n");
        return 0;
    }
    size_t dur=0;
    char* line=NULL;
    ssize_t len;
    int count=0;
    while( (len=getline(&line, &dur, f))!=-1 ){
        ++count;
    }
    return count-1; //skip the first row
}