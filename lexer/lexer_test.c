#include <printf.h>
#include "lexer.h"
#include "../token/token.h"
#include "../libft/libft.h"

struct test {
	enum e_token_type expected_type;
	char *expected_literal;
};

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
char *debug_token_type[20] = {
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
		"REDIRECT",
		"HEREDOC",
		"LPAREN",
		"RPAREN",
		"AND_IF",
		"OR_IF",
		"STRING",
		"ENVIRONMENT",
};

int main()
{
{
	char input1[] = "echo hello|cat";
	struct test test[4] = {
			{COMMAND, "echo"},
			{ARGUMENT, "hello"},
			{PIPE, "|"},
			{COMMAND, "cat"},
	};

	t_lexer *lexer = new_lexer(input1);

	read_char(lexer);

	for (int i = 0; i < 4; ++i) {
		t_token *token = next_token(lexer);

		printf("token->literal:'%s'\n", token->literal);
		//todo: printf is used - make sure not to include _test.c in srcs when compiling
		if (token->type != test[i].expected_type)
			printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_token_type[test[i].expected_type], debug_token_type[token->type]);
		if (ft_strcmp(token->literal, test[i].expected_literal))
			printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, token->literal);
		free(token->literal);
		free(token);
  }
  
	{
		char input[] = "echo hello||cat";
		struct test test[4] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{OR_IF, "||"},
				{STRING, "cat"},
		};
		t_lexer *lexer = new_lexer(input);

		for (int i = 0; i < 4; ++i) {
			t_token token = next_token(lexer);
			if (token.type != test[i].expected_type)
				printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_token_type[test[i].expected_type], debug_token_type[token.type]);
			if (ft_strncmp(token.literal, test[i].expected_literal, ft_strlen(test[i].expected_literal)))
				printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, token.literal);
		}
		free(lexer);
	}

	{
		char input[] = "echo hello>res";
		struct test test[4] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{REDIRECT, ">"},
				{STRING, "res"},
		};
		t_lexer *lexer = new_lexer(input);

		for (int i = 0; i < 4; ++i) {
			t_token token = next_token(lexer);
			if (token.type != test[i].expected_type)
				printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_token_type[test[i].expected_type], debug_token_type[token.type]);
			if (ft_strncmp(token.literal, test[i].expected_literal, ft_strlen(test[i].expected_literal)))
				printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, token.literal);
		}
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
		t_lexer *lexer = new_lexer(input);

		for (int i = 0; i < 5; ++i) {
			t_token token = next_token(lexer);
			if (token.type != test[i].expected_type)
				printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_token_type[test[i].expected_type], debug_token_type[token.type]);
			if (ft_strncmp(token.literal, test[i].expected_literal, ft_strlen(test[i].expected_literal)))
				printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, token.literal);
		}
		free(lexer);
	}

	{
		//todo: how to deal with "echo$HELLO'a'"
		char input[] = "echo$HELLO 'a'";
		struct test test[5] = {
				{STRING, "echo"},
				{ENVIRONMENT, "$HELLO"},
				{AND_IF, "&&"},
				{STRING, "echo"},
				{STRING, "success"},
		};
		t_lexer *lexer = new_lexer(input);

		for (int i = 0; i < 5; ++i) {
			t_token token = next_token(lexer);
			if (token.type != test[i].expected_type)
				printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_token_type[test[i].expected_type], debug_token_type[token.type]);
			if (ft_strncmp(token.literal, test[i].expected_literal, ft_strlen(test[i].expected_literal)))
				printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, token.literal);
		}
		free(lexer);
	}
}