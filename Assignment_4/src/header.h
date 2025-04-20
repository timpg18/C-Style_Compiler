
/*
old header for token table
not used, ignore this.
*/
typedef struct{
	char* token;
	char* token_type;
}token_table;

#define MAX_TOKENS 20000

token_table tokenTable[MAX_TOKENS];

/* Global variable to store the current elementary type */


void add_to_token_table(const char *token, const char *token_type) {
    if (token_count < MAX_TOKENS) {
        tokenTable[token_count].token = strdup(token);
        tokenTable[token_count].token_type = strdup(token_type);
		if (strstr(token_type, "typedef") != NULL){
			update_symtab(tokenTable[token_count].token);
		  }
        token_count++;
    }
}

void print_token_table() {
    printf("\n=================================================================\n");
    printf("                          TOKEN TABLE\n");
    printf("=================================================================\n");
    printf("| %-29s | %-29s |\n", "Token", "Token_Type");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < token_count; i++) {
        printf("| %-29s | %-29s |\n", 
               tokenTable[i].token, tokenTable[i].token_type);
    }
    printf("=================================================================\n");
}