// lexer.c
#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define MAX_TOKENS 1024

char* my_strndup(const char* s, size_t n) {
    if (n == 0) {
        char* p = malloc(1);
        if (!p) return NULL;
        p[0] = '\0';
        return p;
    }
    char* p = malloc(n + 1);
    if (!p) return NULL;
    strncpy(p, s, n);
    p[n] = '\0';
    return p;
}

// توابع کمکی برای شناسایی کلمات کلیدی
static TokenType check_keyword(const char* str) {
    if (strcmp(str, "let") == 0) return TOKEN_LET;
    if (strcmp(str, "mut") == 0) return TOKEN_MUT;
    if (strcmp(str, "unsafe") == 0) return TOKEN_UNSAFE;
    if (strcmp(str, "if") == 0) return TOKEN_IF;
    if (strcmp(str, "else") == 0) return TOKEN_ELSE;
    if (strcmp(str, "elif") == 0) return TOKEN_ELIF;
    if (strcmp(str, "u8") == 0) return TOKEN_TYPE_U8;
    if (strcmp(str, "u16") == 0) return TOKEN_TYPE_U16;
    if (strcmp(str, "i8") == 0) return TOKEN_TYPE_I8;
    if (strcmp(str, "i16") == 0) return TOKEN_TYPE_I16;
    return TOKEN_IDENTIFIER;
}

// Lexer بسیار ساده فقط برای نمونه:
Token* lex(const char* source_code, int* token_count) {
    Token* tokens = malloc(sizeof(Token) * MAX_TOKENS);
    int count = 0;
    int i = 0;

    while (source_code[i] != '\0') {
        if (count >= MAX_TOKENS) {
            fprintf(stderr, "Error: too many tokens\n");
            break;
        }

        if (isspace(source_code[i])) {
            i++;
            continue;
        }

        if (isalpha(source_code[i])) {
            int start = i;
            while (isalnum(source_code[i]) || source_code[i] == '_') i++;

            int length = i - start;
            char* word = malloc(length + 1);
            strncpy(word, &source_code[start], length);
            word[length] = '\0';

            TokenType type = check_keyword(word);

            tokens[count++] = (Token){ .type = type, .lexeme = word };
            continue;
        }

        if (isdigit(source_code[i])) {
            int start = i;
            while (isdigit(source_code[i])) i++;

            int length = i - start;
            char* num = malloc(length + 1);
            strncpy(num, &source_code[start], length);
            num[length] = '\0';

            tokens[count++] = (Token){ .type = TOKEN_NUMBER, .lexeme = num };
            continue;
        }

        // سایر کاراکترها (عملگرها، علامت‌ها)
        switch (source_code[i]) {
            case ':':
                tokens[count++] = (Token){ .type = TOKEN_COLON, .lexeme = my_strndup(":", 1) };
                i++;
                break;
            case ';':
                tokens[count++] = (Token){ .type = TOKEN_SEMICOLON, .lexeme = my_strndup(";", 1) };
                i++;
                break;
            case '{':
                tokens[count++] = (Token){ .type = TOKEN_LBRACE, .lexeme = my_strndup("{", 1) };
                i++;
                break;
            case '}':
                tokens[count++] = (Token){ .type = TOKEN_RBRACE, .lexeme = my_strndup("}", 1) };
                i++;
                break;
            case '+': case '-': case '*': case '/': case '=': case '>': case '<':
                tokens[count++] = (Token){ .type = TOKEN_OPERATOR, .lexeme = my_strndup(&source_code[i],1) };
                i++;
                break;
            default:
                tokens[count++] = (Token){ .type = TOKEN_UNKNOWN, .lexeme = my_strndup(&source_code[i],1) };
                i++;
                break;
        }
    }

    tokens[count++] = (Token){ .type = TOKEN_EOF, .lexeme = my_strndup("", 0) };

    *token_count = count;
    return tokens;
}

void free_tokens(Token* tokens, int token_count) {
    for (int i=0; i<token_count; i++) {
        free(tokens[i].lexeme);
    }
    free(tokens);
}

// متغیرهای ایستا برای ذخیره توکن‌ها و وضعیت lexer
static Token* tokens = NULL;
static int total_tokens = 0;
static int current_token_index = 0;

void init_lexer(const char* source) {
    if (tokens != NULL) {
        free_tokens(tokens, total_tokens);
        tokens = NULL;
        total_tokens = 0;
        current_token_index = 0;
    }
    tokens = lex(source, &total_tokens);
    current_token_index = 0;
}

Token get_next_token(void) {
    if (tokens == NULL || current_token_index >= total_tokens) {
        return (Token){ .type = TOKEN_EOF, .lexeme = NULL };
    }
    return tokens[current_token_index++];
}
