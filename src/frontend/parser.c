#include <stdio.h>
#include <stdlib.h>
#include "frontend_internal.h"

#define PARSER_DEBUG





U32 current_token = 0;

U8 arithemetic_op(U32 id) {
    switch (tokens.type[id]) {
    case TOK_PLUS:      return AST_ADD;
    case TOK_MINUS:     return AST_SUBTRACT;
    case TOK_STAR:      return AST_MULTIPLY;
    case TOK_SLASH:     return AST_DIVIDE;
    
    default:
        fprintf(stderr, "Unknown token in aritmetic_op() on line %lu", tokens.line[id]);
        exit(1);
        return 255;
    }
}

U32 parse_slice_to_int(U32 id) {
    U32 result = 0;
    for (U32 i = tokens.start[id]; i < tokens.start[id] + tokens.length[id]; i++)
        result = result*10 + (U32)(tokens.source[i]-'0');

    #ifdef PARSER_DEBUG
        printf("Int slice result: %lu\n", result);
    #endif

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
        current_token++;
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

    left = astnode_primary_factor();

    if (tokens.type[current_token] == TOK_EOF)
        return left;
    
    node_type = arithemetic_op(tokens.type[current_token]);
    current_token++;

    right = binary_expression();

    result = astnode_create(node_type, 0, left, right);
}


U32 ast_interpert(ASTnode root) {
    U32 leftval, rightval;

    if (root->left)
        leftval = ast_interpert(root->left);
    if (root->right)
        rightval = ast_interpert(root->right);
    
    switch (root->type)
    {
    case AST_ADD: return leftval + rightval;
    case AST_SUBTRACT: return leftval - rightval;
    case AST_MULTIPLY: return leftval * rightval;
    case AST_DIVIDE: return leftval / rightval;
    default:
        fprintf(stderr, "Unknown AST operator %d.\n", root->type);
        exit(1);
    }
}
