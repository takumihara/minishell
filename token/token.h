#ifndef TOKEN_H
# define TOKEN_H

# include <stdio.h>
# include <stdbool.h>
// AND_IF: &&
// OR_IF: ||
typedef enum e_token_type {
	ILLEGAL,
	EOL,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	REDIRECT_APPEND,
	LPAREN,
	RPAREN,
	AND_IF,
	OR_IF,
	STRING,
	SUBSHELL_NEWLINE_MS,
	NEWLINE_MS,
}	t_token_type;

typedef struct s_string {
	char	*start;
	size_t	len;
}	t_string;

typedef struct s_token {
	enum e_token_type	type;
	t_string			literal;
	struct s_token		*next;
}	t_token;

#endif //TOKEN_H
