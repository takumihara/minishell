#ifndef EVAL_H
# define EVAL_H

void	eval_pipeline(t_executor *e, t_pipeline **pipeline_, t_ast_node *node);
void	eval_compound_list(t_executor *e,
			   t_compound_list **cl, t_ast_node *node);

#endif //EVAL_H
