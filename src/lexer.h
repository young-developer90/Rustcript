// lexer.h
#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_LET,
    TOKEN_MUT,
    TOKEN_UNSAFE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_ELIF,
    TOKEN_TYPE_U8,
    TOKEN_TYPE_U16,
    TOKEN_TYPE_I8,
    TOKEN_TYPE_I16,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_OPERATOR,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char* lexeme;
} Token;

Token* lex(const char* source_code, int* token_count);
void free_tokens(Token* tokens, int token_count);

void init_lexer(const char* source);
Token get_next_token(void);

#endif
