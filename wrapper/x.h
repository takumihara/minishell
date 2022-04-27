#ifndef X_H
# define X_H

# include <sys/stat.h>
# include <stdlib.h>
# include <dirent.h>
# include "../utils/utils.h"

int				x_dup(int fildes);
int				x_dup2(int fildes, int fildes2);
pid_t			x_fork(void);
int				x_pipe(int fildes[2]);
void			*x_malloc(size_t size);
char			*x_substr(char const *s, unsigned int start, size_t len);
char			*x_strdup(const char *str);
int				x_stat(const char *restrict path, struct stat *restrict buf);
DIR				*x_opendir(const char *str);
struct dirent	*x_readdir(DIR *dirp);
void			x_closedir(DIR *dir);
char			**x_split(const char *s, char c);
char			*x_strndup(const char *str, size_t size);
char			*x_itoa(int n);
void			x_signal(int sig, void (*f)(int));

#endif
