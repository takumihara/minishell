#ifndef EXECUTE_H
# define EXECUTE_H

# include "../ast/ast.h"
# include "../env/env.h"

int		execute(t_ast_node *root, t_env_var **env_vars, bool is_interactive);

#endif //EXECUTE_H
