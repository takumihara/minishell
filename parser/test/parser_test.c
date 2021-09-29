#include <string.h>
#include "../parser.h"

#define GENERAL_CASE -1
#define ERROR_CASE -2

#define BLUE    "\033[34m"      /* Blue */


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
	int expected_level;
	char expected_literal[52];
} test;

void print_ast_nodes(t_ast_node *node, int level);
void test_parser(char input[], test *expected, int test_type);
void test_ast_nodes(t_ast_node *node, int level, test *expected);
void print_err_cnt();

int ast_index;
int err_cnt;

int main() {
	{
		char input[] = "echo -n hello > res | cat && echo success || echo failure";
		test expected[] = {
				{AND_IF_NODE,           0, ""},
				{PIPE_NODE,             1, ""},
				{COMMAND_ARG_NODE,      2, "echo"},
				{COMMAND_ARG_NODE,      3, "-n"},
				{COMMAND_ARG_NODE,      4, "hello"},
				{REDIRECT_OUT_NODE,     5, ""},
				{REDIRECT_OPERAND_NODE, 6, "res"},
				{COMMAND_ARG_NODE,      2, "cat"},
				{OR_IF_NODE,            1, ""},
				{COMMAND_ARG_NODE,      2, "echo"},
				{COMMAND_ARG_NODE,      3, "success"},
				{COMMAND_ARG_NODE,      2, "echo"},
				{COMMAND_ARG_NODE,      3, "failure"},
		};
		test_parser(input, expected, GENERAL_CASE);
	}
	{
		char input[] = "cat << EOS hello | ls -l | wc -l >> 1.txt || echo failure";
		test expected[] = {
				{OR_IF_NODE,            0, ""},
				{PIPE_NODE,             1, ""},
				{COMMAND_ARG_NODE,      2, "cat"},
				{HEREDOC_NODE,          3, ""},
				{REDIRECT_OPERAND_NODE, 4, "EOS"},
				{COMMAND_ARG_NODE,      4, "hello"},
				{PIPE_NODE,             2, ""},
				{COMMAND_ARG_NODE,      3, "ls"},
				{COMMAND_ARG_NODE,      4, "-l"},
				{COMMAND_ARG_NODE,      3, "wc"},
				{COMMAND_ARG_NODE,      4, "-l"},
				{REDIRECT_APPEND_NODE,  5, ""},
				{REDIRECT_OPERAND_NODE, 6, "1.txt"},
				{COMMAND_ARG_NODE,      1, "echo"},
				{COMMAND_ARG_NODE,      2, "failure"},
		};
		test_parser(input, expected, HEREDOC_NODE);
	}
	{
		char input[] = "cat | ls -l | wc -l 12>> 1.txt hello || echo failure";
		test expected[] = {
				{OR_IF_NODE,            0, ""},
				{PIPE_NODE,             1, ""},
				{COMMAND_ARG_NODE,      2, "cat"},
				{PIPE_NODE,             2, ""},
				{COMMAND_ARG_NODE,      3, "ls"},
				{COMMAND_ARG_NODE,      4, "-l"},
				{COMMAND_ARG_NODE,      3, "wc"},
				{COMMAND_ARG_NODE,      4, "-l"},
				{REDIRECT_APPEND_NODE,  5, "12"},
				{REDIRECT_OPERAND_NODE, 6, "1.txt"},
				{COMMAND_ARG_NODE,      6, "hello"},
				{COMMAND_ARG_NODE,      1, "echo"},
				{COMMAND_ARG_NODE,      2, "failure"},
		};
		test_parser(input, expected, REDIRECT_APPEND_NODE);
	}
	{
		char input[] = "(cd ..)";
		test expected[] = {
				{SUBSHELL_NODE,    0, ""},
				{COMMAND_ARG_NODE, 1, "cd"},
				{COMMAND_ARG_NODE, 2, ".."},
		};
		test_parser(input, expected, SUBSHELL_NODE);
	}
	{
		char input[] = "(echo success || echo failure)";
		test expected[] = {
				{SUBSHELL_NODE,    0, ""},
				{OR_IF_NODE,       1, ""},
				{COMMAND_ARG_NODE, 2, "echo"},
				{COMMAND_ARG_NODE, 3, "success"},
				{COMMAND_ARG_NODE, 2, "echo"},
				{COMMAND_ARG_NODE, 3, "failure"},
		};
		test_parser(input, expected, SUBSHELL_NODE);
	}
	{
		char input[] = "echo hoge && (echo success || echo failure)";
		test expected[] = {
				{AND_IF_NODE,      0, ""},
				{COMMAND_ARG_NODE, 1, "echo"},
				{COMMAND_ARG_NODE, 2, "hoge"},
				{SUBSHELL_NODE,    1, ""},
				{OR_IF_NODE,       2, ""},
				{COMMAND_ARG_NODE, 3, "echo"},
				{COMMAND_ARG_NODE, 4, "success"},
				{COMMAND_ARG_NODE, 3, "echo"},
				{COMMAND_ARG_NODE, 4, "failure"},
		};
		test_parser(input, expected, SUBSHELL_NODE);
	}
	{
		char input[] = "(echo success) < input 2> res >> res1 << EOL";
		test expected[] = {
				{SUBSHELL_NODE,         0, ""},
				{COMMAND_ARG_NODE,      1, "echo"},
				{COMMAND_ARG_NODE,      2, "success"},
				{REDIRECT_IN_NODE,      1, ""},
				{REDIRECT_OPERAND_NODE, 2, "input"},
				{REDIRECT_OUT_NODE,     2, "2"},
				{REDIRECT_OPERAND_NODE, 3, "res"},
				{REDIRECT_APPEND_NODE,  3, ""},
				{REDIRECT_OPERAND_NODE, 4, "res1"},
				{HEREDOC_NODE,          4, ""},
				{REDIRECT_OPERAND_NODE, 5, "EOL"},
		};
		test_parser(input, expected, SUBSHELL_NODE);
	}
	{
		char input[] = "echo \"hello\"";
		test expected[] = {
				{COMMAND_ARG_NODE, 0, "echo"},
				{COMMAND_ARG_NODE, 1, "\"hello\""},
		};
		test_parser(input, expected, COMMAND_ARG_NODE);
	}
	{
		char input[] = "echo \'hello\'";
		test expected[] = {
				{COMMAND_ARG_NODE, 0, "echo"},
				{COMMAND_ARG_NODE, 1, "\'hello\'"},
		};
		test_parser(input, expected, COMMAND_ARG_NODE);
	}
	{
		char input[] = "(\n\n\n echo hello \n\n\n)";
		test expected[] = {
				{SUBSHELL_NODE,    0, ""},
				{COMMAND_ARG_NODE, 1, "echo"},
				{COMMAND_ARG_NODE, 2, "hello"},
		};
		test_parser(input, expected, SUBSHELL_NODE);
	}
	{
		char input[] = "(echo hello \n echo success)";
		test expected[] = {
				{SUBSHELL_NODE,         0, ""},
				{SUBSHELL_NEWLINE_NODE, 1, ""},
				{COMMAND_ARG_NODE,      2, "echo"},
				{COMMAND_ARG_NODE,      3, "hello"},
				{COMMAND_ARG_NODE,      2, "echo"},
				{COMMAND_ARG_NODE,      3, "success"},
		};
		test_parser(input, expected, SUBSHELL_NODE);
	}
	{
		char input[] = "echo hello && echo success";
		test expected[] = {
				{AND_IF_NODE,         0, ""},
				{COMMAND_ARG_NODE,      1, "echo"},
				{COMMAND_ARG_NODE,      2, "hello"},
				{COMMAND_ARG_NODE,      1, "echo"},
				{COMMAND_ARG_NODE,      2, "success"},
		};
		test_parser(input, expected, AND_IF_NODE);
	}
	{
		char input[] = "(echo hello";
		test expected[] = {
				{UNSET_NODE, 0, "minishell: syntax error: unexpected end of file\n"},
		};
		test_parser(input, expected, ERROR_CASE);
	}
	{
		char input[] = "echo hello)";
		test expected[] = {
				{UNSET_NODE, 0, "minishell: syntax error near unexpected token `)'\n"},
		};
		test_parser(input, expected, ERROR_CASE);
	}
	{
		char input[] = "(echo success) && ";
		test expected[] = {
				{UNSET_NODE, 0, "minishell: syntax error: unexpected end of file\n"},
		};
		test_parser(input, expected, ERROR_CASE);
	}
	{
		char input[] = "( && )";
		test expected[] = {
				{UNSET_NODE, 0, "minishell: syntax error near unexpected token `&&'\n"},
		};
		test_parser(input, expected, ERROR_CASE);
	}
	{
		char input[] = "( \n )";
		test expected[] = {
				{UNSET_NODE, 0, "minishell: syntax error near unexpected token `)'\n"},
		};
		test_parser(input, expected, ERROR_CASE);
	}
	{
		char input[] = "echo success > &&";
		test expected[] = {
				{UNSET_NODE, 0, "minishell: syntax error near unexpected token `&&'\n"},
		};
		test_parser(input, expected, ERROR_CASE);
	}
	{
		char input[] = "(echo hello > res1 2>> ||";
		test expected[] = {
				{UNSET_NODE, 0, "minishell: syntax error near unexpected token `||'\n"},
		};
		test_parser(input, expected, ERROR_CASE);
	}
	{
		char input[] = "echo hello ||";
		test expected[] = {
				{UNSET_NODE, 0, "minishell: syntax error: unexpected end of file\n"},
		};
		test_parser(input, expected, ERROR_CASE);
	}
	{
		char input[] = "echo hello |";
		test expected[] = {
				{UNSET_NODE, 0, "minishell: syntax error: unexpected end of file\n"},
		};
		test_parser(input, expected, ERROR_CASE);
	}
	{
		char input[] = "echo hello |)";
		test expected[] = {
				{UNSET_NODE, 0, "minishell: syntax error: unexpected end of file\n"},
		};
		test_parser(input, expected, ERROR_CASE);
	}
	{
		char input[] = "echo hello | echo hello | echo hello |";
		test expected[] = {
				{UNSET_NODE, 0, "minishell: syntax error: unexpected end of file\n"},
		};
		test_parser(input, expected, ERROR_CASE);
	}
	// {
	// 	char input[] = "(echo hello > res1 << \n";
	// 	test expected[] = {
	// 			{UNSET_NODE, 0, "minishell: syntax error near unexpected token `newline'\n"},
	// 	};
	// 	test_parser(input, expected, ERROR_CASE);
	// }
	{
		char input[] = "(echo hello > res1 \n";
		test expected[] = {
				{UNSET_NODE, 0, "minishell: syntax error: unexpected end of file\n"},
		};
		test_parser(input, expected, ERROR_CASE);
	}


	print_err_cnt();
}

void test_parser(char input[], test *expected, int test_type) {
	printf("\n---------------------------------\n");
	if (test_type == GENERAL_CASE)
		printf("	 [GENERAL] TEST\n");
	else if (test_type == ERROR_CASE)
		printf("	 [ERROR] TEST\n");
	else
		printf("	 [%s] TEST\n", debug_node_type[test_type]);
	printf("---------------------------------\n");
	printf("input:%s\n", input);

	t_token *token = lex(input);
	void *res = parse(token);

	if (test_type == ERROR_CASE) {
		char *err_msg = (char *)res;
		printf("expected:\n\t%s", expected[0].expected_literal);
		printf("actual:\n\t%s", err_msg);
		if (ft_strcmp(err_msg, expected[0].expected_literal) != 0) {
			fprintf(stderr, RED "error message wrong!\n" RESET);
			err_cnt++;
		}
	} else {
		t_ast_node *node = (t_ast_node *)res;
		ast_index = 0;
		print_ast_nodes(node, 0);
		ast_index = 0;
		test_ast_nodes(node, 0, expected);
	}

	printf("\n---------------------------------\n");
}

void test_ast_nodes(t_ast_node *node, int level, test *expected) {
	if (!node)
		return;
	char *literal;
	if (node->data) {
		literal = (char *) calloc(node->data->len + 1, sizeof(char));
		ft_memmove(literal, node->data->start, node->data->len);
	} else
		literal = strdup("");
	if (node->type != expected[ast_index].expected_type) {
		fprintf(stderr, RED "test[%d] - node type wrong. expected=%s, got=%s\n" RESET, ast_index,
				debug_node_type[expected[ast_index].expected_type], debug_node_type[node->type]);
		err_cnt++;
	}
	if (level != expected[ast_index].expected_level) {
		fprintf(stderr, RED "test[%d] - node level wrong. expected=%d, got=%d\n" RESET, ast_index,
				expected[ast_index].expected_level,
				level);
		err_cnt++;
	}
	if (ft_strcmp(literal, expected[ast_index].expected_literal)) {
		fprintf(stderr, RED "test[%d] - node literal wrong. expected=%s, got=%s\n" RESET, ast_index,
				expected[ast_index].expected_literal,
				literal);
		err_cnt++;
	}
	ast_index++;
	free(literal);
	test_ast_nodes(node->left, level + 1, expected);
	test_ast_nodes(node->right, level + 1, expected);
}

void print_ast_nodes(t_ast_node *node, int level) {
	if (!node)
		return;
	char *literal;
	if (node->data) {
		literal = (char *) calloc(node->data->len + 1, sizeof(char));
		ft_memmove(literal, node->data->start, node->data->len);
	} else
		literal = strdup("");
	printf("{Index: %d, Level: %d, Type:%s, Literal:'%s'}\n", ast_index, level, debug_node_type[node->type], literal);
	free(literal);
	ast_index++;
	print_ast_nodes(node->left, level + 1);
	print_ast_nodes(node->right, level + 1);
}

void print_err_cnt()
{
	if (err_cnt == 0)
		printf(BLUE "TOTAL ERROR COUNT: 0 \n" RESET);
	else
		printf(RED "TOTAL ERROR COUNT: %d \n" RESET, err_cnt);
}
