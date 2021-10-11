#include "execute.h"

bool	new_t_redirect_out(t_redirect_out **r_out, char *filename, t_node_type type)
{
	while (*r_out)
		r_out = &(*r_out)->next;
	*r_out = malloc(sizeof(**r_out));
	if (!*r_out)
		return (false);
	if (type == REDIRECT_OUT_NODE)
		(*r_out)->fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 00644);
	else
		(*r_out)->fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 00644);
	if ((*r_out)->fd == -1)
		perror("open"); //todo: check when to print error
	(*r_out)->next = NULL;
	return (true);
}

bool	new_t_redirect_in(t_redirect_in **r_in, char *data, t_node_type type)
{
	while (*r_in)
		r_in = &(*r_in)->next;
	*r_in = malloc(sizeof(**r_in));
	if (!*r_in)
		return (false);
	if (type == REDIRECT_IN_NODE)
	{
		(*r_in)->type = T_REDIRECT_IN;
		(*r_in)->fd = open(data, O_RDONLY);
		if ((*r_in)->fd == -1)
			perror("open");
		(*r_in)->next = NULL;
	}
	else if (type == HEREDOC_NODE)
	{
		(*r_in)->type = T_HEREDOC;
		(*r_in)->delim = data;
		(*r_in)->next = NULL;
	}
	return (true);
}
