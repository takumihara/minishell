#ifndef TOKEN_H
# define TOKEN_H

// AND_IF: &&
// OR_IF: ||
typedef enum e_token_type {
	ILLEGAL,
	EOL,
	IDENT,
	COMMAND,
	FILENAME,
	ASSIGN,
	PLUS,
	MINUS,
	ARGUMENT,
	PIPE,
	REDIRECT,
	HEREDOC,
	LPAREN,
	RPAREN,
	AND_IF,
	OR_IF,
	STRING,
	ENVIRONMENT,
}	t_token_type;

// todo: use struct of char* and int len
typedef struct s_token {
	enum e_token_type	type;
	char				*literal;
	// char				literal[10];
}	t_token;

#endif //TOKEN_H
