//
// parse.c - parser
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lex.h"
#include "parse.h"

static bool parse_expect(Type);
static bool parse_accept(Type);
static void parse_program(void);
static void parse_command(void);
static void parse_list(void);
static void parse_list_prime(void);
static void parse_separator_op(void);
static void parse_simple_command(void);
static void parse_cmd_name(void);
static void parse_cmd_suffix(void);
static void parse_io_file(void);
static void parse_filename(void);
static void parse_linebreak(void);
static void parse_newline_list(void);
static void parse_newline_list_prime(void);

// ----------------------------------------------------------------------------

static Parser *parser;

Parser *parse_make(Lex * lex)
{
    parser = malloc(sizeof(Parser));
    parser->lex = lex;
    return parser;
}

static bool parse_expect(Type type)
{
    return parser->lah->type == type;
}

static bool parse_accept(Type type)
{
    if (parse_expect(type)) {
	parser->lah = lex_gettok();
	return true;
    }

    return false;
}

Node *root;
Node *node;

Node *parse_parse(void)
{
    parser->lah = lex_gettok();
    root = malloc(sizeof(Node));
    root->next = NULL;
    node = root;

    parse_program();

    if (parser->lah->type != TEOF) {
	fprintf(stderr, "parse_parse: error\n");
    }

    Node *next = malloc(sizeof(Node));
    next->token = parser->lah;
    next->next = NULL;
    node->next = next;
    node = next;
    return root;
}

// program            : command linebreak
//                    | linebreak
//                    ;
static void parse_program(void)
{
    if (parse_expect(TNewline)) {
	parse_linebreak();
	return;
    }

    parse_command();
    parse_linebreak();
}

// command            : list separator_op
//                    | list
//                    ;
static void parse_command(void)
{
    parse_list();

    if (parse_expect(TAnd)) {
	parse_separator_op();
    }
}

// list               : simple_command list_prime
//                    ;
static void parse_list(void)
{
    parse_simple_command();
    parse_list_prime();
}

// list_prime         : separator_op list_prime
//                    | /*eps*/
//                    ;
static void parse_list_prime(void)
{
    if (parse_expect(TAnd)) {
	parse_separator_op();
	parse_list_prime();
    }
}

// separator_op       : TAnd
//                    ;
static void parse_separator_op(void)
{
    Node *next = malloc(sizeof(Node));
    next->token = parser->lah;
    next->next = NULL;
    node->next = next;
    node = next;

    if (!parse_accept(TAnd)) {
	fprintf(stderr, "parse_separator_op: error\n");
    }
}

// simple_command     : cmd_name
//                    : cmd_name io_file
//                    | cmd_name cmd_suffix io_file
//                    ;
static void parse_simple_command(void)
{
    parse_cmd_name();

    if (parse_expect(TGreat)) {
	parse_io_file();
	return;
    }

    if (parse_expect(TWord)) {
	parse_cmd_suffix();
	parse_io_file();
	return;
    }
}

// cmd_name           : TWord
//                    ;
static void parse_cmd_name(void)
{
    Node *next = malloc(sizeof(Node));
    next->token = parser->lah;
    next->next = NULL;
    node->next = next;
    node = next;

    if (!parse_accept(TWord)) {
	fprintf(stderr, "parse_cmd_name: error\n");
    }
}

// cmd_suffix         : TWord cmd_suffix
//                    | /*eps*/
//                    ;
static void parse_cmd_suffix(void)
{
    Node *next = malloc(sizeof(Node));
    next->token = parser->lah;
    next->next = NULL;
    node->next = next;
    node = next;

    if (parse_accept(TWord)) {
	parse_cmd_suffix();
    }
}

// io_file            : TGreat filename
//                    | /*eps*/
//                    ;
static void parse_io_file(void)
{
    Node *next = malloc(sizeof(Node));
    next->token = parser->lah;
    next->next = NULL;
    node->next = next;
    node = next;

    if (parse_accept(TGreat)) {
	parse_filename();
    }
}

// filename           : TWord
//                    ;
static void parse_filename(void)
{
    Node *next = malloc(sizeof(Node));
    next->token = parser->lah;
    next->next = NULL;
    node->next = next;
    node = next;

    if (parse_accept(TWord)) {

	if (parse_expect(TWord)) {
	    fprintf(stderr, "filename: error\n");
	}

	return;
    }

    fprintf(stderr, "filename: error\n");

}

// linebreak          : newline_list
//                    | /*eps*/
//                    ;
static void parse_linebreak(void)
{
    if (parse_expect(TNewline)) {
	parse_newline_list();
    }
}

// newline_list       : TNewline newline_list_prime
//                    ;
static void parse_newline_list(void)
{
    Node *next = malloc(sizeof(Node));
    next->token = parser->lah;
    next->next = NULL;
    node->next = next;
    node = next;

    if (!parse_accept(TNewline)) {
	fprintf(stderr, "parse_separator_op: error\n");
	return;
    }

    parse_newline_list_prime();
}

// newline_list_prime : TNewline newline_list_prime
//                    ; /*eps*/
static void parse_newline_list_prime(void)
{
    Node *next = malloc(sizeof(Node));
    next->token = parser->lah;
    next->next = NULL;
    node->next = next;
    node = next;

    if (parse_accept(TNewline)) {
	parse_newline_list_prime();
    }
}
