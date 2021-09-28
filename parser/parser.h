#ifndef MINISHELL_PARSER_H
#define MINISHELL_PARSER_H

#include <stdbool.h>

#include "../token/token.h"
#include "../lexer/lexer.h"
#include "../ast/ast.h"
#include "../libft/libft.h"

typedef struct t_parser {
	t_token *token;
}	t_parser;

// parser.c
t_ast_node *parse(t_token *token);

// parser_utils.c
t_parser *new_parser(t_token *token);
bool	consume_token(t_parser *p, t_token_type expected_type, t_ast_node *node);

// t_string related functions
char	*st_append(char *s1, t_string const *s2);

#endif //MINISHELL_PARSER_H
