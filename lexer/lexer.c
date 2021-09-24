#include "lexer.h"
#include "../libft/libft.h"

// inputに対して新しくlexer構造体を作成する
t_lexer *new_lexer(char *input)
{
	t_lexer *lexer;

	// todo: FREE required!!
	lexer = malloc(sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->input = input;
	lexer->position = 0;
	lexer->read_position = 0;
	return lexer;
}

// token解析のための分岐処理
t_token *next_token(t_lexer *lexer)
{
	t_token	*token;

	skip_space(lexer);
	if (lexer->ch == '|')
	{
		if (lexer->input[lexer->read_position] == '|')
		{
			token = new_token(OR_IF, lexer, 2);
			read_char(lexer);
		}
		else
			token = new_token(PIPE, lexer, 1);
	}
	else if (lexer->ch == '>')
	{
		if (lexer->input[lexer->read_position] == '>')
		{
			token = new_token(REDIRECT_APPEND, lexer, 2);
			read_char(lexer);
		}
		else
			token = new_token(REDIRECT_OUT, lexer, 1);
	}
	else if (lexer->ch == '<')
	{
		token = new_token(REDIRECT_IN, lexer, 1);
	}
	else if (lexer->ch == '&')
	{
		if (lexer->input[lexer->read_position] == '&')
		{
			token = new_token(AND_IF, lexer, 2);
			read_char(lexer);
		}
		else
			token = new_token(ILLEGAL, lexer, 1);
	}
	else if (lexer->ch == '$')
	{
		if (!ft_strchr(DELIMITER, lexer->input[lexer->read_position]))
		{
			token = new_token_environment(lexer);
			return (token);
		}
		else
			token = new_token(ILLEGAL, lexer, 1);
	}
	else
	{
		token = new_token_string(lexer);
		return (token);
	}
	read_char(lexer);
	return (token);
}

// token_listの先頭アドレスを返す
t_token	*lexer_main(t_lexer *lexer)
{
	t_token	*token;

	token = NULL;
	read_char(lexer);
	while (lexer->ch)
	{
		if (!token_lstadd_back(&token, next_token(lexer)))
		{
			// todo: token_lstclear() to free list
			token_lstclear(&token);
			return (NULL);
		}
	}
	return (token);
}
