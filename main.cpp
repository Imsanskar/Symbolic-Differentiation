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
        fclose(file);
        return file_content;
    }

    return NULL;
}


//TODO: Calculate derivative(maybe recursively)
std::string_view calculate_derivative(ExpressionTree *tree) {

}


void write_derivative(const char *filename, const ParseStruct& parse_tree) {
    FILE *file = fopen(filename, "a");
    if (file) {
        fprintf(
            file,
            "\nfloat _deriv_%.*s (", parse_tree.funcion_name.length(), parse_tree.funcion_name.data()
        );


        // add funtion parameter to function method
        const int size_function_parameter = parse_tree.function_parameters.size();
        for(int i = 0; i < size_function_parameter; i++) {
            const std::string_view &parameter = parse_tree.function_parameters[i];
            fprintf(file, "float %.*s", parameter.length(), parameter.data());

            if (i != size_function_parameter - 1) {
                fprintf(file, ", ");
            } else {
                fprintf(file, ") {\n");
            }
        }

        fprintf(file, "\n}");

        fclose(file);
    }
}

int main() {    
    const char *filename = "test.c";
    char *content = read_entirefile(filename);

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

                        write_derivative(filename, parser);
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