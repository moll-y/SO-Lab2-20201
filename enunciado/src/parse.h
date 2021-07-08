#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>

#include "lex.h"

typedef struct _sParser {
	Lex * lex;
	Token * lah;
} Parser;

Parser * parse_make(Lex * lex);
void parse_parse();

#endif
