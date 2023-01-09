#include <tree_types.h>
#include <tree_memory.h>


#include "frontend_interal.h"


typedef struct {
    char* source;
    char* start;
    char* current;
    U64 line;
} Scanner;
Scanner scanner;

void scanner_init(char* source) {
    scanner.source  = source;
    scanner.start   = source;
    scanner.current = source;
    scanner.line = 1;
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static char advance() {
    return *scanner.current++;
}

static char peek() {
    return *scanner.current;
}

static char peek_next() {
    if (*scanner.current != '\0')
        return scanner.current[1];
    else return '\0';
}

void tokens_write(U8 type, const char* message) {
    if (tokens.capacity < tokens.len + 1) {
        U64 old_capacity = tokens.capacity;
        tokens.capacity = GROW_CAPACITY(old_capacity);
        tokens.type = GROW_ARRAY(U8, tokens.type, tokens.capacity);
        tokens.start = GROW_ARRAY(U64, tokens.start, tokens.capacity);
        tokens.end  = GROW_ARRAY(U64, tokens.end, tokens.capacity);
        tokens.line = GROW_ARRAY(U64, tokens.line, tokens.capacity);
        tokens.message = GROW_ARRAY(char*, tokens.message, tokens.capacity);
    }

    tokens.type[tokens.len]     = type;
    tokens.start[tokens.len]    = (U64)(scanner.source - scanner.start);
    tokens.end[tokens.len]      = (U64)(scanner.source - scanner.current);
    tokens.line[tokens.len]     = scanner.line;
    tokens.message[tokens.len]  = message;
    tokens.len += 1;
}

static void token_create(U8 type) {
    tokens_write(type, NULL);
}

static void token_error(const char* message) {
    tokens_write(TOK_ERROR, message);
}

static void number() {
    while (is_digit(peek()))
        advance();
    if (peek() == '.') {
        advance();
        while (is_digit(peek()))
            advance();
    }  

    token_create(TOK_INT_LITERAL)
}

void tokenize(char* source) {
    scanner_init(source);
    while (*scanner.start != '\0')
    {
        
    }
    
}