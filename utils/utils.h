#ifndef UTILS_H
# define UTILS_H

# include <stdlib.h>

# include "../libft/libft.h"

bool	assign_mem(void **dst, void *src);
char	*strappend(char *dst, const char *src, size_t l);
char	*strldup(const char *str, size_t size);
char	*ft_strcat(char *dest, char *src);
char	*ft_strstr(char *str, char *to_find);

# endif //UTILS_H
