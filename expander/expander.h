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
# include "../env/env.h"

# include <stdlib.h>

# define EXPANDABLE "\"\'$"
# define EXPANSION_DELIMITER "\"\'$|&<>() =\t\n"

# define DOUBLE_QUOTE 0
# define SINGLE_QUOTE 1
# define NOT_QUOTE 2

# define OUTSIDE 0
# define IN_DOUBLE_QUOTE 1
# define IN_SINGLE_QUOTE 2

typedef struct s_expander t_expander;

extern char	**environ;

struct s_expander {
	t_ast_node	*root;
	t_ast_node	*node;
	t_env_var	*env_vars;
};

// expander.c
t_ast_node	*expand(t_ast_node *root, t_env_var **env_vars);

// expander_utils.c
bool		new_expander(t_expander **e, t_ast_node *root, t_env_var *env_vars);
bool		is_expandable_string(char *str, char delimiter);
int			expand_perror(t_expander *e, const char *s);
void		*expand_redirect_error(char *original_data);

// expander_env.c
size_t		var_strlen(const char *str);
char		*get_env_value(char *key, t_env_var *env_var);
char		*str_insert(char *data, size_t replace_start, char *env_value, size_t env_value_len);

// expander_wildcard.c
char		*append_wildcard_strings(char *dst, char *src, const char *data, t_expander *e);
void		quick_sort(char **array, size_t left, size_t right);
bool		is_match_pattern(const char *data, size_t len, char *name);
char		*sort_strings(char *src, t_expander *e);

// expander_quote.c
bool		is_quote(const char c);
size_t		unquoted_strlen(const char *str);
bool		is_contain_quotes(const char *str);
char		*unquoted_memmove(char *dst, char *src);
int			in_quotes_type(char c, size_t count);

// expander_splitting.c
char		**split_by_space_skip_quotes(char const *str, const char *delims);

#endif


#endif
