#pragma once
#include <cstring>
#include <vector>
#include <string_view>
#include "expr_tree.h"

#define ArrayCount(a) (int)(sizeof(a) / sizeof(a[0]))

struct Tokenizer {
    char *text;
    Token token;
};


// stores relevant info about functions
struct ParseStruct {
    std::string_view parameter;
    std::string_view funcion_name;
    std::vector <std::string_view> function_parameters;
    ExpressionTree *expr_tree = NULL;
};



bool is_whitespace(char c);

inline bool is_alphabet(char c);

inline int is_num(char c);

bool is_equal_case_insensitive(std::string_view s1, const std::string_view s2);

bool is_equal_case_insensitive(const char*  s1, const char* s2, const int size_1, const int size_2);

Token tokenize(Tokenizer *t);
//parsers
bool accept_token(Tokenizer *t, TokenType kind);


bool require_token(Tokenizer *t, TokenType kind);;

bool parse_introspection_parameters(Tokenizer *t, ParseStruct *parser);

bool parse_function_parameter(Tokenizer *t, ParseStruct *parser) ;

bool parse_statement(Tokenizer *t, ParseStruct *parser);


bool parse_function(Tokenizer *t, ParseStruct *parser);


bool parse_introspection(Tokenizer *t, ParseStruct *parser);