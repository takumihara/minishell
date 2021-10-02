#include <string.h>
#include "../expansioner.h"

#define BLUE    "\033[34m"      /* Blue */

#define GENERAL_CASE -1
#define ERROR_CASE -2

char *debug_node_type[20] = {
		"UNSET_NODE",
		"PIPE_NODE",
		"AND_IF_NODE",
		"OR_IF_NODE",
		"COMMAND_ARG_NODE",
		"REDIRECT_OUT_NODE",
		"REDIRECT_IN_NODE",
		"REDIRECT_APPEND_NODE",
		"HEREDOC_NODE",
		"REDIRECT_OPERAND_NODE",
		"SUBSHELL_NODE",
		"SUBSHELL_NEWLINE_NODE",
};

typedef struct s_test {
	t_node_type expected_type;
	char expected_literal[57];
} t_test;

void print_ast_nodes(t_ast_node *node, int level);
void test_expansioner(char input[], t_test *expected, int test_type, char **envp);
void test_ast_nodes(t_ast_node *node, int level, t_test *expected);
void print_err_cnt();

int ast_index;
int err_cnt;

extern char	**environ;

int main(int ac, char **av, char **envp) {
	(void)ac;
	(void)av;
	(void)envp;
	{
		if (setenv("TEST", "hello", 1) != 0)
			perror("setenv");
		char input[] = "echo $TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expansioner(input, expected, GENERAL_CASE, environ);
	}
	{
		setenv("TEST", "ho", 1);
		char input[] = "ec$TEST hello";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "ech"},
				{COMMAND_ARG_NODE, "hello"},
		};
		test_expansioner(input, expected, GENERAL_CASE, environ);
	}
	{
		setenv("TEST", "hoge", 1);
		char input[] = "echo aaa$TEST$TEST";
		t_test expected[] = {
				{COMMAND_ARG_NODE, "echo"},
				{COMMAND_ARG_NODE, "aaahogehoge"},
		};
		test_expansioner(input, expected, GENERAL_CASE, environ);
	}
}

void test_expansioner(char input[], t_test *expected, int test_type, char **envp) {
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
				root = expansion(root, envp);
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
		// ast_index = 0;
		// test_ast_nodes(node, 0, expected);
	// }

	printf("\n---------------------------------\n");
}

// void test_ast_nodes(t_ast_node *node, int level, test *expected) {
// 	if (!node)
// 		return;
// 	char *literal;
// 	if (node->data) {
// 		literal = (char *) calloc(node->data->len + 1, sizeof(char));
// 		ft_memmove(literal, node->data->start, node->data->len);
// 	} else
// 		literal = strdup("");
// 	if (node->type != expected[ast_index].expected_type) {
// 		fprintf(stderr, RED "test[%d] - node type wrong. expected=%s, got=%s\n" RESET, ast_index,
// 				debug_node_type[expected[ast_index].expected_type], debug_node_type[node->type]);
// 		err_cnt++;
// 	}
// 	if (level != expected[ast_index].expected_level) {
// 		fprintf(stderr, RED "test[%d] - node level wrong. expected=%d, got=%d\n" RESET, ast_index,
// 				expected[ast_index].expected_level,
// 				level);
// 		err_cnt++;
// 	}
// 	if (ft_strcmp(literal, expected[ast_index].expected_literal)) {
// 		fprintf(stderr, RED "test[%d] - node literal wrong. expected=%s, got=%s\n" RESET, ast_index,
// 				expected[ast_index].expected_literal,
// 				literal);
// 		err_cnt++;
// 	}
// 	ast_index++;
// 	free(literal);
// 	test_ast_nodes(node->left, level + 1, expected);
// 	test_ast_nodes(node->right, level + 1, expected);
// }

void print_ast_nodes(t_ast_node *node, int level) {
	if (!node)
		return;
	char *literal;
	if (node->data) {
		literal = (char *) calloc(node->data->len + 1, sizeof(char));
		ft_memmove(literal, node->data->start, node->data->len);
	} else
	{
		printf("expansioner returns NULL\n");
		literal = strdup("");
	}
	printf("{Index: %d, Level: %d, Literal:'%s', Size:%zu}\n", ast_index, level, literal, node->data->len);
	free(literal);
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
