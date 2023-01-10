#include <stdio.h>
#include <stdlib.h>
#include "frontend_internal.h"

#define PARSER_DEBUG


U32 current_token = 0;

U8 arithemetic_op() {
    switch (tokens.type[current_token]) {
    case TOK_PLUS:      return AST_ADD;
    case TOK_MINUS:     return AST_SUBTRACT;
    case TOK_STAR:      return AST_MULTIPLY;
    case TOK_SLASH:     return AST_DIVIDE;
    
    default:
        fprintf(stderr, "Unknown token of type: %u, in aritmetic_op() on line %lu\n",
            tokens.type[current_token], tokens.line[current_token]);
        exit(1);
        return 255;
    }
}

U32 parse_slice_to_int(U32 id) {
    U32 result = 0;
    for (U32 i = tokens.start[id]; i < tokens.start[id] + tokens.length[id]; i++)
        result = result*10 + (U32)(tokens.source[i]-'0');
    return result;
}

ASTnode astnode_create(U8 type, U32 value, ASTnode left, ASTnode right) {
    ASTnode result = (ASTnode)malloc(sizeof(ASTNodeData));

    result->type = type;
    result->value = value;
    result->left = left;
    result->right = right;

    return result;
}

ASTnode astnode_create_leaf(U8 type, U32 value) {
    return astnode_create(type, value, NULL, NULL);
}

ASTnode astnode_create_unary(U8 type, U32 value, ASTnode left) {
    return astnode_create(type, value, left, NULL);

}


static ASTnode astnode_primary_factor() {
    ASTnode result;
    switch (tokens.type[current_token]) {
    case TOK_INT_LITERAL:
        result = astnode_create_leaf(AST_INT_LITERAL, parse_slice_to_int(current_token));
        return result;
    default:
        fprintf(stderr, "Syntax error on line: %lu", tokens.line[current_token]);
        exit(1);
        return NULL;
    }
}

ASTnode binary_expression() {
    ASTnode result = NULL, left = NULL, right = NULL;
    U8 node_type = 255;
    // printf("We are on token %lu...\n", current_token);
    left = astnode_primary_factor();
    current_token++;

    if (tokens.type[current_token] == TOK_EOF)
        return left;
    
    node_type = arithemetic_op();
    current_token++;

    right = binary_expression();

    result = astnode_create(node_type, 0, left, right);
    return result;
}

U32 ast_interpret(ASTnode root) {
    U32 leftval, rightval;

    if (root->left)
        leftval = ast_interpret(root->left);
    if (root->right)
        rightval = ast_interpret(root->right);
    
    switch (root->type)
    {
    case AST_ADD: return leftval + rightval;
    case AST_SUBTRACT: return leftval - rightval;
    case AST_MULTIPLY: return leftval * rightval;
    case AST_DIVIDE: return leftval / rightval;

    case AST_INT_LITERAL: return root->value;
    default:
        fprintf(stderr, "Unknown AST operator %d.\n", root->type);
        exit(1);
    }
}

void ast_print(ASTnode root, U16 depth) {
    if (root == NULL)
        return;
    putchar('|');
    for (U16 i = 0; i < depth*2; i++)
        putchar('_');

    printf("Data: %lu\n", root->value);
    if (root->left != NULL)
        ast_print(root->left, depth + 1);
    if (root->right != NULL)
        ast_print(root->right, depth + 1);
}
