#include "execute.h"

bool	new_t_redirect_out(t_redirect_out **r_out, char *filename, bool append)
{
	while (*r_out)
		r_out = &(*r_out)->next;
	*r_out = malloc(sizeof(**r_out));
	if (!*r_out)
		return (false);
	if (append)
		(*r_out)->fd = open(filename, O_APPEND | O_CREAT, 00644);
	else
		(*r_out)->fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 00644);
	if ((*r_out)->fd == -1)
		perror("open"); //todo: check when to print error
	(*r_out)->append = append;
	(*r_out)->next = NULL;
	return (true);
}

bool	new_t_redirect_in(t_redirect_in **r_in, char *filename)
{
	while (*r_in)
		r_in = &(*r_in)->next;
	*r_in = malloc(sizeof(**r_in));
	if (!*r_in)
		return (false);
	(*r_in)->fd = open(filename, O_RDONLY);
	if ((*r_in)->fd == -1)
		perror("open");
	(*r_in)->next = NULL;
	return (true);
}

bool	new_t_heredoc(t_heredoc **heredoc, char *doc)
{
	while (*heredoc)
		heredoc = &(*heredoc)->next;
	*heredoc = malloc(sizeof(**heredoc));
	if (!*heredoc)
		return (false);
	(*heredoc)->doc = doc;
	(*heredoc)->next = NULL;
	return (true);
}