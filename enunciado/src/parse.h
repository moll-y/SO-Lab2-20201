#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>

#include "lex.h"

typedef struct _sParser {
    Lex *lex;
    Token *lah;
} Parser;


typedef struct _sNode {
    Token *token;
    struct _sNode *next;
} Node;

Parser *parse_make(Lex *);
Node *parse_parse(void);

#endif
