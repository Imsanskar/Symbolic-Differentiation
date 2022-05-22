#pragma once
#include <cstring>
#include <vector>
#include <string_view>

extern "C" {
enum TokenType {
    Token_Float,
    Token_Int,
    Token_Double,
    Token_Return,
    Token_Variable,
    Token_Sin,
    Token_Cos,
    Token_Tan,
    Token_Pow,
    Token_Exp,
    Token_Identifier,
    Token_Colon,
    Token_OpenParenthesis,
    Token_CloseParenthesis,
    Token_OpenBrace,
    Token_CloseBrace,
    Token_SemiColon,
    Token_String,
    Token_Comma,
    Token_Number,

    Token_Add,
    Token_Subtract,
    Token_Divide,
    Token_Multiply,
    Token_Equal,

    Token_EOF,
    Token_Unknown
};

const std::string_view keywords[] = {
    "float", "int", "double", "return", "variable", "sin", "cos", "tan", "pow", "exp"
};


static const char * TokenTypeStrings[] = {
    "Token_Float",
    "Token_Int",
    "Token_Double",
    "Token_Return",
    "Token_Variable",
    "Token_Sin",
    "Token_Cos",
    "Token_Tan",
    "Token_Pow",
    "Token_Exp",
    "Token_Identifier",
    "Token_Colon",
    "Token_OpenParenthesis",
    "Token_CloseParenthesis",
    "Token_OpenBrace",
    "Token_CloseBrace",
    "Token_SemiColon",
    "Token_String",
    "Token_Comma",
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
}
