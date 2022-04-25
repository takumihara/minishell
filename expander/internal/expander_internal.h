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
typedef struct s_wildcard	t_wildcard;

struct s_expander {
	int			err;
	char		*err_data;
	t_env_var	*env_vars;
	int			exit_status;
};

struct s_wildcard {
	char		*data;
	t_wildcard	*prev;
	t_wildcard	*next;
};

// expander_utils.c
void		new_expander(t_expander **e, t_env_var *env_vars);
int			quotation_status(char c, int status);

// expander_env.c
char		*variable_expansion(t_expander *e, char *data);

// expander_env_utils.c
bool		is_expanded_data(char *pre_expansion_data, char *data);
bool		is_expandable_env_var(char start, int status);
size_t		var_strlen(const char *str);

// expander_wildcard.c
void		filename_expansion(t_ast_node *node, t_expander *e,
				char *original_data, const t_ast_node *original_right);

// expander_wildcard_matching_pattern.c
void		expand_matching_pattern(t_ast_node *node, char *data,
				size_t pre_len, t_ast_node *tmp_right);

// expander_wildcard_lst.c
void		append_pattern_list(t_wildcard **pattern_list, const char *data);
t_wildcard	*pattern_lstlast(t_wildcard *pattern_list);
void		delete_pattern_list(t_wildcard *pattern_list);
void		expand_node_from_pattern_list(t_ast_node *node,
				t_ast_node *tmp_right, t_wildcard *pattern_list);

// expander_wildcard_sort.c
void		sort_pattern(t_wildcard *pattern_list);

// expander_wildcard_star_utils.c
char		*remove_multi_stars(char *data);
bool		contain_expandable_star(char *data, size_t *pre_len);

// expander_wildcard_utils.c
bool		is_specified_dot_files(char *d_name, size_t len, char *data);
char		*sort_strings(char *src, char *data);
char		*strrchr_skip_quotes(char *data, int c);
bool		strncmp_skip_quotes(const char *data, char *d_name, size_t len);

// expander_remove_quote.c
void		remove_null_argument(char *str);
char		*remove_quotes(char *data);
void		quotes_removal(t_ast_node *node, t_ast_node *original_right);

// expander_quote_utils.c
int			in_quotes_type(char c, size_t count);
bool		is_quote(const char c);
size_t		unquoted_strlen(const char *str);

// expander_word_splitting.c
void		word_splitting(t_ast_node *node, t_expander *e, char *orig_data,
				const t_ast_node *original_right);

// expander_word_splitting_utils.c
size_t		skip_quotes(const char *str, char quote_type);
bool		is_delims(char c, char const *delims);

// expander_error.c
void		expand_redirect_error(char *original_data, t_expander *e);
t_ast_node	*handle_expand_error(t_expander *e);
bool		is_empty_data(t_expander *e, t_ast_node *node, char *original_data);

#endif
