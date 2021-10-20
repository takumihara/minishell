#ifndef INTERNAL_H
# define INTERNAL_H

# include <stdbool.h>
# include <unistd.h>

# include "../../token/token.h"
# include "../../lexer/lexer.h"
# include "../../ast/ast.h"
# include "../../libft/libft.h"
# include "../../utils/utils.h"

# define ERR_UNEXPECTED_TOKEN 1
# define ERR_UNEXPECTED_EOF 2

//# define TEST

typedef struct t_parser {
	t_token	*token;
	int		err;
	bool	is_subshell;
}	t_parser;

// parser_utils.c
t_parser	*new_parser(t_token *token);
bool		consume_token(t_parser *p, t_token_type expected, t_ast_node *node);

# ifdef TEST
char		*handle_err(t_parser *p);
# else
bool		handle_err(t_parser *p);
# endif

t_ast_node	*command_line(t_parser *p);
t_ast_node	*simple_command(t_parser *p);

#endif //INTERNAL_H
