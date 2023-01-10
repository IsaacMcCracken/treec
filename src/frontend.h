#ifndef FRONTEND_EXTERNAL
#define FRONTEND_EXTERNAL

typedef void* ASTnode;

void tokenize(char* source);

ASTnode binary_expression();
void ast_print(ASTnode root);
U32 ast_interpret(ASTnode root, U16 depth);
#endif // FRONTEND_EXTERNAL