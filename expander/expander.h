#ifndef EXPANDER_H
# define EXPANDER_H

# include "../ast/ast.h"
# include "../env/env.h"

// expander.c
t_ast_node	*expand(t_ast_node *root, t_env_var **env_vars);

#endif
