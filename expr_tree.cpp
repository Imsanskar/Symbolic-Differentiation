#include "expr_tree.h"
#include <iostream>

inline int priority(TokenType token_type) {
    switch (token_type){
        case Token_Add:
            return 1;
            break;
        case Token_Subtract:
            return 0;
            break;
        case Token_Multiply:
            return 2;
            break;
        case Token_Divide:
            return 3;
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
 
    if (node->type == Token_Identifier) {
        std::cout << node->id << "\n";
    } else {
        std::cout << TokenTypeStrings[node->type] << "\n";
    }
 
    /* now recur on right child */
    print_inorder(node->right);
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
            ExpressionTree *new_tree = new ExpressionTree;
            new_tree->type = token.token_type;
            new_tree->left = tree->right;
            tree->right = new_tree;

            return tree;
        }
    }

    return NULL;
}