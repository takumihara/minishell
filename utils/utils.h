#ifndef UTILS_H
# define UTILS_H

# include <stdlib.h>

# include "../libft/libft.h"

bool	assign_mem(void **dst, void *src);
char	*strappend(char *dst, const char *src, size_t l);
char	*strldup(const char *str, size_t size);
void	free_2d_array(void ***array);

# endif //UTILS_H
