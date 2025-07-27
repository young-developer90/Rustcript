#include <stdio.h>
#include "token.h"
#include <stdlib.h>

Token get_next_token();
void init_lexer(const char* source);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source.rscr>\n", argv[0]);
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        printf("Error: Cannot open file %s\n", argv[1]);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* source = malloc(size + 1);
    fread(source, 1, size, f);
    source[size] = '\0';
    fclose(f);

    init_lexer(source);

    Token token;
    do {
        token = get_next_token();
        printf("Token: %-15d Lexeme: %s\n", token.type, token.lexeme);
        free(token.lexeme);
    } while (token.type != TOKEN_EOF);

    free(source);

    return 0;
}
