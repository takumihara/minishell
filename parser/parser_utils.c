#include "parser.h"

t_parser *new_parser(t_token *token)
{
	t_parser *parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->token = token;
	return (parser);
}

int is_builtin(t_string *literal)
{
	if (literal->len == 4 && !ft_strncmp(literal->start, "echo", literal->len))
		return (1);
	if (literal->len == 2 && !ft_strncmp(literal->start, "cd", literal->len))
		return (1);
	if (literal->len == 3 && !ft_strncmp(literal->start, "pwd", literal->len))
		return (1);
	if (literal->len == 6 && !ft_strncmp(literal->start, "export", literal->len))
		return (1);
	if (literal->len == 5 && !ft_strncmp(literal->start, "unset", literal->len))
		return (1);
	if (literal->len == 3 && !ft_strncmp(literal->start, "env", literal->len))
		return (1);
	if (literal->len == 4 && !ft_strncmp(literal->start, "exit", literal->len))
		return (1);
	return (0);
}

int is_delim_token(t_token_type type)
{
	// todo: need to talk about how to deal with ILLEGAL / ENVIRONMENT token
	if (type == ILLEGAL || type == PIPE  || type == EOL || type == REDIRECT
		|| type == ASSIGN || type == ENVIRONMENT)
		return (1);
	return (0);
}