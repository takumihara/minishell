#ifndef MINISHELL_PARSER_H
#define MINISHELL_PARSER_H

#include "../token/token.h"
#include "../lexer/lexer.h"
#include "../ast/ast.h"
#include "../libft/libft.h"

typedef struct t_parser {
	t_token *token;
}	t_parser;

// parser.c
t_node *parse(t_token *token);

// parser_utils.c
t_parser *new_parser(t_token *token);
int is_builtin(t_string *literal);
int is_delim_token(t_token_type type);

// t_string related functions
char	*st_append(char *s1, t_string const *s2);

#endif //MINISHELL_PARSER_H
