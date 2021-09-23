#include "../parser.h"

char *join_tokens(t_tokens *tokens);

char *debug_node_type[20] = {
		"PROGRAM",
		"BUILTIN",
};

int main()
{
	struct test {
		char input[30];
		t_node_type expected_type;
		char expected_literal[30];
	};
	// test builtin
	{
		struct test test[2] = {
				{"echo -n hello", BUILTIN, "echo -n hello"},
				{"echo     -n    hello", BUILTIN, "echo -n hello"},
		};

		for (int i = 0; i < 2; ++i) {
			printf("\n---------------------------------\n");
			printf("input:%s\n", test[i].input);

			t_lexer *lexer = new_lexer(test[i].input);
			read_char(lexer);
			t_node *node = parse(lexer);

			char *literal = join_tokens(node->tokens);

			printf("{Type:%s, Literal:'%s'}\n", debug_node_type[node->type], literal);

			if (node->type != test[i].expected_type)
				printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_node_type[test[i].expected_type], debug_node_type[node->type]);
			// todo
			if (ft_strcmp(literal, test[i].expected_literal))
				printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, literal);
			free(literal);
			free(lexer);
		}
		printf("---------------------------------\n");
	}

	// test program
	{
		struct test test[2] = {
				{"cat -n hello.txt", PROGRAM, "echo -n hello"},
				{"cat     -n    hello.txt", PROGRAM, "echo -n hello"},
		};

		for (int i = 0; i < 2; ++i) {
			printf("\n---------------------------------\n");
			printf("input:%s\n", test[i].input);

			t_lexer *lexer = new_lexer(test[i].input);
			read_char(lexer);
			t_node *node = parse(lexer);

			char *literal = join_tokens(node->tokens);

			printf("{Type:%s, Literal:'%s'}\n", debug_node_type[node->type], literal);

			if (node->type != test[i].expected_type)
				printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_node_type[test[i].expected_type], debug_node_type[node->type]);
			// todo
			if (ft_strcmp(literal, test[i].expected_literal))
				printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, literal);
			free(literal);
			free(lexer);
		}
		printf("---------------------------------\n");
	}
}

char *join_tokens(t_tokens *tokens)
{
	size_t	i;
	char	*rtn;
	t_token	*itr;

	rtn = NULL;
	i = 0;
	itr = tokens->head;
	while (1)
	{
		rtn = st_append(rtn, &itr->literal);
		if (!rtn)
			return (NULL);
		if (++i == tokens->len)
			return (rtn);
		rtn = ft_strappend(rtn, " ", 1);
		itr = itr->next;
	}
}