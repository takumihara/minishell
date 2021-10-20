#ifndef WRAPPER_H
#define WRAPPER_H

int		x_dup(int fildes);
int		x_dup2(int fildes, int fildes2);
pid_t	x_fork(void);
int		x_pipe(int fildes[2]);
void	*x_malloc(size_t size);
int		x_get_next_line(int fd, char **line);
char	*x_substr(char const *s, unsigned int start, size_t len);
char	*x_strdup(const char *str);

#endif //WRAPPER_H
