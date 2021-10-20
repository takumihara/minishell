#ifndef WRAPPER_H
#define WRAPPER_H

int		ms_dup(int fildes);
int		ms_dup2(int fildes, int fildes2);
pid_t	ms_fork(void);
int		ms_pipe(int fildes[2]);

#endif //WRAPPER_H
