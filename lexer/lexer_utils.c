#include "lexer.h"
#include "../libft/libft.h"

void	read_char(t_lexer *lexer)
{
	if (lexer->read_position == ft_strlen(lexer->input))
	{
		lexer->ch = 0;
	}
	else if (lexer->read_position > ft_strlen(lexer->input))
		return ;
	else
	{
		lexer->ch = lexer->input[lexer->read_position];
	}
	lexer->ch = lexer->input[lexer->read_position];
	lexer->position = lexer->read_position;
	lexer->read_position++;
}

void	skip_space(t_lexer *lexer)
{
	while (ft_isspace(lexer->ch))
		read_char(lexer);
}

t_token	*new_token(t_token_type token_type, char ch)
{
	t_token	 *token;

	token = (t_token *)malloc(sizeof(t_token));
	// TODO: return NULL;
	if (!token)
		exit(EXIT_FAILURE);
	token->literal = (char *)malloc(sizeof(char) * 2);
	if (!token->literal)
		exit(EXIT_FAILURE);
	token->literal[0] = ch;
	token->literal[1] = '\0';
	token->type = token_type;
	return (token);
}

int	is_letter(char ch)
{
	return (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z'));
}

char	*read_identifier(t_lexer *lexer)
{
	size_t	position;
	char	*ident;
	size_t	len;
	size_t	i;

	position = lexer->position;
	while (is_letter(lexer->ch))
		read_char(lexer);
	len = lexer->position - position;
	ident = (char *)malloc(sizeof(char) * (len + 1));
	if (!ident)
		return (NULL);
	i = 0;
	while (i < len)
	{
		ident[i] = lexer->input[position + i];
		i++;
	}
	ident[i] = '\0';
	return (ident);
}

enum e_token_type	lookup_ident(char *ident)
{
	char	idents[2][10] = {"echo", "cat"};
	size_t	i;

	i = 0;
	while (i < 2)
	{
		if (!ft_strcmp(idents[i], ident))
			return (COMMAND);
		i++;
	}
	return (ARGUMENT);
}
