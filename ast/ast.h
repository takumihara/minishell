#ifndef MINISHELL_AST_H
#define MINISHELL_AST_H

#include <stdbool.h>

#include "../token/token.h"

typedef struct s_tokens t_tokens;
typedef struct s_ast_node t_ast_node;

typedef enum e_node_type {
	UNSET_NODE,
	PIPE_NODE,
	AND_IF_NODE,
	OR_IF_NODE,
	COMMAND_ARG_NODE,
	REDIRECT_OUT_NODE,
	REDIRECT_IN_NODE,
	REDIRECT_APPEND_NODE,
	HEREDOC_NODE,
	FILENAME_NODE,
	HEREDOC_DELIM_NODE
}	t_node_type;

struct s_tokens {
	t_token *head;
	size_t len;
};

struct s_ast_node {
	t_node_type	type;
	t_string	*data;
	t_ast_node	*left;
	t_ast_node	*right;
};
void	set_ast_nodes(t_ast_node *root, t_ast_node *left, t_ast_node *right);
void	*delete_ast_nodes(t_ast_node *node1, t_ast_node *node2);
bool	new_ast_node(t_ast_node **node);

#endif //MINISHELL_AST_H
