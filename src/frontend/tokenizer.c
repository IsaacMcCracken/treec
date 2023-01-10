
#include <tree_types.h>
#include <tree_memory.h>

#define TOKEN_DEBUG

#ifdef TOKEN_DEBUG
    #include <stdio.h>
    #include <time.h>
#endif
#include "frontend_internal.h"

//Global Tokens
Tokens tokens;
const char* token_names[] = {
    "EOF",
    "ERROR",
    "INT LITERAL",
    "PLUS",
    "MINUS",
    "STAR",
    "SLASH",
};


typedef struct {
    U32 start;
    U32 current;
    U32 line;
} Scanner;
Scanner scanner;

void scanner_init() {
    scanner.start   = 0;
    scanner.current = 0;
    scanner.line = 1;
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static char advance() {
    return tokens.source[scanner.current++];
}

static char peek() {
    return tokens.source[scanner.current];
}

static char peek_next() {
    if (tokens.source[scanner.current] != '\0')
        return tokens.source[scanner.current + 1];
    else return '\0';
}

static void skip_whitespace() {
    while (true)
    {
        switch (peek())
        {
        case ' ':
        case '\t':
        case '\r':
        case '\v': advance(); break;
        case '\n': advance(); scanner.line++; break;
        case '/':
            if (peek_next() == '/') {
                while (peek() != '\n')
                    advance();
                scanner.line++;
            } break;
        default: return;
        }
    }
    
}

void tokens_write(U8 type, const char* message) {
    if (tokens.capacity < tokens.len + 1) {
        U32 old_capacity = tokens.capacity;
        tokens.capacity = GROW_CAPACITY(old_capacity);
        tokens.type = GROW_ARRAY(U8, tokens.type, tokens.capacity);
        tokens.start = GROW_ARRAY(U32, tokens.start, tokens.capacity);
        tokens.length  = GROW_ARRAY(U16, tokens.length, tokens.capacity);
        tokens.line = GROW_ARRAY(U32, tokens.line, tokens.capacity);
        tokens.message = GROW_ARRAY(const char*, tokens.message, tokens.capacity);
    }

    tokens.type[tokens.len]     = type;
    tokens.start[tokens.len]    = scanner.start;
    tokens.length[tokens.len]   = (U16)(scanner.current - scanner.start);
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

static void token_number() {
    while (is_digit(peek()))
        advance();
    // if (peek() == '.') {
    //     advance();
    //     while (is_digit(peek()))
    //         advance();
    // }  

    token_create(TOK_INT_LITERAL);
}

void tokenize(char* source) {
    #ifdef TOKEN_DEBUG
        U32 time_start = (U32)clock();
    #endif
    scanner_init();
    tokens.source = source;
    while (peek() != 0)
    {
        skip_whitespace();
        scanner.start = scanner.current;
        
        char c = advance();
        if (is_digit(c)) { token_number(); continue; }

        switch (c) {
            case '+': token_create(TOK_PLUS); continue;
            case '-': token_create(TOK_MINUS); continue;
            case '*': token_create(TOK_STAR);  continue;
            case '/': token_create(TOK_SLASH); continue;

            case '\0': token_create(TOK_EOF); continue;
            default:
                // fprintf(stderr, "We do not recognize the char %c.\n", peek());
                continue;
        }
    }

    #ifdef TOKEN_DEBUG
    U32 time_stop = (U32)clock();
    printf("\n== Token Count: %03lu ==\n", tokens.len);
    for (U32 i = 0; i < tokens.len; i++)
    {
        printf("[%03lu] Type: %s Start: %03lu length: %03u Line: %03lu.\n", i, token_names[tokens.type[i]], 
                tokens.start[i], tokens.length[i], tokens.line[i]);
    }
    printf("Tokenized %lu lines of code in %lu milliseconds.\n", scanner.line, time_stop - time_start);
    #endif

}