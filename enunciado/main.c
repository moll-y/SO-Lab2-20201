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
	Node *node = parse_parse();

	for (; node->next; node = node->next) {
	    if (node->token != NULL) {
		printf("%s -> ", node->token->text);
	    }
	}
	printf("\n");
    }
    free(lex);
    free(parser);
    return 0;
}
