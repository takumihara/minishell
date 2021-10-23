#ifndef EXPANDER_INTERNAL_H
# define EXPANDER_INTERNAL_H

# include "../../ast/ast.h"
# include "../../libft/libft.h"
# include "../../utils/utils.h"
# include "../../execute/execute.h"
# include "../../env/env.h"
# include "../../wrapper/x.h"

# include <stdlib.h>
# include <dirent.h>

# define EXPANDABLE "\"\'$"
# define EXPANSION_DELIMITER "\"\'$|&<>() =\t\n"

# define DOUBLE_QUOTE 0
# define SINGLE_QUOTE 1
# define NOT_QUOTE 2

# define OUTSIDE 0
# define IN_DOUBLE_QUOTE 1
# define IN_SINGLE_QUOTE 2

# define NO_ERR 0
# define AMBIGUOUS_REDIRECT_ERR 1

typedef struct s_expander	t_expander;

struct s_expander {
	int			err;
	char		*err_data;
	t_env_var	*env_vars;
	int			exit_status;
};

char		*remove_quotes(char *data);

// expander_utils.c
void		new_expander(t_expander **e, t_env_var *env_vars);
int			quotation_status(char c, int status);

// expander_env.c
size_t		var_strlen(const char *str);
char		*str_insert(char *data, size_t replace_start, char *env_value, size_t env_value_len);
bool		is_expandable_env_var(char start, int status);
char		*expand_environment_variable(char *data, size_t replace_starts, t_expander *e, int status);

// expander_wildcard.c
char		*append_wildcard_strings(char *dst, char *src, const char *data);
void		quick_sort(char **array, size_t left, size_t right);
bool		is_match_pattern(const char *data, size_t len, char *name);
char		*sort_strings(char *src, char *data);
char		*expand_wildcard(char *data, size_t pre_len);

// expander_quote.c
bool		is_quote(const char c);
size_t		unquoted_strlen(const char *str);
bool		contain_quotes(const char *str);
char		*unquoted_memmove(char *dst, char *src);
int			in_quotes_type(char c, size_t count);
void		remove_null_argument(char *str);

// expander_splitting.c
char		**split_by_space_skip_quotes(char const *str, const char *delims);
void		word_splitting(t_ast_node *node, t_expander *e, char *original_data);

// expander_error.c
void		expand_redirect_error(char *original_data, t_expander *e);
t_ast_node	*handle_expand_error(t_expander *e);
bool		is_empty_data(t_expander *e, t_ast_node *node, char *original_data);

#endif
