#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_EOF,
    TOKEN_LET,
    TOKEN_MUT,
    TOKEN_IDENTIFIER,
    TOKEN_TYPE,
    TOKEN_NUMBER,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_ELIF,
    TOKEN_UNSAFE,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_ASSIGN,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_ASTERISK,
    TOKEN_SLASH,
    TOKEN_GT,
    TOKEN_EQ,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char* lexeme;
} Token;

#endif
