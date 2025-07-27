#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

typedef struct {
    const char* start;
    const char* current;
} Lexer;

static Lexer lexer;

void init_lexer(const char* source) {
    lexer.start = source;
    lexer.current = source;
}

static int is_at_end() {
    return *lexer.current == '\0';
}

static char advance() {
    return *lexer.current++;
}

static char peek() {
    return *lexer.current;
}

static char peek_next() {
    if (is_at_end()) return '\0';
    return *(lexer.current + 1);
}

static int match_char(char expected) {
    if (is_at_end()) return 0;
    if (*lexer.current != expected) return 0;
    lexer.current++;
    return 1;
}

static void skip_whitespace() {
    while (isspace(peek())) {
        advance();
    }
}

static Token make_token(TokenType type) {
    Token token;
    token.type = type;
    size_t length = lexer.current - lexer.start;
    token.lexeme = (char*)malloc(length + 1);
    if (!token.lexeme) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    strncpy(token.lexeme, lexer.start, length);
    token.lexeme[length] = '\0';
    return token;
}

static Token error_token(const char* message) {
    Token token;
    token.type = TOKEN_UNKNOWN;
    token.lexeme = strdup(message);
    if (!token.lexeme) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    return token;
}

static int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

static int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

static Token identifier() {
    while (is_alpha(peek()) || is_digit(peek())) advance();

    size_t length = lexer.current - lexer.start;
    char* text = (char*)malloc(length + 1);
    if (!text) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    strncpy(text, lexer.start, length);
    text[length] = '\0';

    // Keywords
    if (strcmp(text, "let") == 0) {
        free(text);
        return make_token(TOKEN_LET);
    }
    if (strcmp(text, "mut") == 0) {
        free(text);
        return make_token(TOKEN_MUT);
    }
    if (strcmp(text, "if") == 0) {
        free(text);
        return make_token(TOKEN_IF);
    }
    if (strcmp(text, "else") == 0) {
        free(text);
        return make_token(TOKEN_ELSE);
    }
    if (strcmp(text, "elif") == 0) {
        free(text);
        return make_token(TOKEN_ELIF);
    }
    if (strcmp(text, "unsafe") == 0) {
        free(text);
        return make_token(TOKEN_UNSAFE);
    }
    // Type tokens: u8, u16, i32, etc.
    if ((text[0] == 'u' || text[0] == 'i') && length > 1) {
        int all_digits = 1;
        for (size_t i = 1; i < length; i++) {
            if (!is_digit(text[i])) {
                all_digits = 0;
                break;
            }
        }
        if (all_digits) {
            free(text);
            return make_token(TOKEN_TYPE);
        }
    }

    Token token = make_token(TOKEN_IDENTIFIER);
    free(text);
    return token;
}

static Token number() {
    while (is_digit(peek())) advance();

    size_t length = lexer.current - lexer.start;
    Token token = make_token(TOKEN_NUMBER);
    return token;
}

Token get_next_token() {
    skip_whitespace();
    lexer.start = lexer.current;

    if (is_at_end()) return make_token(TOKEN_EOF);

    char c = advance();

    if (is_alpha(c)) return identifier();

    if (is_digit(c)) return number();

    switch (c) {
        case '{': return make_token(TOKEN_LBRACE);
        case '}': return make_token(TOKEN_RBRACE);
        case '=':
            if (match_char('=')) return make_token(TOKEN_EQ);
            else return make_token(TOKEN_ASSIGN);
        case ':': return make_token(TOKEN_COLON);
        case ';': return make_token(TOKEN_SEMICOLON);
        case '+': return make_token(TOKEN_PLUS);
        case '-': return make_token(TOKEN_MINUS);
        case '*': return make_token(TOKEN_ASTERISK);
        case '/': return make_token(TOKEN_SLASH);
        case '>': return make_token(TOKEN_GT);
    }

    return error_token("Unexpected character.");
}

void free_token(Token* token) {
    if (token->lexeme) {
        free(token->lexeme);
        token->lexeme = NULL;
    }
}
