#pragma once
#include <string_view>
#include "token.h"

struct ExpressionTree {
    TokenType type = Token_Unknown;
    std::string_view id = "";
    ExpressionTree *left = NULL;
    ExpressionTree *right = NULL;

    ExpressionTree() = default;
};

ExpressionTree* insert_expr(ExpressionTree *tree, Token token);

void print_inorder(ExpressionTree *node);


std::string_view expressiontree_to_string(ExpressionTree *tree);