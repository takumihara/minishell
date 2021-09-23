#ifndef MINISHELL_AST_H
#define MINISHELL_AST_H

#include "../token/token.h"

typedef struct s_tokens t_tokens;
typedef struct s_node t_node;

typedef enum e_node_type {
	PROGRAM,
	BUILTIN,
}	t_node_type;

struct s_tokens {
	t_token *head;
	size_t len;
};

// used vars depend on its type
// program - tokens(command, args)
// builtin - tokens(command)
struct s_node {
	t_node_type	type;
	t_tokens	*tokens;
	t_node		*next;
}

#endif //MINISHELL_AST_H
