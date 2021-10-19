#include "../execute.h"
#include "../../builtin/builtin.h"

#define BLUE    "\033[34m"      /* Blue */

void test_split_path_from_env_normal();
void test_split_path_from_env_colon();
void print_err_cnt();
void test_create_envp();

int err_cnt;

int main()
{
	test_split_path_from_env_normal();
	test_split_path_from_env_colon();
//	test_create_envp();
	print_err_cnt();
}

void test_split_path_from_env_normal()
{
	t_env_var	*env_vars;
	char		*path_from_env;
	char		**paths;
	char		**expected;

	env_vars = init_env_lst();
	if (register_env_var(ft_strdup("?"), ft_strdup("0"), &env_vars) == MALLOC_ERROR)
		exit(delete_env_lst(env_vars, NULL, NULL));
	path_from_env = get_env_value("PATH", env_vars);
	paths = split_path_from_env(path_from_env);
	expected = ft_split(path_from_env, ':');

	for (int i = 0; expected[i]; i++) {
		if (ft_strcmp(expected[i], paths[i]))
		{
			printf("expected=%s got=%s\n", expected[i], paths[i]);
			err_cnt++;
		}
	}
	free_2d_array((void ***) &paths);
	free_2d_array((void ***) &expected);
	delete_env_lst(env_vars, NULL, NULL);
	env_vars = NULL;
	path_from_env = NULL;
	paths = NULL;
	system("leaks a.out");

}

void test_split_path_from_env_colon()
{
	char *path_from_env = ft_strdup(":hello::test:test:test::");
	char *cur_path = getcwd(NULL, 0);
	char *expected[10] = {cur_path, "hello", cur_path, "test", "test", "test", cur_path, cur_path, NULL };

	char **paths = split_path_from_env(path_from_env);
	for (int i = 0; expected[i]; i++) {
		if (ft_strcmp(expected[i], paths[i]))
		{
			printf("expected=%s got=%s\n", expected[i], paths[i]);
			err_cnt++;
		}
	}
	free_2d_array((void ***) &paths);
	free(cur_path);
	free(path_from_env);
	path_from_env = NULL;
	paths = NULL;
	system("leaks a.out");
}

//void test_create_envp()
//{
//	t_executor *e;
//	char		**envp;
//	char		*expected[10] = {"key=value", NULL };
//	t_env_var	*env_vars = init_env_var("key", "value");
//	set_key_value(&key, &value, argv[i]);
//	return (BUILTIN_MALLOC_ERROR);
//	else if (register_env_var(key, value, env_vars) == MALLOC_ERROR)
//	init_env_var("second_key", "second_value");
//
//	t_env_var *p_env_vars = &env_vars;
//	new_executor(&e, NULL, &p_env_vars);
//	envp = create_envp(e);
//	for (int i = 0; envp[i]; i++) {
//		if (ft_strcmp(expected[i], envp[i]))
//		{
//			printf("expected=%s got=%s\n", expected[i], envp[i]);
//			err_cnt++;
//		}
//	}
//}

void print_err_cnt() {
	if (err_cnt == 0)
		printf(BLUE "TOTAL ERROR COUNT: 0 \n" RESET);
	else
		printf(RED "TOTAL ERROR COUNT: %d \n" RESET, err_cnt);
}

//int main(int argc, char **argv) {
//	if (argc != 2) {
//		printf("argument num wrong");
//		return(1);
//	}
//	t_token *token = lex(argv[1]);
//	t_ast_node *node = parse(token);
//	if (!node)
//	{
//		fprintf(stderr, RED "parse() returned NULL!\n" RESET);
//		return (1);
//	}
//	execute(node);
//}

