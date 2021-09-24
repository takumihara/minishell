#include <printf.h>
#include "../lexer.h"
#include "../../token/token.h"
#include "../../libft/libft.h"

typedef struct test {
	enum e_token_type expected_type;
	char *expected_literal;
}t_test;

//	ILLEGAL,
//	EOL,
//	IDENT,
//	COMMAND,
//	FILENAME,
//	ASSIGN,
//	PLUS,
//	MINUS,
//	ARGUMENT,
//	PIPE,
//	REDIRECT,
//	LPAREN,
//	RPAREN,
//	AND_IF,
//	OR_IF,
//	STRING,
char *debug_token_type[30] = {
		"ILLEGAL",
		"EOL",
		"IDENT",
		"COMMAND",
		"FILENAME",
		"ASSIGN",
		"PLUS",
		"MINUS",
		"ARGUMENT",
		"PIPE",
		"REDIRECT_IN",  // "<"
    	"REDIRECT_OUT",  // ">"
    	"HEREDOC",  // "<<"
    	"REDIRECT_APPEND", // ">>"
		"LPAREN",
		"RPAREN",
		"AND_IF",
		"OR_IF",
		"STRING",
		"ENVIRONMENT",
};

void	compare_literal_and_type(char *input, char **debug_token_type, int expected_type, t_test *test, int token_num);

int main()
{
	{
		char input[] = "echo hello|cat";
		struct test test[4] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{PIPE, "|"},
				{STRING, "cat"},
		};
		compare_literal_and_type(input, debug_token_type, PIPE, test, 4);
	}
  
	{
		char input[] = "echo hello||cat";
		struct test test[4] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{OR_IF, "||"},
				{STRING, "cat"},
		};
		printf("\n---------------------------------\n");
		printf("	 [%s] TEST\n", debug_token_type[OR_IF]);
		printf("---------------------------------\n");
		printf("input:%s\n", input);

		t_lexer *lexer = new_lexer(input);
		t_token *token;
		t_token *head_token;
		char	*token_literal_str;

		token = lexer_main(lexer);
		head_token = token;

		for (int i = 0; i < 4; ++i) {
			token_literal_str = (char *)malloc(sizeof(char) * (token->literal.len + 1));

			ft_memmove(token_literal_str, token->literal.start, token->literal.len);
			token_literal_str[token->literal.len] = '\0';
			printf("{Type:%s, Literal:'%s'}\n", debug_token_type[token->type], token_literal_str);

			//todo: printf is used - make sure not to include _test.c in srcs when compiling
			if (token->type != test[i].expected_type)
				printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_token_type[test[i].expected_type], debug_token_type[token->type]);
			if (ft_strncmp(token->literal.start, test[i].expected_literal, token->literal.len))
				printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, token_literal_str);
			free(token_literal_str);
			token = token->next;
		}
		token_lstclear(&head_token);
		free(lexer);
	}

	{
		char input[] = "echo hello>res";
		struct test test[4] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{REDIRECT_OUT, ">"},
				{STRING, "res"},
		};
		printf("\n---------------------------------\n");
		printf("	 [%s] TEST\n", debug_token_type[REDIRECT_OUT]);
		printf("---------------------------------\n");
		printf("input:%s\n", input);

		t_lexer *lexer = new_lexer(input);
		t_token *token;
		t_token *head_token;
		char	*token_literal_str;

		token = lexer_main(lexer);
		head_token = token;

		for (int i = 0; i < 4; ++i) {
			token_literal_str = (char *)malloc(sizeof(char) * (token->literal.len + 1));

			ft_memmove(token_literal_str, token->literal.start, token->literal.len);
			token_literal_str[token->literal.len] = '\0';
			printf("{Type:%s, Literal:'%s'}\n", debug_token_type[token->type], token_literal_str);

			//todo: printf is used - make sure not to include _test.c in srcs when compiling
			if (token->type != test[i].expected_type)
				printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_token_type[test[i].expected_type], debug_token_type[token->type]);
			if (ft_strncmp(token->literal.start, test[i].expected_literal, token->literal.len))
				printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, token_literal_str);
			free(token_literal_str);
			token = token->next;
		}
		token_lstclear(&head_token);
		free(lexer);
	}

	{
		char input[] = "echo hello>>res";
		struct test test[4] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{REDIRECT_APPEND, ">>"},
				{STRING, "res"},
		};
		printf("\n---------------------------------\n");
		printf("	 [%s] TEST\n", debug_token_type[REDIRECT_APPEND]);
		printf("---------------------------------\n");
		printf("input:%s\n", input);

		t_lexer *lexer = new_lexer(input);
		t_token *token;
		t_token *head_token;
		char	*token_literal_str;

		token = lexer_main(lexer);
		head_token = token;

		for (int i = 0; i < 4; ++i) {
			token_literal_str = (char *)malloc(sizeof(char) * (token->literal.len + 1));

			ft_memmove(token_literal_str, token->literal.start, token->literal.len);
			token_literal_str[token->literal.len] = '\0';
			printf("{Type:%s, Literal:'%s'}\n", debug_token_type[token->type], token_literal_str);

			//todo: printf is used - make sure not to include _test.c in srcs when compiling
			if (token->type != test[i].expected_type)
				printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_token_type[test[i].expected_type], debug_token_type[token->type]);
			if (ft_strncmp(token->literal.start, test[i].expected_literal, token->literal.len))
				printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, token_literal_str);
			free(token_literal_str);
			token = token->next;
		}
		token_lstclear(&head_token);
		free(lexer);
	}

	{
		char input[] = "cat < res";
		struct test test[3] = {
				{STRING, "cat"},
				{REDIRECT_IN, "<"},
				{STRING, "res"},
		};
		printf("\n---------------------------------\n");
		printf("	 [%s] TEST\n", debug_token_type[REDIRECT_IN]);
		printf("---------------------------------\n");
		printf("input:%s\n", input);

		t_lexer *lexer = new_lexer(input);
		t_token *token;
		t_token *head_token;
		char	*token_literal_str;

		token = lexer_main(lexer);
		head_token = token;

		for (int i = 0; i < 3; ++i) {
			token_literal_str = (char *)malloc(sizeof(char) * (token->literal.len + 1));

			ft_memmove(token_literal_str, token->literal.start, token->literal.len);
			token_literal_str[token->literal.len] = '\0';
			printf("{Type:%s, Literal:'%s'}\n", debug_token_type[token->type], token_literal_str);

			//todo: printf is used - make sure not to include _test.c in srcs when compiling
			if (token->type != test[i].expected_type)
				printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_token_type[test[i].expected_type], debug_token_type[token->type]);
			if (ft_strncmp(token->literal.start, test[i].expected_literal, token->literal.len))
				printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, token_literal_str);
			free(token_literal_str);
			token = token->next;
		}
		token_lstclear(&head_token);
		free(lexer);
	}

	{
		char input[] = "cat < res";
		struct test test[3] = {
				{STRING, "cat"},
				{REDIRECT_IN, "<"},
				{STRING, "res"},
		};
		printf("\n---------------------------------\n");
		printf("	 [%s] TEST\n", debug_token_type[REDIRECT_IN]);
		printf("---------------------------------\n");
		printf("input:%s\n", input);

		t_lexer *lexer = new_lexer(input);
		t_token *token;
		t_token *head_token;
		char	*token_literal_str;

		token = lexer_main(lexer);
		head_token = token;

		for (int i = 0; i < 3; ++i) {
			token_literal_str = (char *)malloc(sizeof(char) * (token->literal.len + 1));

			ft_memmove(token_literal_str, token->literal.start, token->literal.len);
			token_literal_str[token->literal.len] = '\0';
			printf("{Type:%s, Literal:'%s'}\n", debug_token_type[token->type], token_literal_str);

			//todo: printf is used - make sure not to include _test.c in srcs when compiling
			if (token->type != test[i].expected_type)
				printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_token_type[test[i].expected_type], debug_token_type[token->type]);
			if (ft_strncmp(token->literal.start, test[i].expected_literal, token->literal.len))
				printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, token_literal_str);
			free(token_literal_str);
			token = token->next;
		}
		token_lstclear(&head_token);
		free(lexer);
	}

	{
		char input[] = "ls -l&&echo success";
		struct test test[5] = {
				{STRING, "ls"},
				{STRING, "-l"},
				{AND_IF, "&&"},
				{STRING, "echo"},
				{STRING, "success"},
		};
		printf("\n---------------------------------\n");
		printf("	 [%s] TEST\n", debug_token_type[AND_IF]);
		printf("---------------------------------\n");
		printf("input:%s\n", input);

		t_lexer *lexer = new_lexer(input);
		t_token *token;
		t_token *head_token;
		char	*token_literal_str;

		token = lexer_main(lexer);
		head_token = token;

		for (int i = 0; i < 5; ++i) {
			token_literal_str = (char *)malloc(sizeof(char) * (token->literal.len + 1));

			ft_memmove(token_literal_str, token->literal.start, token->literal.len);
			token_literal_str[token->literal.len] = '\0';
			printf("{Type:%s, Literal:'%s'}\n", debug_token_type[token->type], token_literal_str);

			//todo: printf is used - make sure not to include _test.c in srcs when compiling
			if (token->type != test[i].expected_type)
				printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_token_type[test[i].expected_type], debug_token_type[token->type]);
			if (ft_strncmp(token->literal.start, test[i].expected_literal, token->literal.len))
				printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, token_literal_str);
			free(token_literal_str);
			token = token->next;
		}
		token_lstclear(&head_token);
		free(lexer);
	}

	{
		char input[] = "ls -l & echo success";
		struct test test[5] = {
				{STRING, "ls"},
				{STRING, "-l"},
				{ILLEGAL, "&"},
				{STRING, "echo"},
				{STRING, "success"},
		};
		printf("\n---------------------------------\n");
		printf("	 [%s] TEST\n", debug_token_type[ILLEGAL]);
		printf("---------------------------------\n");
		printf("input:%s\n", input);

		t_lexer *lexer = new_lexer(input);
		t_token *token;
		t_token *head_token;
		char	*token_literal_str;

		token = lexer_main(lexer);
		head_token = token;

		for (int i = 0; i < 5; ++i) {
			token_literal_str = (char *)malloc(sizeof(char) * (token->literal.len + 1));

			ft_memmove(token_literal_str, token->literal.start, token->literal.len);
			token_literal_str[token->literal.len] = '\0';
			printf("{Type:%s, Literal:'%s'}\n", debug_token_type[token->type], token_literal_str);

			//todo: printf is used - make sure not to include _test.c in srcs when compiling
			if (token->type != test[i].expected_type)
				printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_token_type[test[i].expected_type], debug_token_type[token->type]);
			if (ft_strncmp(token->literal.start, test[i].expected_literal, token->literal.len))
				printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, token_literal_str);
			free(token_literal_str);
			token = token->next;
		}
		token_lstclear(&head_token);
		free(lexer);
	}

	{
		//todo: how to deal with "echo$HELLO'a'"
		char input[] = "echo$HELLO";
		struct test test[2] = {
				{STRING, "echo"},
				{ENVIRONMENT, "$HELLO"},
		};
		compare_literal_and_type(input, debug_token_type, ENVIRONMENT, test, 2);
	}
}

void	compare_literal_and_type(char *input, char **debug_token_type, int expected_type, t_test *test, int token_num)
{
	printf("\n---------------------------------\n");
	printf("	 [%s] TEST\n", debug_token_type[expected_type]);
	printf("---------------------------------\n");
	printf("input:%s\n", input);

	t_lexer *lexer = new_lexer(input);
	t_token *token;
	t_token *head_token;
	char	*token_literal_str;

	token = lexer_main(lexer);
	head_token = token;

	for (int i = 0; i < token_num; ++i) {
		token_literal_str = (char *)malloc(sizeof(char) * (token->literal.len + 1));

		ft_memmove(token_literal_str, token->literal.start, token->literal.len);
		token_literal_str[token->literal.len] = '\0';
		printf("{Type:%s, Literal:'%s'}\n", debug_token_type[token->type], token_literal_str);

		//todo: printf is used - make sure not to include _test.c in srcs when compiling
		if (token->type != test[i].expected_type)
			printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_token_type[test[i].expected_type], debug_token_type[token->type]);
		if (ft_strncmp(token->literal.start, test[i].expected_literal, token->literal.len))
			printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, token_literal_str);
		free(token_literal_str);
		token = token->next;
	}
	token_lstclear(&head_token);
	free(lexer);
}