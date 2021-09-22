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
	LPAREN,
	RPAREN,
	AND_IF,
	OR_IF,
	STRING,
	ENVIRONMENT,
}	t_token_type;

// todo: need to discuss how to hold the value of literal
// one idea is char * and malloc
// second idea is array and error message "argument is too long"
// third is use struct of char* and int len
typedef struct s_token {
	enum e_token_type	type;
//	char				*literal;
	char				literal[10];
}	t_token;

#endif //TOKEN_H
