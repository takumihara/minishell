#include "execute.h"

bool	new_t_pipeline(t_pipeline **pipeline)
{
	*pipeline = malloc(sizeof(**pipeline));
	if (!*pipeline)
		return (false);
	(*pipeline)->command = NULL;
	(*pipeline)->type = UNSET;
	(*pipeline)->next = NULL;
	return (true);
}

bool	new_t_subshell(t_subshell **ss)
{
	*ss = malloc(sizeof(**ss));
	if (!*ss)
		return (false);
	(*ss)->compound_list = NULL;
	(*ss)->r_out = NULL;
	(*ss)->r_in = NULL;
	return (true);
}

bool	new_t_compound_list(t_compound_list **cl)
{
	*cl = malloc(sizeof(**cl));
	if (!*cl)
		return (false);
	(*cl)->exit_status = -1; //idk what should be the default
	(*cl)->condition = -1;
	(*cl)->pipeline = NULL;
	(*cl)->compound_list_next = NULL;
	return (true);
}

bool	new_t_simple_command(t_simple_command **sc)
{
	*sc = (t_simple_command *) malloc(sizeof(**sc));
	if (!(*sc))
		return (false);
	(*sc)->root = NULL;
	(*sc)->argc = 0;
	(*sc)->argv = NULL;
	(*sc)->r_out = NULL;
	(*sc)->r_in = NULL;
	(*sc)->err = NO_ERR;
	return (true);
}

bool	new_argv(t_simple_command *sc)
{
	t_ast_node	*node;
	int			i;

	node = sc->root;
	// todo: free required!
	sc->argv = malloc(sizeof(*sc->argv) * (sc->argc + 1));
	if (!sc->argv)
		return (false);
	i = 0;
	while (node != NULL)
	{
		if (node->type == COMMAND_ARG_NODE)
			sc->argv[i++] = node->data;
		node = node->right;
	}
	sc->argv[i] = NULL;
	return (true);
}

