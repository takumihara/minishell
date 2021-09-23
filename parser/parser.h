#ifndef MINISHELL_PARSER_H
#define MINISHELL_PARSER_H

#include "../token/token.h"
#include "../lexer/lexer.h"
#include "../ast/ast.h"
#include "../libft/libft.h"

typedef struct t_parser {
	t_lexer *l;
	t_token *token;
}	t_parser;

// parser.c
t_node *parse(t_lexer *lexer);
t_node *parse_string(t_parser *p);
t_node *parse_builtin(t_parser *p);

// parser_utils.c
t_parser *new_parser(t_lexer *lexer);
int is_builtin(t_string *literal);
int is_delim_token(t_token_type type);

// t_string related functions
char	*st_append(char *s1, t_string const *s2);

#endif //MINISHELL_PARSER_H
