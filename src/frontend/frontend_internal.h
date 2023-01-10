#ifndef FRONTEND_INTERNAL
#define FRONTEND_INTERNAL

#include <tree_types.h>

typedef struct {
    const char** message;
    U8* type;
    U32* start;
    U16* length;
    U32* line;
    
    U32 len;
    U32 capacity;

    char* source;
} Tokens;
extern Tokens tokens;

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
    TOK_STAR,
    TOK_SLASH,
} TokenType;

extern const char* token_names[];
typedef enum {
    AST_INT_LITERAL,
    AST_ADD,
    AST_SUBTRACT,
    AST_MULTIPLY,
    AST_DIVIDE,
    AST_COUNT,
} ASTtype;

#endif // FRONTEND_INTERNAL