#include <string.h>
#include "../parser.h"

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
		"FILENAME_NODE",
};

typedef struct s_test {
	t_node_type expected_type;
	int expected_level;
	char expected_literal[10];
}	test;

void	print_ast_nodes(t_ast_node *node, int level);
void	test_parser(char input[], test *expected, t_node_type test_type);
void	test_ast_nodes(t_ast_node *node, int level, test *expected);

int ast_index;

int main()
{
	{
		char input[] = "echo -n hello > res | cat && echo success || echo failure";
		test expected[15] = {
				{AND_IF_NODE, 0,""},
				{PIPE_NODE, 1,""},
				{COMMAND_ARG_NODE, 2,"echo"},
				{COMMAND_ARG_NODE, 3,"-n"},
				{COMMAND_ARG_NODE, 4,"hello"},
				{REDIRECT_OUT_NODE, 5,""},
				{FILENAME_NODE, 6,"res"},
				{COMMAND_ARG_NODE, 2,"cat"},
				{OR_IF_NODE, 1,""},
				{COMMAND_ARG_NODE, 2,"echo"},
				{COMMAND_ARG_NODE, 3,"success"},
				{COMMAND_ARG_NODE, 2,"echo"},
				{COMMAND_ARG_NODE, 3,"failure"},
		};
		test_parser(input, expected, -1);
	}
	// i guess lexer is not working yet...
//	{
//		char input[] = "cat << EOS hello | ls -l | wc -l >> 1.txt || echo failure";
//		test expected[16] = {
//				{OR_IF_NODE, 0,""},
//				{PIPE_NODE, 1,""},
//				{COMMAND_ARG_NODE, 2,"cat"},
//				{HEREDOC_NODE, 3,""},
//				{HEREDOC_DELIM_NODE, 4,"EOS"},
//				{COMMAND_ARG_NODE, 5,"hello"},
//				{PIPE_NODE, 2,""},
//				{COMMAND_ARG_NODE, 3,"ls"},
//				{COMMAND_ARG_NODE, 4,"-l"},
//				{COMMAND_ARG_NODE, 3,"wc"},
//				{COMMAND_ARG_NODE, 4,"-l"},
//				{REDIRECT_APPEND_NODE, 5,""},
//				{FILENAME_NODE, 6,"1.txt"},
//				{COMMAND_ARG_NODE, 1,"echo"},
//				{COMMAND_ARG_NODE, 2,"failure"},
//		};
//		test_parser(input, expected, HEREDOC_NODE);
//	}
//	{
//		char input[] = "cat | ls -l | wc -l >> 1.txt hello || echo failure";
//		test expected[16] = {
//				{OR_IF_NODE, 0,""},
//				{PIPE_NODE, 1,""},
//				{COMMAND_ARG_NODE, 2,"cat"},
//				{PIPE_NODE, 2,""},
//				{COMMAND_ARG_NODE, 3,"ls"},
//				{COMMAND_ARG_NODE, 4,"-l"},
//				{COMMAND_ARG_NODE, 3,"wc"},
//				{COMMAND_ARG_NODE, 4,"-l"},
//				{REDIRECT_APPEND_NODE, 5,""},
//				{FILENAME_NODE, 6,"1.txt"},
//				{COMMAND_ARG_NODE, 1,"echo"},
//				{COMMAND_ARG_NODE, 2,"failure"},
//		};
//		test_parser(input, expected, REDIRECT_APPEND_NODE);
//	}
}

void	test_parser(char input[], test *expected, t_node_type test_type)
{
	printf("\n---------------------------------\n");
	if (test_type == -1)
		printf("	 [GENERAL] TEST\n");
	else
		printf("	 [%s] TEST\n", debug_node_type[test_type]);
	printf("---------------------------------\n");
	printf("input:%s\n", input);

	t_lexer *lexer = new_lexer(input);
	t_token *token = lexer_main(lexer);
	t_ast_node *node = parse(token);
	ast_index = 0;
	print_ast_nodes(node, 0);
	ast_index = 0;
	test_ast_nodes(node, 0, expected);

	printf("\n---------------------------------\n");
}

void	test_ast_nodes(t_ast_node *node, int level, test *expected)
{
	if (!node)
		return ;
	char *literal;
	if (node->data)
	{
		literal = (char *) calloc(node->data->len + 1, sizeof(char));
		ft_memmove(literal, node->data->start, node->data->len);
	}
	else
		literal = strdup("");
	if (node->type != expected[ast_index].expected_type)
		printf("test[%d] - node type wrong. expected=%s, got=%s\n", ast_index, debug_node_type[expected[ast_index].expected_type], debug_node_type[node->type]);
	if (level != expected[ast_index].expected_level)
		printf("test[%d] - node level wrong. expected=%d, got=%d\n", ast_index, expected[ast_index].expected_level, level);
	if (ft_strcmp(literal, expected[ast_index].expected_literal))
		printf("test[%d] - node literal wrong. expected=%s, got=%s\n", ast_index, expected[ast_index].expected_literal, literal);
	ast_index++;
	free(literal);
	test_ast_nodes(node->left, level + 1, expected);
	test_ast_nodes(node->right, level + 1, expected);
}

void	print_ast_nodes(t_ast_node *node, int level)
{
	if (!node)
		return ;
	char *literal;
	if (node->data)
	{
		literal = (char *) calloc(node->data->len + 1, sizeof(char));
		ft_memmove(literal, node->data->start, node->data->len);
	}
	else
		literal = strdup("");
	printf("{Index: %d, Level: %d, Type:%s, Literal:'%s'}\n", ast_index, level, debug_node_type[node->type], literal);
	free(literal);
	ast_index++;
	print_ast_nodes(node->left, level + 1);
	print_ast_nodes(node->right, level + 1);
}
