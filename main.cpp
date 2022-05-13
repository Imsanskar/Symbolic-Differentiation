#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_view>
#include <string>

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

enum TokenType {
    Token_Identifier,
    Token_Colon,
    Token_OpenParenthesis,
    Token_CloseParenthesis,
    Token_OpenBrace,
    Token_CloseBrace,
    Token_SemiColon,
    Token_String,
    Token_Comma,
    Token_Asterik,
    Token_Number,

    Token_Add,
    Token_Subtract,
    Token_Divide,
    Token_Multiply,
    Token_Equal,

    Token_EOF,
    Token_Unknown
};


const char * TokenTypeStrings[] = {
    "Token_Identifier",
    "Token_Colon",
    "Token_OpenParenthesis",
    "Token_CloseParenthesis",
    "Token_OpenBrace",
    "Token_CloseBrace",
    "Token_SemiColon",
    "Token_String",
    "Token_Comma",
    "Token_Asterik",
    "Token_Number",

    "Token_Add",
    "Token_Subtract",
    "Token_Divide",
    "Token_Multiply",
    "Token_Equal",

    "Token_EOF",
    "Token_Unknown"
};

struct Token {
    TokenType token_type;
    int length; 
    std::string_view id; // contents of the token
    char *text;

    bool operator ==(const char* match) {
        if (strlen(match) != length) {
            return false;
        }
        for(int i = 0; i < length; i++) {
            if (id[i] != match[i]) {
                return false;
            } 
        }

        return true;
    }
};

struct Tokenizer {
    char *text;
    Token token;
};

bool is_whitespace(char c) {
    return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}

inline bool is_alphabet(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

inline int is_num(char c) {
	return (c >= '0' && c <= '9');
}

bool is_equal_case_insensitive(std::string_view s1, const std::string_view s2){
    const int size_1 = s1.size();
    const int size_2 = s2.size();

    if (size_1 != size_2){
        return false;
    } 
    for(int i = 0; i < size_1; i++){
        if(tolower(s1[i]) != tolower(s2[i])){
            return false;
        }
    }

    return true;
}

bool is_equal_case_insensitive(const char*  s1, const char* s2, const int size_1, const int size_2){
    if (size_1 != size_2){
        return false;
    } 
    for(int i = 0; i < size_1; i++){
        if(tolower(s1[i]) != tolower(s2[i])){
            return false;
        }
    }

    return true;
}

Token tokenize(Tokenizer *t) {
    Token token{};
    // token.token_type = Token_EOF;
    while (is_whitespace(t->text[0])) {
        t->text++;
    }

    if(t->text[0] == '/' && t->text[1] == '/') {
        while(t->text[0] && t->text[0] != '\n' && t->text[0] != '\r'){
            t->text++;
        }
        if (t->text[0]) t->text++;
    }

    // symbol tokenization
    switch (t->text[0]) {
        case '\0':
            token.token_type = Token_EOF;
            break;
        case '(' :
            token.token_type = Token_OpenParenthesis;
            t->text++;
            token.id = "(";
            break;
        case ')' :
            token.token_type = Token_CloseParenthesis;
            t->text++;
            token.id = ")";
            break;
        case '{' :
            token.token_type = Token_OpenBrace;
            t->text++;
            token.id = "{";
            break;
        case '}' :
            token.token_type = Token_CloseBrace;
            t->text++;
            token.id = "}";
            break;
        
        case ';' :
            token.token_type = Token_Colon;
            t->text++;
            token.id = ";";
            break;
            
        case ',' :
            token.token_type = Token_Comma;
            t->text++;
            token.id = ",";
            break;
            
        case '+' :
            token.token_type = Token_Add;
            t->text++;
            token.id = "+";
            break;
            
        case '/' :
            token.token_type = Token_Divide;
            t->text++;
            token.id = "/";
            break;
            
        case '-' :
            token.token_type = Token_Subtract;
            t->text++;
            token.id = "-";
            break;

        case '*' :
            token.token_type = Token_Asterik;
            t->text++;
            token.id = "*";
            break;
            

        case '=' :
            token.token_type = Token_Equal;
            t->text++;
            token.id = "=";
            break;
             
        case '"': 
        {
            token.token_type = Token_String;
            t->text++;
            const char *start = t->text;
            while(t->text && t->text[0] != '\"') {
                if (t->text[0] == '\\') {
                    t->text++;
                }
                t->text++;
            }
            token.id = std::string_view(start, t->text - start);
            t->text++;
            break;
        }
        default:
        {
            if(is_alphabet(t->text[0])) {
                const char *start = t->text;
                while(is_alphabet(t->text[0]) || t->text[0] == '_' || is_num(t->text[0])){
                    t->text++;
                }
                const char *end = t->text;
                token.id = std::string_view(start, end - start);
                token.length = end - start;
                token.token_type = Token_Identifier;
            } else if(is_num(t->text[0])){
                token.token_type = Token_Number;
                const char *start = t->text;
                while(is_num(t->text[0])){
                    t->text++;
                }

                // t->value = strtoull(start, (char **)&t->ptr, 10);

                // if (errno == ERANGE) {
                //     t->kind = TOKEN_ERROR;
                //     token.id = "Number is out of range";
                // }

                // t->ptr = ptr;
                // 
            } else  {
                token.token_type = Token_Unknown;
                t->text++;
            }
        }
    }

    t->token = token;
    return token;
}

//parsers
bool accept_token(Tokenizer *t, TokenType kind, TokenType *out = NULL){
    if (t->token.token_type = kind) {
        if(out) {
            *out = kind;
        }
        tokenize(t);
        return true;
    }
    return false;
}


bool require_token(Tokenizer *t, TokenType kind){
    Token token = tokenize(t);
    return token.token_type == kind;
}



bool parse_introspection_parameters(Tokenizer *t) {
    if (require_token(t, Token_OpenParenthesis)) {
        if (require_token(t, Token_Identifier)) {

        }
        if (require_token(t, Token_String)) {

        }

        if (require_token(t, Token_CloseParenthesis)) {

        }
    } else {
        return false;
    }
}


bool parse_function(Tokenizer *t) {
    
}


bool parse_introspection(Tokenizer *t) {
    if(require_token(t, Token_OpenParenthesis)) {
        parse_introspection_parameters(t);

        parse_function(t);
    }

    return true;
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
                        // parse_introspection(&t);
                    }
                    break;
                }
                case Token_Unknown:
                    continue;
                    break;

            }
            printf("{\"%s\", %.*s } \n", TokenTypeStrings[token.token_type], token.id.length(), token.id.data());
        }
    }
}