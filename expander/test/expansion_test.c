#include <string.h>
#include "../expander.h"

#define BLUE    "\033[34m"      /* Blue */

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
void test_expander(char input[], t_test *expected, int test_type, char **envp);
void test_ast_nodes(t_ast_node *node, int level, t_test *expected);
void print_err_cnt();

int ast_index;
int err_cnt;

extern char	**environ;

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
		test_expander(input, expected, ENV_VARS, environ);
	}
	{
		setenv("TEST", "ho", 1);
		char input[] = "ec$TEST hello";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, ENV_VARS, environ);
	}
	{
		setenv("TEST", "hoge", 1);
		char input[] = "echo aaa$TEST$TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "aaahogehoge"},
		};
		test_expander(input, expected, ENV_VARS, environ);
	}
	{
		setenv("TEST", "echo hello", 1);
		char input[] = "$TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, ENV_VARS, environ);
	}
	{
		char input[] = "echo $HOGE";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, ""},
		};
		test_expander(input, expected, ENV_VARS, environ);
	}
	{
		char input[] = "echo hello$HOGE";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, ENV_VARS, environ);
	}
	{
		char input[] = "echo \"hoge\"";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "\"hoge\""},
		};
		test_expander(input, expected, QUOTES, environ);
	}
	{
		setenv("TEST", "example", 1);
		char input[] = "echo \"$TEST\"";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "\"example\""},
		};
		test_expander(input, expected, QUOTES, environ);
	}
	{
		setenv("TEST", "example", 1);
		char input[] = "echo \'$TEST\'";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "\'$TEST\'"},
		};
		test_expander(input, expected, QUOTES, environ);
	}
	{
		setenv("USER", "user42", 1);
		char input[] = "echo \"\'$USER\'\"";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "\"\'user42\'\""},
		};
		test_expander(input, expected, QUOTES, environ);
	}
	{
		setenv("USER", "user42", 1);
		char input[] = "echo \'\"$USER\"\'";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "\'\"$USER\"\'"},
		};
		test_expander(input, expected, QUOTES, environ);
	}
	{
		char input[] = "echo exp*.c";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "expansion_test.c"},
				{COMMAND_ARG_NODE, "expansion_wildcard_test.c"},
		};
		test_expander(input, expected, WILDCARD, environ);
	}
	{
		char input[] = "echo *.c";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "expansion_test.c"},
				{COMMAND_ARG_NODE, "expansion_wildcard_test.c"},
		};
		test_expander(input, expected, WILDCARD, environ);
	}
	{
		char input[] = "echo expa*";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "expansion_test.c"},
				{COMMAND_ARG_NODE, "expansion_wildcard_test.c"},
		};
		test_expander(input, expected, WILDCARD, environ);
	}
	{
		char input[] = "echo \"ex\"p*.\'c\'";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "expansion_test.c"},
				{COMMAND_ARG_NODE, "expansion_wildcard_test.c"},
		};
		test_expander(input, expected, WILDCARD, environ);
	}
	{
		char input[] = "echo *";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "Makefile"},
				{COMMAND_ARG_NODE, "a.out"},
				{COMMAND_ARG_NODE, "expansion_test.c"},
				{COMMAND_ARG_NODE, "expansion_wildcard_test.c"},
				{COMMAND_ARG_NODE, "obj"},
		};
		test_expander(input, expected, WILDCARD, environ);
	}
	{
		char input[] = "echo o*";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "obj"},
		};
		test_expander(input, expected, WILDCARD, environ);
	}
	{
		char input[] = "\"echo hello\"";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "\"echo hello\""},
		};
		test_expander(input, expected, WORD_SPLIT, environ);
	}
	{
		setenv("TEST", "echo hello", 1);
		char input[] = "\"ls -l\"$TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "\"ls -l\"echo"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT, environ);
	}
	{
		setenv("TEST", "echo hello", 1);
		char input[] = "hoge\"ls -l\"$TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "hoge\"ls -l\"echo"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expander(input, expected, WORD_SPLIT, environ);
	}
}

void test_expander(char input[], t_test *expected, int test_type, char **envp) {
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
				root = expand(root, envp);
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
		ast_index = 0;
		print_ast_nodes(root, 0);
		ast_index = 0;
		test_ast_nodes(root, 0, expected);
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
	if (ft_strcmp(node->data, expected[ast_index].expected_literal)) {
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
