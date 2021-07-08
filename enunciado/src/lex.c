// lex.c - lexer

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lex.h"

static char lex_next(void);
static char lex_peek(void);
static void lex_ignore(void);
static void lex_space(void);
static Token *lex_emit(Type);
static Token *lex_word(void);

// ---------------------------------------------------------------------------

static Lex *lex;

static char lex_peek(void)
{
    char c = lex_next();
    lex->lah--;
    return c;
}

Lex *lex_make(void)
{
    lex = malloc(sizeof(Lex));
    lex->lah = 0;
    lex->srt = 0;
    lex->done = true;
    return lex;
}

void lex_sreadfrom(const char *input)
{
    lex->buf = input;
    lex->lah = 0;
    lex->srt = 0;
    lex->done = false;
}

static char lex_next(void)
{
    if (!lex->buf[lex->lah]) {
	lex->done = true;
    }

    char c = lex->buf[lex->lah];
    lex->lah++;
    return c;
}

static void lex_ignore(void)
{
    lex->srt = lex->lah;
}

static Token *lex_emit(Type type)
{
    Token *tok = malloc(sizeof(Token));
    size_t n_chars = lex->lah - lex->srt;

    tok->text = malloc(sizeof(char) * (n_chars + 1));
    strncpy(tok->text, lex->buf + lex->srt, n_chars);

    tok->text[n_chars] = '\0';
    tok->type = type;
    lex->srt = lex->lah;
    return tok;
}

Token *lex_gettok(void)
{
    for (;;) {
	switch (lex_next()) {

	default:
	    return lex_word();

	case '>':
	    return lex_emit(TGreat);

	case '\n':
	    return lex_emit(TNewline);

	case '&':
	    return lex_emit(TAnd);

	case '\0':
	    return lex_emit(TEOF);

	case ' ':
	case '\t':
	case '\v':
	case '\f':
	case '\r':
	    lex_space();
	    break;

	}
    }
}

static Token *lex_word(void)
{
    for (;;) {
	switch (lex_peek()) {

	default:
	    lex_next();
	    break;

	case '\t':
	case '\v':
	case '\f':
	case '\r':
	case '\n':
	case '\0':
	case '>':
	case '&':
	case ' ':
	    return lex_emit(TWord);
	}
    }
}

static void lex_space(void)
{
    for (;;) {
	switch (lex_peek()) {

	default:
	    lex_ignore();
	    return;

	case ' ':
	case '\t':
	case '\v':
	case '\f':
	case '\r':
	    lex_next();
	    break;
	}
    }
}
