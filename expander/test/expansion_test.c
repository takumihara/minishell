#include <string.h>
#include "../expander.h"
#include "../internal/expander_internal.h"
#include "../../parser/parser.h"
#include "../../lexer/lexer.h"

#define BLUE    "\033[34m"      /* Blue */
#define RESET   "\033[0m"
#define RED     "\033[31m"

#define GENERAL_CASE -1
#define ERROR_CASE -2

typedef enum e_expand_type {
	ENV_VARS,
	QUOTES,
	WILDCARD,
	WORD_SPLIT,
}	t_expand_type;

char *debug_node_type[20] = {
	"ENV_VARS",
	"QUOTES",
	"WILDCARD",
	"WORD_SPLIT",
};

typedef struct s_test {
	t_node_type expected_type;
	char expected_literal[100];
} t_test;

void print_ast_nodes(t_ast_node *node, int level);
void test_expander(char input[], t_test *expected, int test_type);
void test_ast_nodes(t_ast_node *node, int level, t_test *expected);
void print_err_cnt();

int ast_index;
int err_cnt;

int main(int ac, char **av) {
	(void)ac;
	(void)av;
	{
		if (setenv("TEST", "hello", 1) != 0)
			perror("setenv");
		char input[] = "echo $TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, ENV_VARS);
	}
	{
		setenv("TEST", "ho", 1);
		char input[] = "ec$TEST hello";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, ENV_VARS);
	}
	{
		setenv("TEST", "hoge", 1);
		char input[] = "echo aaa$TEST$TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "aaahogehoge"},
		};
		test_expander(input, expected, ENV_VARS);
	}
	{
		setenv("TEST", "echo hello", 1);
		char input[] = "$TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, ENV_VARS);
	}
	{
		char input[] = "echo $HOGE";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, ""},
		};
		test_expander(input, expected, ENV_VARS);
	}
	{
		char input[] = "echo hello$HOGE";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, ENV_VARS);
	}
	{
		char input[] = "echo \"hoge\"";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "hoge"},
		};
		test_expander(input, expected, QUOTES);
	}
	{
		setenv("TEST", "example", 1);
		char input[] = "echo \"$TEST\"";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "example"},
		};
		test_expander(input, expected, QUOTES);
	}
	{
		setenv("TEST", "example", 1);
		char input[] = "echo \'$TEST\'";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "$TEST"},
		};
		test_expander(input, expected, QUOTES);
	}
	{
		setenv("USER", "user42", 1);
		char input[] = "echo \"\'$USER\'\"";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "\'user42\'"},
		};
		test_expander(input, expected, QUOTES);
	}
	{
		setenv("USER", "user42", 1);
		char input[] = "echo \'\"$USER\"\'";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "\"$USER\""},
		};
		test_expander(input, expected, QUOTES);
	}
	{
		char input[] = "echo exp*.c";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "expansion_test.c"},
				{COMMAND_ARG_NODE, "expansion_wildcard_test.c"},
		};
		test_expander(input, expected, WILDCARD);
	}
	{
		char input[] = "echo *.c";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "expansion_test.c"},
				{COMMAND_ARG_NODE, "expansion_wildcard_test.c"},
		};
		test_expander(input, expected, WILDCARD);
	}
	{
		char input[] = "echo expa*";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "expansion_test.c"},
				{COMMAND_ARG_NODE, "expansion_wildcard_test.c"},
		};
		test_expander(input, expected, WILDCARD);
	}
	{
		char input[] = "echo \"ex\"p*.\'c\'";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "expansion_test.c"},
				{COMMAND_ARG_NODE, "expansion_wildcard_test.c"},
		};
		test_expander(input, expected, WILDCARD);
	}
	{
		char input[] = "echo *";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "Makefile"},
				{COMMAND_ARG_NODE, "a.out"},
				{COMMAND_ARG_NODE, "expansion_test.c"},
				{COMMAND_ARG_NODE, "obj"},
				{COMMAND_ARG_NODE, "res"},
		};
		test_expander(input, expected, WILDCARD);
	}
	{
		char input[] = "echo \"*\"";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "*"},
		};
		test_expander(input, expected, WILDCARD);
	}
	{
		char input[] = "echo o*";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "obj"},
		};
		test_expander(input, expected, WILDCARD);
	}
	{
		char input[] = "\"echo hello\"";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		setenv("TEST", "echo hello", 1);
		char input[] = "\"ls -l\"$TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "ls -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		setenv("TEST", "echo hello", 1);
		char input[] = "hoge\"ls -l\"$TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		setenv("D", "hello", 1);
		char input[] = "echo $D$A";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		setenv("wildcard", "*", 1);
		char input[] = "echo $wildcard.c";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		char input[] = "echo \".\"*";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		char input[] = "echo \"..\"*";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		char input[] = "echo *.***";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		char input[] = "echo ***.***";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		char input[] = "echo *.\"*\"";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		char input[] = "echo M*\"k\"*le*";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		char input[] = "echo mi*ni*she*ll*";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		char input[] = "echo mi*ni*she*ll";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		char input[] = "echo *\".\"*";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		if (setenv("TEST", "res* res*", 1) != 0)
			perror("setenv");
		char input[] = "echo hello > $TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		if (setenv("TEST", "res* res*", 1) != 0)
			perror("setenv");
		char input[] = "echo $TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	{
		if (setenv("TEST", "res res1", 1) != 0)
			perror("setenv");
		char input[] = "echo hello > $TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, ENV_VARS);
	}
	{
		char input[] = "echo hello > $TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, ENV_VARS);
	}
	{
		char input[] = "echo res*";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hogels -lecho"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT);
	}
	system("leaks a.out");
}

void test_expander(char input[], t_test *expected, int test_type) {
	printf("\n---------------------------------\n");
	if (test_type == GENERAL_CASE)
		printf("	 [GENERAL] TEST\n");
	else if (test_type == ERROR_CASE)
		printf("	 [ERROR] TEST\n");
	else
		printf("	 [%s] TEST\n", debug_node_type[test_type]);
	printf("---------------------------------\n");
	printf("input:%s\n", input);

	(void)expected;
	t_token		*token = lex(input);
	t_ast_node	*root = parse(token);
	t_env_var	*env_vars = init_env_lst();
	// if (!res) {
	// 	fprintf(stderr, RED "parse() returned NULL!\n" RESET);
	// 	err_cnt++;
	// 	return;
	// }

	// if (test_type == ERROR_CASE) {
	// 	char *err_msg = (char *)res;
	// 	printf("expected:\n\t%s", expected[0].expected_literal);
	// 	printf("actual:\n\t%s", err_msg);
	// 	if (ft_strcmp(err_msg, expected[0].expected_literal) != 0) {
	// 		fprintf(stderr, RED "error message wrong!\n" RESET);
	// 		err_cnt++;
	// 	}
	// } else {
		expand(root, &env_vars);
		ast_index = 0;
		print_ast_nodes(root, 0);
		ast_index = 0;
		// test_ast_nodes(root, 0, expected);
		delete_env_lst(env_vars);
		delete_ast_nodes(root, NULL);
	// }

	printf("\n---------------------------------\n");
}

void test_ast_nodes(t_ast_node *node, int level, t_test *expected) {
	if (!node)
		return;
	if (node->type != expected[ast_index].expected_type) {
		fprintf(stderr, RED "test[%d] - node type wrong. expected=%s, got=%s\n" RESET, ast_index,
				debug_node_type[expected[ast_index].expected_type], debug_node_type[node->type]);
		err_cnt++;
	}
	if (node->data && ft_strcmp(node->data, expected[ast_index].expected_literal)) {
		fprintf(stderr, RED "test[%d] - node literal wrong. expected=%s, got=%s\n" RESET, ast_index,
				expected[ast_index].expected_literal,
				node->data);
		err_cnt++;
	}
	ast_index++;
	test_ast_nodes(node->left, level + 1, expected);
	test_ast_nodes(node->right, level + 1, expected);
}

void print_ast_nodes(t_ast_node *node, int level) {
	if (!node)
		return;
	printf("{Index: %d, Level: %d, Literal:'%s'}\n", ast_index, level, node->data);
	ast_index++;
	print_ast_nodes(node->left, level + 1);
	print_ast_nodes(node->right, level + 1);
}

// void print_err_cnt()
// {
// 	if (err_cnt == 0)
// 		printf(BLUE "TOTAL ERROR COUNT: 0 \n" RESET);
// 	else
// 		printf(RED "TOTAL ERROR COUNT: %d \n" RESET, err_cnt);
// }
