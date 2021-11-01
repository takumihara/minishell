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
# define EXPANSION_DELIMITER "\"\'$|&<>(). =\t\n"

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

// expander_utils.c
void		new_expander(t_expander **e, t_env_var *env_vars);
int			quotation_status(char c, int status);

// expander_env.c
char		*expand_environment_variable(char *data, size_t replace_starts,
				t_expander *e, int status);

// expander_wildcard.c
char		*expand_wildcard(char *data, size_t pre_len);

// expander_wildcard_star_utils.c
char		*remove_multi_stars(char *data);
bool		contain_stars_as_pattern(char *original_data);

// expander_wildcard_utils.c
bool		is_not_printable_dot_files(char *d_name, size_t len, char *data);
char		*sort_strings(char *src, char *data);
char		*append_wildcard_strings(char *dst, char *src, const char *data);
char		*strrchr_skip_quotes(char *data, int c);
bool		strncmp_skip_quotes(const char *data, char *d_name, size_t len);

// expander_remove_quote.c
void		remove_null_argument(char *str);
char		*remove_quotes(char *data);

// expander_quote_utils.c
int			in_quotes_type(char c, size_t count);
bool		is_quote(const char c);
size_t		unquoted_strlen(const char *str);

// expander_word_splitting.c
void		word_splitting(t_ast_node *node, t_expander *e, char *orig_data);

// expander_word_splitting_utils.c
size_t		skip_quotes(const char *str, char quote_type);
bool		is_delims(char c, char const *delims);

// expander_error.c
void		expand_redirect_error(char *original_data, t_expander *e);
t_ast_node	*handle_expand_error(t_expander *e);
bool		is_empty_data(t_expander *e, t_ast_node *node, char *original_data);

#endif
