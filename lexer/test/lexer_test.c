#include <printf.h>
#include "../lexer.h"
#include "../../token/token.h"
#include "../../libft/libft.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */

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
		"NOT_CLOSED",
		"REDIRECT_MODIFIER",
		"SUBSHELL_NEWLINE",
};

void	compare_literal_and_type(char *input, char **debug_token_type, int expected_type, t_test *test, int token_num);

int main()
{
	{
		char input[] = "echo hello|cat";
		struct test test[] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{PIPE, "|"},
				{STRING, "cat"},
				{EOL, "\0"},
		};
		compare_literal_and_type(input, debug_token_type, PIPE, test, 5);
	}
  
	{
		char input[] = "echo hello||cat";
		struct test test[4] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{OR_IF, "||"},
				{STRING, "cat"},
		};
		compare_literal_and_type(input, debug_token_type, OR_IF, test, 4);
	}

	{
		char input[] = "echo hello>res";
		struct test test[4] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{REDIRECT_OUT, ">"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_OUT, test, 4);
	}

	{
		char input[] = "echo hello>>res";
		struct test test[4] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{REDIRECT_APPEND, ">>"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_APPEND, test, 4);
	}

	{
		char input[] = "cat < res";
		struct test test[3] = {
				{STRING, "cat"},
				{REDIRECT_IN, "<"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_IN, test, 3);
	}

	{
		char input[] = "cat << end";
		struct test test[3] = {
				{STRING, "cat"},
				{HEREDOC, "<<"},
				{STRING, "end"},
		};
		compare_literal_and_type(input, debug_token_type, HEREDOC, test, 3);
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
		compare_literal_and_type(input, debug_token_type, AND_IF, test, 5);
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
		compare_literal_and_type(input, debug_token_type, ILLEGAL, test, 5);
	}

	{
		char input[] = "echo$HELLO";
		struct test test[2] = {
				{STRING, "echo"},
				{ENVIRONMENT, "$HELLO"},
		};
		compare_literal_and_type(input, debug_token_type, ENVIRONMENT, test, 2);
	}

	{
		char input[] = "echo \'hello\'";
		struct test test[2] = {
				{STRING, "echo"},
				{STRING, "\'hello\'"},
		};
		compare_literal_and_type(input, debug_token_type, STRING, test, 2);
	}

	{
		char input[] = "echo \'hello";
		struct test test[2] = {
				{STRING, "echo"},
				{STRING, "\'hello"},
		};
		compare_literal_and_type(input, debug_token_type, STRING, test, 2);
	}

	{
		char input[] = "echo \"hello\"";
		struct test test[2] = {
				{STRING, "echo"},
				{STRING, "\"hello\""},
		};
		compare_literal_and_type(input, debug_token_type, STRING, test, 2);
	}

	{
		char input[] = "echo \"$PATH\"";
		struct test test[2] = {
				{STRING, "echo"},
				{STRING, "\"$PATH\""},
		};
		compare_literal_and_type(input, debug_token_type, STRING, test, 2);
	}

	{
		char input[] = "echo \"hello";
		struct test test[2] = {
				{STRING, "echo"},
				{STRING, "\"hello"},
		};
		compare_literal_and_type(input, debug_token_type, STRING, test, 2);
	}

	{
		char input[] = "echo \"$PATH";
		struct test test[2] = {
				{STRING, "echo"},
				{STRING, "\"$PATH"},
		};
		compare_literal_and_type(input, debug_token_type, STRING, test, 2);
	}
	
	{
		char input[] = "(echo $PATH)";
		struct test test[4] = {
				{LPAREN, "("},
				{STRING, "echo"},
				{ENVIRONMENT, "$PATH"},
				{RPAREN, ")"},
		};
		compare_literal_and_type(input, debug_token_type, LPAREN, test, 4);
	}

	{
		char input[] = "export TEST=\"test\"";
		struct test test[2] = {
				{STRING, "export"},
				{STRING, "TEST=\"test\""},
		};
		compare_literal_and_type(input, debug_token_type, STRING, test, 2);
	}

	{
		char input[] = "e\"$TEST\"o hello";
		struct test test[2] = {
				{STRING, "e\"$TEST\"o"},
				{STRING, "hello"},
		};
		compare_literal_and_type(input, debug_token_type, STRING, test, 2);
	}

	{
		char input[] = "echo hello 11 res";
		struct test test[4] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{STRING, "11"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, STRING, test, 4);
	}

	{
		char input[] = "echo hello 1a> res";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{STRING, "1a"},
				{REDIRECT_OUT, ">"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_OUT, test, 5);
	}

	{
		char input[] = "echo hello 1> res";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{REDIRECT_MODIFIER, "1"},
				{REDIRECT_OUT, ">"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_OUT, test, sizeof(test)/sizeof(test[0]));
	}

	{
		char input[] = "echo hello 100> res";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{REDIRECT_MODIFIER, "100"},
				{REDIRECT_OUT, ">"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_OUT, test, sizeof(test)/sizeof(test[0]));
	}

	{
		char input[] = "echo hello 1 > res";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{STRING, "1"},
				{REDIRECT_OUT, ">"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_OUT, test, 5);
	}

	{
		char input[] = "echo hello 1a< res";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{STRING, "1a"},
				{REDIRECT_IN, "<"},
				{STRING, "res"},
		};
		
		compare_literal_and_type(input, debug_token_type, REDIRECT_IN, test, 5);
	}

	{
		char input[] = "echo hello 2< res";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{REDIRECT_MODIFIER, "2"},
				{REDIRECT_IN, "<"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_IN, test, 5);
	}

	{
		char input[] = "echo hello 200< res";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{REDIRECT_MODIFIER, "200"},
				{REDIRECT_IN, "<"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_IN, test, 5);
	}

	{
		char input[] = "echo hello 1 < res";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{STRING, "1"},
				{REDIRECT_IN, "<"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_IN, test, 5);
	}

	{
		char input[] = "echo hello 1a<< res";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{STRING, "1a"},
				{HEREDOC, "<<"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, HEREDOC, test, 5);
	}

	{
		char input[] = "echo hello 3<< res";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{REDIRECT_MODIFIER, "3"},
				{HEREDOC, "<<"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, HEREDOC, test, 5);
	}

	{
		char input[] = "echo hello 1 << res";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{STRING, "1"},
				{HEREDOC, "<<"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, HEREDOC, test, 5);
	}

	{
		char input[] = "echo hello 1a>> res";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{STRING, "1a"},
				{REDIRECT_APPEND, ">>"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_APPEND, test, 5);
	}

	{
		char input[] = "echo hello 4>> res";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{REDIRECT_MODIFIER, "4"},
				{REDIRECT_APPEND, ">>"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_APPEND, test, 5);
	}

	{
		char input[] = "echo hello 1 >> res";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{STRING, "1"},
				{REDIRECT_APPEND, ">>"},
				{STRING, "res"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_APPEND, test, 5);
	}

	{
		char input[] = "echo hello |\n cat";
		struct test test[4] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{PIPE, "|"},
				{STRING, "cat"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_APPEND, test, 4);
	}

	{
		char input[] = "echo hello &&\n echo world";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{AND_IF, "&&"},
				{STRING, "echo"},
				{STRING, "world"},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_APPEND, test, 5);
	}

	{
		char input[] = "echo hello ||\n cd ..";
		struct test test[5] = {
				{STRING, "echo"},
				{STRING, "hello"},
				{OR_IF, "||"},
				{STRING, "cd"},
				{STRING, ".."},
		};
		compare_literal_and_type(input, debug_token_type, REDIRECT_APPEND, test, 5);
	}

	{
		char input[] = "(\n\necho\n $PATH\n)";
		struct test test[7] = {
				{LPAREN, "("},
				{SUBSHELL_NEWLINE, "\n\n"},
				{STRING, "echo"},
				{SUBSHELL_NEWLINE, "\n"},
				{ENVIRONMENT, "$PATH"},
				{RPAREN, ")"},
				{EOL, "\0"},
		};
		compare_literal_and_type(input, debug_token_type, LPAREN, test, 7);
	}

	{
		char input[] = "(echo hello \n echo success)";
		struct test test[7] = {
				{LPAREN, "("},
				{STRING, "echo"},
				{STRING, "hello"},
				{SUBSHELL_NEWLINE, "\n"},
				{STRING, "echo"},
				{STRING, "success;"},
				{RPAREN, ")"},
		};
		compare_literal_and_type(input, debug_token_type, LPAREN, test, 7);
	}

	{
		char input[] = "(\n\n\n echo hello \n\n\n)";
		struct test test[6] = {
				{LPAREN, "("},
				{SUBSHELL_NEWLINE, "\n\n\n"},
				{STRING, "echo"},
				{STRING, "hello"},
				{RPAREN, ")"},
				{EOL, "\0"},
		};
		compare_literal_and_type(input, debug_token_type, LPAREN, test, 6);
	}

}

void	compare_literal_and_type(char *input, char **debug_token_type, int expected_type, t_test *test, int token_num)
{
	printf("\n---------------------------------\n");
	printf("	 [%s] TEST\n", debug_token_type[expected_type]);
	printf("---------------------------------\n");
	printf("input:%s\n", input);

	t_token *token;
	t_token *head_token;
	char	*token_literal_str;

	token = lex(input);
	head_token = token;

	for (int i = 0; i < token_num; ++i) {
		token_literal_str = (char *)malloc(sizeof(char) * (token->literal.len + 1));

		ft_memmove(token_literal_str, token->literal.start, token->literal.len);
		token_literal_str[token->literal.len] = '\0';
		printf("{%s, \"%s\"}\n", debug_token_type[token->type], token_literal_str);

		//todo: printf is used - make sure not to include _test.c in srcs when compiling
		if (token->type != test[i].expected_type)
			fprintf(stderr, RED "test[%d] - token type wrong. expected=%s, got=%s\n" RESET, i, debug_token_type[test[i].expected_type], debug_token_type[token->type]);
		if (ft_strncmp(token->literal.start, test[i].expected_literal, token->literal.len))
			fprintf(stderr, RED "test[%d] - token literal wrong. expected=%s, got=%s\n" RESET, i, test[i].expected_literal, token_literal_str);
		free(token_literal_str);
		token = token->next;
	}
	token_lstclear(&head_token);
}