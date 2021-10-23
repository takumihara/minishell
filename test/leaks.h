#ifndef LEAKS_H
#define LEAKS_H

# ifndef LEAKS
#  define LEAKS 0
# endif

# if LEAKS

void	end(void) __attribute__((destructor));

# endif

#endif //LEAKS_H
