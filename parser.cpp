#include "parser.h"
#include <string>

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
            // token.id = "(";
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
        
        case ':':
            token.token_type = Token_Colon;
            t->text++;
            token.id = ":";
            break;
        
        case ';' :
            token.token_type = Token_SemiColon;
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
            token.token_type = Token_Multiply;
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
                const size_t size = ArrayCount(keywords);
                for(int i = 0; i < size; i++) {
                    if (is_equal_case_insensitive(token.id, keywords[i])) {
                        token.token_type = (TokenType) i;
                    }
                }
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
bool accept_token(Tokenizer *t, TokenType kind){
    if (t->token.token_type == kind) {
        tokenize(t);
        return true;
    }
    return false;
}


bool require_token(Tokenizer *t, TokenType kind){
    bool result = t->token.token_type == kind;
    tokenize(t);
    return result;
}



bool parse_introspection_parameters(Tokenizer *t, ParseStruct *parser) {
    if (require_token(t, Token_Variable)) {
        if(!require_token(t, Token_Colon)) {
            //TODO:Error checking
            return false;
        }
        Token token = t->token;
        if (require_token(t, Token_String)) {
            parser->parameter = token.id;
        } else {
            return false;
        }

        if (!require_token(t, Token_CloseParenthesis)) {
            //TODO:Error checking
            return false;
        }

    }

    return true;
}


bool parse_function_parameter(Tokenizer *t, ParseStruct *parser) {
    if (!accept_token(t, Token_Float)) {
        if (!require_token(t, Token_Double)) {
            return false;
        }
    }
    Token token = t->token;
    if (require_token(t, Token_Identifier)) {
        parser->function_parameters.push_back(token.id);
        return true;
    }

    return false;
}


bool parse_statement(Tokenizer *t, ParseStruct *parser) {
    Token token;
    if (require_token(t, Token_Return)) {
        // TODO: parse mathematical expression
        token = t->token;
        if (!require_token(t, Token_Identifier)) {
            
        }
        parser->expr_tree = insert_expr(parser->expr_tree, token);
        bool is_remaining_expression  = true;


        while(is_remaining_expression) {
            token = t->token;
            if (!accept_token(t, Token_Add) && !accept_token(t, Token_Subtract) && !accept_token(t, Token_Multiply) && !accept_token(t, Token_Divide)) {
                return false;
            }
            parser->expr_tree = insert_expr(parser->expr_tree, token);

            token = t->token;
            if (!require_token(t, Token_Identifier)) {
                return false;
            }
            parser->expr_tree = insert_expr(parser->expr_tree, token);

            if (accept_token(t, Token_SemiColon)) {
                is_remaining_expression = false;
                return true;
            }
        }
    }

    return false;
}


bool parse_function(Tokenizer *t, ParseStruct *parser) {
    //FIXME: proper checking and maybe store types too
    accept_token(t, Token_Double);
    accept_token(t, Token_Float);
    Token token = t->token;
    if(require_token(t, Token_Identifier)) {
        parser->funcion_name = token.id;
    }
    
    // consume open parenthesis
    tokenize(t);

    // parse function parameters
    bool function_parameter_remaining = true;
    while (function_parameter_remaining) {
        parse_function_parameter(t, parser);

        // if next token is Token_ClosseParenthesis then there are no more function parameters
        if (accept_token(t, Token_CloseParenthesis)) {
            function_parameter_remaining = false;
            continue;
        } else if (!require_token(t, Token_Comma)) {
            return false;
        }
    }

    if (!require_token(t, Token_OpenBrace)) {
        //TODO: insert error checkint
        return false;
    }

    bool statements_remaining = true;

    while (parse_statement(t, parser)) {
        
    }



    if (!require_token(t, Token_CloseBrace)) {
        //TODO: insert error checking here
        return false;
    }
}


bool parse_introspection(Tokenizer *t, ParseStruct *parser) {
    if(require_token(t, Token_OpenParenthesis)) {
        parse_introspection_parameters(t, parser);
        
        parse_function(t, parser);
    }

    return true;
}
