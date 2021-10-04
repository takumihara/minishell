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

# include <stdlib.h>

# define EXPANDABLE "\"\'$"
# define EXPANSION_DELIMITER "\"\'$|&<>() =\t\n"

typedef struct s_env_var t_env_var;

struct s_env_var {
	char	*key;
	char	*value;
	t_env_var	*next;
};

// expander.c
t_ast_node	*expand(t_ast_node *node, char **envp);

// expander_utils.c
bool		is_expandable_string(char *str, char delimiter);

// expander_env.c
size_t		var_len(const char *str);	
char		*search_env_vars(char *data, size_t var_start, t_env_var *vars);
char		*str_insert(char *data, size_t replace_start, char *env_value, size_t env_value_len);

// expander_list.c
t_env_var	*split_environment_vars(char **envp);
void		env_lstclear(t_env_var *lst);
void		set_key_value(char *envp, t_env_var *vars);
// todo: remove this
void		print_env_lst(t_env_var *vars);

// expander_wildcard.c
char		*append_wildcard_strings(char *dst, const char *src, const char *data);
bool		is_match_pattern(const char *data, size_t len, char *name);

// expander_quote.c
bool		is_quoted(const char *str);
size_t		not_quoted_strlen(const char *str);

#endif


#endif