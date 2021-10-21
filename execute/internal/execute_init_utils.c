#include "../execute.h"
#include "../../wrapper/x.h"
#include "execute_internal.h"

void	new_t_pipeline(t_pipeline **pipeline)
{
	*pipeline = x_malloc(sizeof(**pipeline));
	(*pipeline)->command = NULL;
	(*pipeline)->type = UNSET;
	(*pipeline)->next = NULL;
}

void	new_t_subshell(t_subshell **ss)
{
	*ss = x_malloc(sizeof(**ss));
	(*ss)->compound_list = NULL;
}

void	new_t_compound_list(t_compound_list **cl)
{
	*cl = x_malloc(sizeof(**cl));
	(*cl)->pipeline = NULL;
	(*cl)->compound_list_next = NULL;
	(*cl)->next = NULL;
}

void	new_t_simple_command(t_simple_command **sc)
{
	*sc = x_malloc(sizeof(**sc));
	(*sc)->root = NULL;
	(*sc)->argc = 0;
	(*sc)->argv = NULL;
	(*sc)->r_out = NULL;
	(*sc)->r_in = NULL;
	(*sc)->err = NO_ERR;
}

void	new_argv(t_simple_command *sc)
{
	t_ast_node	*node;
	int			i;

	node = sc->root;
	sc->argv = x_malloc(sizeof(*sc->argv) * (sc->argc + 1));
	i = 0;
	while (node != NULL)
	{
		if (node->type == COMMAND_ARG_NODE)
			sc->argv[i++] = node->data;
		node = node->right;
	}
	sc->argv[i] = NULL;
}