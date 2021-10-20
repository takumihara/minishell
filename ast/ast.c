#include <stdlib.h>
#include "ast.h"
#include "../wrapper/x.h"

void	set_ast_nodes(t_ast_node *root, t_ast_node *left, t_ast_node *right)
{
	root->left = left;
	root->right = right;
}

void	*delete_ast_nodes(t_ast_node *node1, t_ast_node *node2)
{
	if (!node1)
		return (NULL);
	free(node1->data);
	if (node2)
		delete_ast_nodes(node2, NULL);
	delete_ast_nodes(node1->left, NULL);
	delete_ast_nodes(node1->right, NULL);
	free(node1);
	return (NULL);
}

void	new_ast_node(t_ast_node **node)
{
	*node = x_malloc(sizeof(**node));
	(*node)->left = NULL;
	(*node)->right = NULL;
	(*node)->data = NULL;
	(*node)->type = UNSET_NODE;
}

bool	assign_ast_node(t_ast_node **dst, t_ast_node *src)
{
	if (!src)
		return (false);
	*dst = src;
	return (true);
}

void	attach_ast_nodes(t_ast_node *root, t_ast_node *left, t_ast_node *right)
{
	if (left)
		root->left = left;
	if (right)
		root->right = right;
}
