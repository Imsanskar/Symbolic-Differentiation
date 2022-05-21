#include "expr_tree.h"
#include <iostream>
#include <string.h>
#include <stdio.h>

inline int priority(TokenType token_type) {
    switch (token_type){
        case Token_Add:
            return 0;
            break;
        case Token_Subtract:
            return 0;
            break;
        case Token_Multiply:
            return 1;
            break;
        case Token_Divide:
            return 1;
            break;
        default:
            return 4;
    }
}

void print_inorder(ExpressionTree *node) {
    if (node == NULL)
        return;
 
    /* first recur on left child */
    print_inorder(node->left);
 
    if (node->type == Token_Identifier || node->type == Token_Number) {
        std::cout << node->id << "\n";
    } else {
        std::cout << TokenTypeStrings[node->type] << "\n";
    }
 
    /* now recur on right child */
    print_inorder(node->right);
}


std::string_view expressiontree_to_string(ExpressionTree *tree) {
    if (!tree) {
        return "";
    }

    if (tree->type == Token_Identifier || tree->type == Token_Number) {
        return tree->id;
    }

    std::string_view operator_type;
    switch (tree->type) {
        case Token_Add:
            operator_type = "+";
            break;
        case Token_Subtract:
            operator_type = "-";
            break;
        case Token_Multiply:
            operator_type = "*";
            break;
        case Token_Divide:
            operator_type = "/";
        break;
    }

    std::string_view left = expressiontree_to_string(tree->left);
    std::string_view right = expressiontree_to_string(tree->right);
    char *result = (char *)malloc(left.length() + operator_type.length() + right.length() + 8);
    sprintf(result, "(%.*s %.*s (%.*s))", left.length(), left.data(), operator_type.length(), operator_type.data(), right.length(), right.data());

    return std::string_view(result);
}



ExpressionTree* insert_expr(ExpressionTree *tree, Token token) {
    if (token.token_type == Token_Identifier) {
        if(!tree) {
            tree = new ExpressionTree;
            tree->left = new ExpressionTree;
            tree->left->id = token.id;
            tree->left->type = token.token_type;

            return tree;
        }else if (tree && !tree->right && tree->left) {
            tree->right = new ExpressionTree;
            tree->right->id = token.id;
            tree->right->type = token.token_type;

            return tree;
        } else  {
            tree->right = insert_expr(tree->right, token);

            return tree;
        }

    }


    if (token.token_type != Token_Identifier) {
        if (tree->left && !tree->right){
            tree->type = token.token_type;

            return tree;
        }

        if (priority(tree->type) >= priority(token.token_type)) {
            ExpressionTree *new_tree = new ExpressionTree;
            new_tree->type = token.token_type;
            new_tree->left = tree;

            return new_tree;
        } else {
            if (priority(token.token_type) < priority(tree->type)) {
                tree->left = tree;
                tree->right = NULL;
            } else {
                tree->right = insert_expr(tree->right, token);
            }

            return tree;
        }
    }

    return NULL;
}