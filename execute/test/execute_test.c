#include "../execute.h"
#include "../../builtin/builtin.h"

#define BLUE    "\033[34m"      /* Blue */

void test_split_path_from_env_normal();
void test_split_path_from_env_colon();
void print_err_cnt();
void test_create_envp(char **expected);

int err_cnt;

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	test_split_path_from_env_normal();
	test_split_path_from_env_colon();
	test_create_envp(envp);
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
//	system("leaks a.out");

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
//	system("leaks a.out");
}

void test_create_envp(char **expected)
{
	t_executor *e;
	char		**envp;
	t_env_var	*env_vars = init_env_lst();

	new_executor(&e, NULL, &env_vars);
		printf("DEBUG \n");
	envp = create_envp(e);
	for (int i = 0; envp[i]; i++) {
		printf("%d: %s\n", i, envp[i]);
	}
//	printf("expected=%s got=%s\n", expected[0], envp[0]);
	(void)envp;
	(void)expected;
//	if (expected)
//		printf("expected[0]=%s\n", expected[0]);
//	for (int i = 0; envp[i]; i++) {
//		if (!expected[i])
//			break;
//		if (ft_strcmp(expected[i], envp[i]))
//		{
//			printf("expected=%s got=%s\n", expected[i], envp[i]);
//			err_cnt++;
//		}
//	}
}

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

