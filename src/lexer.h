#ifndef LEXER_H
#define LEXER_H

#include "token.h"

void init_lexer(const char* source);
Token get_next_token();
void free_token(Token* token);

#endif
