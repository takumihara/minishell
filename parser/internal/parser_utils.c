#include "parser_internal.h"
#include "../../wrapper/x.h"

#define ERR_MSG_SYNTAX_FORMER "minishell: syntax error near unexpected token `"
#define ERR_MSG_SYNTAX_LATTER "'"
#define ERR_MSG_UNEXPECTED_EOF "minishell: syntax error: unexpected end of file"

t_parser	*new_parser(t_token *token)
{
	t_parser	*parser;

	parser = x_malloc(sizeof(t_parser));
	parser->token = token;
	parser->err = 0;
	parser->is_subshell = false;
	parser->is_first_word = true;
	return (parser);
}

// move to next token if current token was expected token type
bool	consume_token(t_parser *p, t_token_type expected, t_ast_node *node)
{
	if (p->token->type != expected)
		return (false);
	if (node)
		node->data = x_strndup(p->token->literal.start, p->token->literal.len);
	p->token = p->token->next;
	return (true);
}

#ifdef TEST

char	*handle_err(t_parser *p)
{
	char	*rtn;

	rtn = NULL;
	if (p->err == ERR_UNEXPECTED_TOKEN)
	{
		rtn = ft_strdup("minishell: syntax error near unexpected token `");
		if (p->token->type == NEWLINE_MS
			|| p->token->type == SUBSHELL_NEWLINE_MS
			|| p->token->type == EOL)
			rtn = strappend(rtn, "newline", 7);
		else
			rtn = strappend(rtn,
					p->token->literal.start, p->token->literal.len);
		rtn = strappend(rtn, "'\n", 2);
	}
	else if (p->err == ERR_UNEXPECTED_EOF)
		rtn = ft_strdup("minishell: syntax error: unexpected end of file\n");
	return (rtn);
}

#else

bool	handle_err(t_parser *p)
{
	if (p->err == ERR_UNEXPECTED_TOKEN)
	{
		ft_putstr_fd(ERR_MSG_SYNTAX_FORMER, STDERR_FILENO);
		if (p->token->type == NEWLINE_MS
			|| p->token->type == SUBSHELL_NEWLINE_MS
			|| p->token->type == EOL)
			ft_putstr_fd("newline", STDERR_FILENO);
		else
			write(STDERR_FILENO,
				  p->token->literal.start, p->token->literal.len);
		ft_putendl_fd(ERR_MSG_SYNTAX_LATTER, STDERR_FILENO);
	}
	else if (p->err == ERR_UNEXPECTED_EOF)
		ft_putendl_fd(ERR_MSG_UNEXPECTED_EOF, STDERR_FILENO);
	if (p->err)
		return (true);
	return (false);
}

#endif
