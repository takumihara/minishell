 char *value, t_env_var *env_vars)
// {
// 	while (env_vars)
// 	{
// 		if (!strcmp(key, env_vars->key))
// 		{
// 			free(env_vars->value);
// 			env_vars->value = value;
// 			return (true);
// 		}
// 		env_vars = env_vars->next;
// 	}
// 	env_vars = init_env_var(key, value);
// 	if (!env_vars)
// 		return (false);
// 	free(key);
// 	key = NULL;
// 	return (true);
// }