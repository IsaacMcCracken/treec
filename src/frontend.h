#ifndef FRONTEND_EXTERNAL
#define FRONTEND_EXTERNAL

typedef void* ASTnode;

void tokenize(char* source);

ASTnode binary_expression();
void ast_print(ASTnode head);
U32 ast_interpert(ASTnode root);
#endif // FRONTEND_EXTERNAL