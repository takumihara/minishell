#include "get_cmd_path.h"

static void	destroy_sep_list(t_sep_list *head)
{
	t_sep_list	*next;

	while (head)
	{
		next = head->next;
		free(head);
		head = next;
	}
}

static t_sep_list *new_sep_list(int sep_index)
{
	t_sep_list	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->sep_index = sep_index;
	return (new);
}

static char **create_paths(const char *path_from_env, t_sep_list *sep_list, size_t list_len)
{
	char	**paths;
	size_t	start;
	int		i;
	size_t	len;

	paths = malloc(sizeof(*paths) * (list_len + 1));
	if (!paths)
		return (NULL);
	start = sep_list->sep_index;
	sep_list = sep_list->next;
	i = 0;
	while (sep_list)
	{
		len = sep_list->sep_index - start - 1;
		if (len == 0)
			paths[i++] = getcwd(NULL, 0);//todo: check when can the returned value be NULL
		else
			paths[i++] = ft_substr(path_from_env, start + 1, len);
		start = sep_list->sep_index;
		sep_list = sep_list->next;
	}
	paths[i] = NULL;
	return (paths);
}

static t_sep_list *create_sep_list(const char *path_from_env, int *list_len)
{
	t_sep_list	*head;
	t_sep_list	*tail;
	int			i;

	head = new_sep_list(-1);
	if (!head)
		return (NULL);
	tail = head;
	i = -1;
	*list_len = 1;
	while (path_from_env[++i])
	{
		if (path_from_env[i] == ':')
		{
			tail->next = new_sep_list(i);
			if (!tail->next)
			{
				destroy_sep_list(head);
				return (NULL);
			}
			tail = tail->next;
			*list_len += 1;
		}
	}
	tail->next = new_sep_list(i);
	return (head);
}

char **split_path_from_env(const char *path_from_env)
{
	char		**paths;
	t_sep_list	*sep_list;
	int			list_len;

	sep_list = create_sep_list(path_from_env, &list_len);
	if (!sep_list)
		return (NULL);
	paths = create_paths(path_from_env, sep_list, list_len);
	destroy_sep_list(sep_list);
	return (paths);
}

char	*get_cmd_path(t_executor *e, char *command)
{
	char 	*path_from_env;
	char	**paths;
	char	*path;
	int		i;

	path_from_env = get_env_value("PATH", *e->env_vars);
	if (!path_from_env)
		return (ft_strdup(""));
	paths = split_path_from_env(path_from_env);
	if (!paths)
		exit(EXIT_FAILURE); //todo: free process
	if (!paths[0])
		return (ft_strdup(""));
	i = -1;
	while (1)
	{
		if (!paths[++i])
		{
			free_2d_array((void ***) &paths);
			return (NULL);
		}
		path = strjoin_three(paths[i], "/", command);
		if (access(path, F_OK) == 0)
			break ;
		free(path);
	}
	free_2d_array((void ***) &paths);
	return (path);
}