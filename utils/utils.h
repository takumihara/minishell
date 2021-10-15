#ifndef UTILS_H
# define UTILS_H

# include <stdlib.h>
# include <unistd.h>

# include "../libft/libft.h"

bool	assign_mem(void **dst, void *src);
char	*strappend(char *dst, const char *src, size_t l);
char	*ft_strndup(const char *str, size_t size);
void	free_2d_array(void ***array);
char	**split_by_delims(char const *str, const char *delims);
bool	atoi_strict(const char *str, int *num);
bool	is_valid_argument(char *argv, size_t len);

# endif //UTILS_H
