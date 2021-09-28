#include <printf.h>
#include "lexer.h"
#include "../libft/libft.h"

void	read_char(t_lexer *lexer)
{
	if (lexer->read_position == ft_strlen(lexer->input))
		lexer->ch = 0;
	else if (lexer->read_position > ft_strlen(lexer->input))
		return ;
	else
		lexer->ch = lexer->input[lexer->read_position];
	lexer->ch = lexer->input[lexer->read_position];
	lexer->position = lexer->read_position;
	lexer->read_position++;
}

void	skip_space(t_lexer *lexer)
{
	while (ft_isspace(lexer->ch))
		read_char(lexer);
}

int	is_digit(char c)
{
	return ('0' <= c && c <= '9');
}

void	init_tokenizer(t_tokenizer *tokenizer, t_token *token)
{
	tokenizer->token = token;
	tokenizer->tokens_start = token;
	tokenizer->is_subshell = FALSE;
	tokenizer->type = ILLEGAL;
}
