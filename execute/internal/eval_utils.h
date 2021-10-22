#ifndef EVAL_UTILS_H
# define EVAL_UTILS_H

// eval_utils.c
void	new_pipeline(t_pipeline **pipeline);
void	new_subshell(t_subshell **ss);
void	new_compound_list(t_compound_list **cl);
void	new_simple_command(t_simple_command **sc);
void	new_argv(t_simple_command *sc);

// new_redirect.c
void	new_redirect_out(t_simple_command *sc,
			 char *filename, t_node_type type);
void	new_redirect_in(t_simple_command *sc, char *data, t_node_type type);

#endif //EVAL_UTILS_H
