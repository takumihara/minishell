#ifndef EXECUTE_INTERNAL_H
# define EXECUTE_INTERNAL_H

# include "../../ast/ast.h"
# include "../../env/env.h"

# define CONDITION_AND_IF 0
# define CONDITION_OR_IF 1
# define CONDITION_NL 2

# define READ 0
# define WRITE 1

# define CHILD_PROCESS 0

# define CHILD_PROCESS_NOT_CREATED 0

typedef struct s_pipeline		t_pipeline;
typedef struct s_subshell		t_subshell;
typedef struct s_compound_list	t_compound_list;
typedef struct s_simple_command	t_simple_command;
typedef struct s_redirect_out	t_redirect_out;
typedef struct s_redirect_in	t_redirect_in;

typedef enum e_list_type {
	UNSET,
	T_REDIRECT_OUT,
	T_REDIRECT_IN,
	T_SIMPLE_COMMAND,
	T_COMPOUND_LIST,
	T_SUBSHELL,
	T_PIPELINE,
}	t_list_type;

typedef enum e_simple_command_err {
	NO_ERR,
	REDIRECT_ERR,
	EXPANSION_ERR,
}	t_simple_command_err;

// todo: root may not needed
typedef struct s_executor {
	t_ast_node	*root;
	int			exit_status;
	int			condition;
	t_pipeline	*pipeline;
	t_env_var	**env_vars;
}	t_executor;

struct s_pipeline {
	void		*command;
	t_list_type	type;
	t_pipeline	*next;
};

struct s_subshell {
	t_compound_list		*compound_list;
};

struct s_compound_list {
	int				condition;
	t_pipeline		*pipeline;
	t_ast_node		*compound_list_next;
	t_compound_list	*next;
};

struct s_simple_command {
	t_ast_node				*root;
	int						argc;
	char					**argv;
	t_redirect_out			*r_out;
	t_redirect_in			*r_in;
	t_simple_command_err	err;
};

// maybe there is no need to connect these, just overwrite
struct s_redirect_out {
	int				fd;
	t_redirect_out	*next;
};

struct s_redirect_in {
	int				fd;
	t_redirect_in	*next;
};

int		init_command_line(t_executor *e, t_ast_node *node);
void	init_compound_list(t_executor *e,
			   t_compound_list **cl, t_ast_node *node);
// execute_init_utils.c
void	new_t_pipeline(t_pipeline **pipeline);
void	new_t_subshell(t_subshell **ss);
void	new_t_compound_list(t_compound_list **cl);
void	new_t_simple_command(t_simple_command **sc);
void	new_argv(t_simple_command *sc);

// execute_utils.c
void	new_executor(t_executor **e, t_ast_node *root, t_env_var **env_vars);
//void	delete_executor(t_executor **e);
//int		ex_perror(t_executor *e, const char *s);
void	delete_list(void *element, t_list_type type);
bool	execute_builtin(t_executor *e, int argc, char **argv, bool islast);
bool	is_execute_condition(int condition, int exit_status);
void	execute_redirect(t_simple_command *sc);

// execute_command.c
int		execute_pipeline(t_executor *e, t_pipeline *c);

// get_cmd_path.c
char	*get_cmd_path(t_executor *e, char *command);

// create_envp.c
char	**create_envp(t_executor *e);

// handle_exec_error.c
void	handle_exec_error(char *path, bool is_exec_run);

// new_redirect.c
void	new_redirect_out(t_simple_command *sc,
			 char *filename, t_node_type type);
void	new_redirect_in(t_simple_command *sc, char *data, t_node_type type);

// is_dir.c
bool	is_dir(const char *path);

#endif //EXECUTE_INTERNAL_H
