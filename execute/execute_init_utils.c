#include "execute.h"

bool new_t_pipeline(t_pipeline **pipeline)
{
	*pipeline = malloc(sizeof(**pipeline));
	if (!*pipeline)
		return (false);
	(*pipeline)->command = NULL;
	(*pipeline)->type = UNSET;
	(*pipeline)->next = NULL;
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
	(*sc)->heredoc = NULL;
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
	while (node != NULL && node->type != EOL)
	{
		if (node->type == COMMAND_ARG_NODE)
			sc->argv[i++] = node->data;
		node = node->right;
	}
	sc->argv[i] = NULL;
	return (true);
}

