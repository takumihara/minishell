#ifndef EXPANDER_H
#define EXPANDER_H
#ifndef expander_H
# define expander_H

# include "../token/token.h"
# include "../lexer/lexer.h"
# include "../ast/ast.h"
# include "../parser/parser.h"
# include "../libft/libft.h"
# include "../utils/utils.h"
# include "../execute/execute.h"

# include <stdlib.h>

# define EXPANDABLE "\"\'$"
# define EXPANSION_DELIMITER "\"\'$|&<>() =\t\n"

typedef struct s_expander t_expander;

struct s_expander {
	t_ast_node	*root;
	t_ast_node	*node;
};

// expander.c
t_ast_node	*expand(t_ast_node *root, char **envp);

// expander_utils.c
bool		new_expander(t_expander **e, t_ast_node *root);
bool		is_expandable_string(char *str, char delimiter);
int			expand_perror(t_expander *e, const char *s);

// expander_env.c
size_t		var_strlen(const char *str);
char		*search_env_vars(char *data, size_t var_start);
char		*str_insert(char *data, size_t replace_start, char *env_value, size_t env_value_len);

// expander_list.c
// t_env_var	*split_environment_vars(char **envp);
// void		env_lstclear(t_env_var *lst);
// void		set_key_value(char *envp, t_env_var *vars);
// // todo: remove this
// void		print_env_lst(t_env_var *vars);

// expander_wildcard.c
char		*append_wildcard_strings(char *dst, char *src, const char *data, t_expander *e);
bool		is_match_pattern(const char *data, size_t len, char *name);
char		*sort_strings(char *src, t_expander *e);

// expander_quote.c
bool		is_quote(const char c);
size_t		unquoted_strlen(const char *str);
bool		is_removable_quotes(const char *str);
char		*unquoted_memmove(char *dst, char *src);

// expander_splitting.c
char		**split_by_space_skip_quotes(char const *str, const char *delims);

#endif


#endif
