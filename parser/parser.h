#ifndef PARSER_H
# define PARSER_H

# include "../token/token.h"
# include "../ast/ast.h"

# ifdef TEST
void		*parse(t_token *token);
# else
t_ast_node	*parse(t_token *token);
# endif

#endif //PARSER_H
