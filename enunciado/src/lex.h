#ifndef LEX_H
#define LEX_H

#include <stdbool.h>

typedef enum {
    TEOF,
    TWord,
    TNewline,			// \n
    TAnd,			// &
    TGreat,			// >
} Type;

typedef struct _sToken {
    char *text;
    Type type;
} Token;

typedef struct __sLex {
    const char *buf;
    size_t lah;
    size_t srt;
    bool done;
} Lex;

Lex *lex_make(void);
void lex_sreadfrom(const char *);
Token *lex_gettok(void);

#endif
