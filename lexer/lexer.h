#ifndef LEXER_H
# define LEXER_H

# include "../token/token.h"
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
// opendir(), readdir() -> wildcard
# include <sys/types.h>
# include <dirent.h>
// todo: remove this
# include "printf.h"

# define DELIMITER "|&<>=$() "

// todo: need to discuss if read_position is actually needed?
// todo: type of positions are different from the book
// position: current character
// read_point: next character
// ch: currently investigated character
typedef struct s_lexer
{
	char	*input;
	size_t	position;
	size_t	read_position;
	char	ch;
}	t_lexer;

typedef struct  s_tokenizer{
    // size_t			str_i;//入力値のindex
    // size_t			tok_i;//token内の文字列のindex
    // size_t			str_len;//入力値の長さ
    // bool			esc_flag;//エスケープを行うか
    // bool			is_quoted;//現在クオートの中にいるか
	bool			is_subshell; // 現在()の中にいるか
    t_token			*token;//現在操作しているトークン
    t_token			*tokens_start;//トークンリストの先頭
    t_token_type	type;//何のトークンであるか
    // char            *quote_start; //クオートの開始地点
}               t_tokenizer;

typedef int	t_bool;
# define FALSE 0
# define TRUE 1

// lexer.c
t_lexer	*new_lexer(char *input);
t_token	*next_token(t_lexer *lexer, t_tokenizer *tokenizer);
t_token	*lexer_main(t_lexer *lexer);

// lexer_utils.c
void	read_char(t_lexer *lexer);
void	skip_space(t_lexer *lexer);
int		is_digit(char c);
void	init_tokenizer(t_tokenizer *tokenizer, t_token *token);

// lexer_new_token.c
t_token	*new_token(t_token_type token_type, t_lexer *lexer, size_t len);
t_token	*new_token_string(t_lexer *lexer);
t_token	*new_token_environment(t_lexer *lexer);
t_token	*new_token_redirect_or_string(t_lexer *lexer);

// lexer_list.c
int		token_lstadd_back(t_token **lst, t_token *new);
t_token	*token_lstlast(t_token *lst);
void	token_lstclear(t_token **lst);

#endif //LEXER_H
