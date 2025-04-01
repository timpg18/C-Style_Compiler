#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool eq(const char *s1, const char *s2) {
    // If either pointer is NULL, check if both are NULL.
    if (s1 == NULL || s2 == NULL) {
        return s1 == s2;
    }
    return strcmp(s1, s2) == 0;
}
char *concat(const char *s1, const char *s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char *result = (char*)malloc(len1 + len2 + 3);  // +1 for the null terminator
    if (result == NULL) {
        // Handle memory allocation error appropriately
        return NULL;
    }
    strcpy(result, s1);
    strcat(result, " ");
    strcat(result, s2);
    return result;
}

void check_type(const char* s1, const char* s2,  char* s3){
    if(eq(s1 , s2) == false){
        char *err = s3;
        err = concat(err,s1);
        err = concat(err, s2);
        yyerror(err);
    }
}

bool contains(const char *str, const char *substr) {
    if (str == NULL || substr == NULL) {
        return false;
    }
    return (strstr(str, substr) != NULL);
}

char* extract_between_parentheses(const char* str) {
    if (!str) return NULL;  
    const char* start = strchr(str, '(');
    const char* end = strrchr(str, ')');
    if (!start || !end || start >= end - 1) {
        return NULL; 
    }
    end--;
    start++;
    start++;
    int len = end - start;
    if(len<=0)return "";
    char* result = (char*)malloc(len + 1);
    if (!result) return NULL;  
    strncpy(result, start, len);
    result[len] = '\0'; 

    return result;
}

bool is_first_arg_STRING(const char *input) {
    if (input == NULL) {
        return false;
    }
    
    return strncmp(input, "CHAR*", 5) == 0;
}

#endif
