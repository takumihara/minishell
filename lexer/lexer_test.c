#include <printf.h>
#include "lexer.h"
#include "../token/token.h"
#include "../libft/libft.h"

struct test {
	enum e_token_type expected_type;
	char *expected_literal;
};

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
};

int main()
{
	char input1[] = "echo hello|cat";
	struct test test[4] = {
			{COMMAND, "echo"},
			{ARGUMENT, "hello"},
			{PIPE, "|"},
			{COMMAND, "cat"},
	};

	t_lexer *lexer = new_lexer(input1);

	for (int i = 0; i < 4; ++i) {
		t_token token = next_token(lexer);

		//todo: printf is used - make sure not to include _test.c in srcs when compiling
		if (token.type != test[i].expected_type)
			printf("test[%d] - token type wrong. expected=%s, got=%s\n", i, debug_token_type[test[i].expected_type], debug_token_type[token.type]);
		if (!ft_strncmp(token.literal, test[i].expected_literal, ft_strlen(test[i].expected_literal)))
			printf("test[%d] - token literal wrong. expected=%s, got=%s\n", i, test[i].expected_literal, token.literal);
//		free(token.literal);
	}
	free(lexer);
	for (int i = 0; i < 4; ++i) {
	}
}