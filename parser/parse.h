#ifndef PARSE_H
# define PARSE_H

#include "../token/token.h"
#include "../ast/ast.h"

# ifdef TEST
void		*parse(t_token *token);
# else
t_ast_node	*parse(t_token *token);
# endif

#endif //PARSE_H
