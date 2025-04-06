#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>


int validate_arguments(const char* params, const char* args) {
    // Calculate the length of the fixed parameter list (up to '.' or end)
    size_t param_len = 0;
    int has_variadic = 0;
    for (; params[param_len] != '\0' && params[param_len] != '.'; param_len++);
    if (params[param_len] == '.') {
        has_variadic = 1; // Variadic parameter detected
    }

    size_t args_len = strlen(args);

    // Check if arguments are shorter than the fixed parameters (invalid)
    if (args_len < param_len) {
        return 0; // Error: arguments too short
    }

    // Compare each fixed parameter type with the corresponding argument type
    for (size_t i = 0; i < param_len; i++) {
        if (params[i] != args[i]) {
            return 0; // Type mismatch in fixed parameters
        }
    }

    // If no variadic, argument count must match exactly
    if (!has_variadic && args_len != param_len) {
        return 0; // Length mismatch for non-variadic case
    }

    return 1; // Valid
}

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

bool is_udt(const char*inp){
    if(contains(inp, "struct") == true || contains(inp,"class") == true||contains(inp,"enum") ==true || contains(inp, "union") == true ){
        return true;
    }
    return false;
}

bool isPROCEDURE(const char *input){
    return contains(input,"PROCEDURE") && (!eq(input,"PROCEDURE"));
}

// error where there must be lvalue
void lvalueError(const char* s1){
    if(contains(s1, "PROCEDURE")){
        if(contains(s1, ")*")){
            return;
            //func ptr
        }
    }
    if(contains(s1,"PROCEDURE") || contains(s1,"CONST") || contains(s1,"GOTO LABEL")){
        yyerror("lvalue required as left operand of assignment");
    }
}


#endif
