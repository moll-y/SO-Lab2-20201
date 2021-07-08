#include <stdio.h>
#include <stdlib.h>

#include "src/lex.h"
#include "src/parse.h"

#define LEN 1000

int main(void)
{
    char input[LEN];
    Lex *lex = lex_make();
    Parser *parser = parse_make(lex);

    while (fgets(input, LEN, stdin)) {
	lex_sreadfrom(input);
	parse_parse();
    }
    free(lex);
    free(parser);
    return 0;
}
