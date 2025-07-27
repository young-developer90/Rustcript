#include <stdio.h>
#include "lexer.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    // خواندن محتوای فایل به صورت رشته
    FILE* f = fopen(argv[1], "rb");
    if (!f) {
        perror("Failed to open file");
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* source = malloc(size + 1);
    fread(source, 1, size, f);
    source[size] = '\0';
    fclose(f);

    // مقداردهی اولیه lexer
    init_lexer(source);

    Token token;
    do {
        token = get_next_token();
        printf("Token: %d\tLexeme: %s\n", token.type, token.lexeme ? token.lexeme : "NULL");
    } while (token.type != TOKEN_EOF);

    free(source);
    // اگر در init_lexer حافظه‌ای برای توکن‌ها اختصاص داده شده بود، می‌توان آزاد کرد:
    // free_tokens(tokens, total_tokens);  // البته اگر دسترسی به tokens و total_tokens داشته باشی

    return 0;
}
