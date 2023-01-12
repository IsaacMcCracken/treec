
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

void tokens_print() {
    for (U32 i = 0; i < tokens.len; i++)
    {
        printf("Type: %-10s | ", token_names[tokens.type[i]]);
        if (i%2==0 && i!=0) {
            putchar('\n');
        }
    }
    
}


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
        case ' ':  advance(); break;
        case '\t': advance(); break;
        case '\r': advance(); break;
        case '\v': advance(); break;
        case '\n': advance(); scanner.line++; break;
        case '/':
            if (peek_next() == '/') {
                while (peek() != '\n' && peek_next() != '\0')
                    advance();
                advance();
                scanner.line++;
            } else return;
            break;
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
    scanner_init();
    tokens.source = source;
    for (;;)
    {
        skip_whitespace();
        scanner.start = scanner.current;
        
        char c = advance();
        if (is_digit(c)) { token_number(); continue; }
        printf("%d\n", scanner.current);
        switch (c) {
            case '+': token_create(TOK_PLUS); continue;
            case '-': token_create(TOK_MINUS); continue;
            case '*': token_create(TOK_STAR);  continue;
            case '/': token_create(TOK_SLASH); continue;

            case '\0': token_create(TOK_EOF); return;
            default:
                fprintf(stderr, "We do not recognize the char %c.\n", peek());
                exit(69);
                continue;
        }
    }
}
