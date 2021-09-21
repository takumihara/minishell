#ifndef TOKEN_H
# define TOKEN_H

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
}	t_token_type;

typedef struct s_token {
	enum e_token_type	type;
	char				*literal;
	// char				literal[10];
}	t_token;

#endif //TOKEN_H
