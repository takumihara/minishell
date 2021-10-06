#include "execute.h"

static int command_line(t_executor *e, t_ast_node *node);
static t_pipeline *pipeline(t_executor *e, t_ast_node *node);
static t_subshell *subshell(t_executor *e, t_ast_node *node);
t_compound_list *init_compound_list(t_executor *e, t_ast_node *node);
static t_simple_command *simple_command(t_executor *e, t_ast_node *node);

int	execute(t_ast_node *root)
{
	t_executor	*e;
	int			exit_status;

	if (!root)
		return (EXIT_FAILURE);
	if (!new_executor(&e, root))
		return (ex_perror(NULL, "malloc"));
	command_line(e, root);
	exit_status = e->exit_status;
	delete_ast_nodes(e->root, NULL);
	free(e);
	return (exit_status);
}

int command_line(t_executor *e, t_ast_node *node)
{
	if (node->type == AND_IF_NODE || node->type == OR_IF_NODE)
	{
		if (is_execute_condition(e->condition, e->exit_status))
			e->exit_status = execute_pipeline(e, pipeline(e, node->left));
		if (node->type == AND_IF_NODE)
			e->condition = CONDITION_AND_IF;
		else if (node->type == OR_IF_NODE)
			e->condition = CONDITION_OR_IF;
		return (command_line(e, node->right));
	}
	else
	{
		e->exit_status = execute_pipeline(e, pipeline(e, node));
		return (e->exit_status);
	}
}

t_pipeline	*pipeline(t_executor *e, t_ast_node *node)
{
	t_pipeline *pipeline_;
	t_pipeline *pipeline_next;

	pipeline_next = NULL;
	// expected node: COMMAND_ARG_NODE, REDIRECT*, PIPE_NODE, SUBSHELL
	if (!new_t_pipeline(&pipeline_))
		exit(ex_perror(e, "malloc"));
	if (node->type == PIPE_NODE)
	{
		pipeline_next = pipeline(e, node->right);
		node = node->left;
	}
	if (node->type == SUBSHELL_NODE)
	{
		pipeline_->command = (void *)subshell(e, node);
		pipeline_->type = T_SUBSHELL;
	}
	else
	{
		pipeline_->command = (void *)simple_command(e, node);
		pipeline_->type = T_SIMPLE_COMMAND;
	}
	if (pipeline_next)
		pipeline_->next = pipeline_next;
	return (pipeline_);
}

t_subshell	*subshell(t_executor *e, t_ast_node *node)
{
	t_subshell *ss;

	if (!new_t_subshell(&ss))
		exit(ex_perror(e, "malloc"));
	ss->compound_list = init_compound_list(e, node->left);
	while (node->right != NULL)
	{
		node = node->right;
		if (node->type == REDIRECT_OUT_NODE)
		{
			if (!new_t_redirect_out(&ss->r_out, node->data, false))
				exit(ex_perror(e, "malloc"));
		}
		else if (node->type == REDIRECT_IN_NODE)
		{
			if (!new_t_redirect_in(&ss->r_in, node->data))
				exit(ex_perror(e, "malloc"));
		}
		else if (node->type == REDIRECT_APPEND_NODE)
		{
			if (!new_t_redirect_out(&ss->r_out, node->data, true))
				exit(ex_perror(e, "malloc"));
		}
		else if (node->type == HEREDOC_NODE)
		{
			if (!new_t_heredoc(&ss->heredoc, node->data))
				exit(ex_perror(e, "malloc"));
		}
	}
	return (ss);
}

t_compound_list *init_compound_list(t_executor *e, t_ast_node *node)
{
	t_compound_list	*cl;

	if (!new_t_compound_list(&cl))
		exit (ex_perror(e, "malloc"));
	if (node->type == AND_IF_NODE)
	{
		cl->pipeline = pipeline(e, node->left);
		cl->condition = CONDITION_AND_IF;
		cl->compound_list_next = node->right;
	}
	else if (node->type == OR_IF_NODE)
	{
		cl->pipeline = pipeline(e, node->left);
		cl->condition = CONDITION_OR_IF;
		cl->compound_list_next = node->right;
	}
	else if (node->type == SUBSHELL_NEWLINE_NODE)
	{
		cl->pipeline = pipeline(e, node->left);
		cl->condition = CONDITION_NL;
		cl->compound_list_next = node->right;
	}
	else
		cl->pipeline = pipeline(e, node);
	return (cl);
}

t_simple_command *simple_command(t_executor *e, t_ast_node *node)
{
	t_simple_command	*sc;

	if (!new_t_simple_command(&sc))
		exit(ex_perror(e, "malloc"));
	sc->root = node;
	// expected node: COMMAND_ARG_NODE, REDIRECT*
	while (node != NULL)
	{
		if (node->type == COMMAND_ARG_NODE)
			sc->argc++;
		else if (node->type == REDIRECT_OUT_NODE)
		{
			if (!new_t_redirect_out(&sc->r_out, node->data, false))
				exit(ex_perror(e, "malloc"));
		}
		else if (node->type == REDIRECT_IN_NODE)
		{
			if (!new_t_redirect_in(&sc->r_in, node->data))
				exit(ex_perror(e, "malloc"));
		}
		else if (node->type == REDIRECT_APPEND_NODE)
		{
			if (!new_t_redirect_out(&sc->r_out, node->data, true))
				exit(ex_perror(e, "malloc"));
		}
		else if (node->type == HEREDOC_NODE)
		{
			if (!new_t_heredoc(&sc->heredoc, node->data))
				exit(ex_perror(e, "malloc"));
		}
		node = node->right;
	}
	if (!new_argv(sc))
		exit(ex_perror(e, "malloc"));
	return (sc);
}
