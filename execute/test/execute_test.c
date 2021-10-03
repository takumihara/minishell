#include "../execute.h"
#include "../../lexer/lexer.h"
#include "../../parser/parser.h"

#define GENERAL_CASE -1
#define ERROR_CASE -2

#define BLUE    "\033[34m"      /* Blue */

typedef struct s_test {
	t_node_type expected_type;
	int expected_level;
	char expected_literal[52];
} test;

void test_execution(char input[], test *expected, int test_type);

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("argument num wrong");
		return(1);
	}
	t_token *token = lex(argv[1]);
	t_ast_node *node = parse(token);
	if (!node)
	{
		fprintf(stderr, RED "parse() returned NULL!\n" RESET);
		return (1);
	}
	execute(node);
}

