#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_view>
#include <string>
#include <vector>
#include <unordered_map>
#include "parser.h"

char* read_entirefile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *file_content = (char *)malloc(file_size + 2);
        fread(file_content, file_size, 1, file);
        file_content[file_size] = '\0';

        return file_content;
    }

    return NULL;
}





int main() {    
    char *content = read_entirefile("test.c");

    if(content != NULL){
        Token token;
        Tokenizer t;
        t.text = content;
        bool parsing = true;
        while(parsing) {
            token = tokenize(&t);
            switch(token.token_type){
                case Token_EOF: 
                    parsing = false;
                    break;
                
                case Token_Identifier:
                {
                    if (token == "derivative") {
                        ParseStruct parser;
                        tokenize(&t);
                        parse_introspection(&t, &parser);
                    }
                }
                case Token_Unknown:
                    continue;
                    break;

            }
            // printf("{\"%s\", %.*s } \n", TokenTypeStrings[token.token_type], token.id.length(), token.id.data());
        }
    }
}