#ifndef MINISHELL_PARSER_H
#define MINISHELL_PARSER_H

#include <stdbool.h>
#include <unistd.h>

#include "../token/token.h"
#include "../lexer/lexer.h"
#include "../ast/ast.h"
#include "../libft/libft.h"
#include "../utils/utils.h"

# define ERR_UNEXPECTED_TOKEN 1
# define ERR_UNEXPECTED_EOF 2

# define EXIT_STATUS_PARSER 258

//# define TEST

typedef struct t_parser {
	t_token *token;
	int		err;
}	t_parser;

// parser.c
# ifdef TEST
void	*parse(t_token *token);
# else
t_ast_node	*parse(t_token *token);
# endif

// parser_utils.c
t_parser *new_parser(t_token *token);
bool	consume_token(t_parser *p, t_token_type expected_type, t_ast_node *node);
t_ast_node	*route_expressions(t_parser *p, t_ast_node *(*f[])(t_parser *), int element_num);

#ifdef TEST
char	*handle_err(t_parser *p);
#else
void	handle_err(t_parser *p);
#endif

// t_string related functions
char	*st_append(char *s1, t_string const *s2);

#endif //MINISHELL_PARSER_H
