#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include "../../push_swap/libft/get_next_line.h"
# include "../../push_swap/libft/list.h"
# include "../../push_swap/push_swap.h"

int				ft_atoi(char *str);
size_t			ft_strlen(char const *str);
void			*ft_memmove(void *dst, const void *src, size_t len);
int				ft_strncmp(const char *str1, const char *str2, size_t n);
int				put_err(char *msg);
void			*assign_mem(void **dst, void *src);

char			*get_next_line(int fd);

// ft_lstnew: initialize a new node with its content being 'content'
t_list			*ft_lstnew(int content);
// ft_lstadd_front: add a new node represented by 'node'
// into the last place of list
// ('lst' doesn't have to be the head node, one of the nodes of the list)
t_list			*ft_lstadd_back(t_list **lst, t_list *node);
// ft_lstlast: detect the last node
t_list			*ft_lstlast(t_list *lst);
// ft_lstfirst: detect the first node
t_list			*ft_lstfirst(t_list *lst);
// ft_lstadd_front: add a new node represented by 'node'
// into the first place of list
// ('lst' doesn't have to be the head node, one of the nodes of the list)
void			ft_lstadd_front(t_list **lst, t_list *node);
// ft_lstsize: return the number of nodes in the stack
int				ft_lstsize(t_list *lst);
void			ft_lstdelone(t_list *lst, void (*del)(int));
void			ft_lstclear(t_list **lst, void (*del)(int));

#endif //LIBFT_H
