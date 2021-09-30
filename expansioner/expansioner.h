#ifndef EXPANSIONER_H
# define EXPANSIONER_H

# include "../token/token.h"
# include "../lexer/lexer.h"
# include "../ast/ast.h"
# include "../parser/parser.h"
# include "../libft/libft.h"
# include "../utils/utils.h"

# include <stdlib.h>

# define EXPANDABLE "\"\'$"
# define EXPANSION_DELIMITER "\"\'$|&<>$() \t\n"

// expansioner.c
t_ast_node	*expansion(t_ast_node *node, char **envp);

// expansioner_utils.c
bool		is_expandable_string(char *str);
char		*cut_environment_variable(char *var_start);
t_string	*str_insert(t_string *data, char *env_var, char *env_value, size_t replace_start);

#endif
