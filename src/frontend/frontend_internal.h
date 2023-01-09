#ifndef FRONTEND_INTERNAL
#define FRONTEND_INTERNAL

#include <tree_types.h>

typedef struct {
    char** message;
    U8* type;
    U64* start;
    U64* end;
    U64* line;
    
    
    U64 len;
    U64 capacity;
} Tokens;
Tokens tokens;

typedef struct {
    void* left;
    void* right;
    U8 type;
    U32 value;
} ASTNodeData, *ASTnode;

typedef enum {
    TOK_EOF,
    TOK_ERROR,
    TOK_INT_LITERAL,
    TOK_PLUS,
    TOK_MINUS,
    TOK_SUBTRACT,
    TOK_STAR,
    TOK_SLASH,
} TokenType;

typedef enum {
    AST_INT_LITERAL,
    AST_ADD,
    AST_SUBTRACT,
    AST_MULTIPLY,
    AST_DIVIDE,
    AST_COUNT,
} ASTtype;

#endif // FRONTEND_INTERNAL