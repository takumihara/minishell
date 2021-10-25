#include "../execute.h"
#include "../builtin/builtin.h"
#include "../internal/split_path_from_env.h"
#include "../internal/execute_internal.h"

#define RESET   "\033[0m"
#define BLUE    "\033[34m"      /* Blue */
#define RED     "\033[31m"

void test_split_path_from_env_normal(t_env_var *env_vars);
void test_split_path_from_env_colon();
void print_err_cnt();
void test_create_envp(char **expected, t_env_var *env_vars);

int err_cnt;

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_env_var	*env_vars = init_env_lst();
	register_env_var_from_literal("?", NULL, 0, &env_vars);
	test_split_path_from_env_normal(env_vars);
	test_split_path_from_env_colon();
	test_create_envp(envp, env_vars);
	print_err_cnt();
}

void test_split_path_from_env_normal(t_env_var *env_vars)
{
	char		*path_from_env;
	char		**paths;
	char		**expected;


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
	path_from_env = NULL;
	paths = NULL;
	system("leaks a.out");
}

void test_split_path_from_env_colon()
{
	char *path_from_env = ft_strdup(":hello::test:test:test::");
	char *expected[10] = {".", "hello", ".", "test", "test", "test", ".", ".", NULL };

	char **paths = split_path_from_env(path_from_env);
	for (int i = 0; expected[i]; i++) {
		if (ft_strcmp(expected[i], paths[i]))
		{
			printf("expected=%s got=%s\n", expected[i], paths[i]);
			err_cnt++;
		}
	}
	free_2d_array((void ***) &paths);
	free(path_from_env);
	path_from_env = NULL;
	paths = NULL;
	system("leaks a.out");
}

void test_create_envp(char **expected, t_env_var *env_vars)
{
	t_executor *e;
	char		**envp;

	new_executor(&e, &env_vars, NULL, false);
	envp = create_envp(e);
	for (int i = 0; expected[i] && envp[i]; i++) {
		if (ft_strcmp(expected[i], envp[i]))
		{
			printf("expected=%s got=%s\n", expected[i], envp[i]);
			err_cnt++;
		}
	}
	system("leaks a.out");
}

void print_err_cnt() {
	if (err_cnt == 0)
		printf(BLUE "TOTAL ERROR COUNT: 0 \n" RESET);
	else
		printf(RED "TOTAL ERROR COUNT: %d \n" RESET, err_cnt);
}

//int main(int argc, char **argv) {
//	t_env_var	*env_vars;
//
//	env_vars = init_env_lst();
//	if (register_env_var_from_literal("?", "0", 0, &env_vars) == MALLOC_ERROR)
//		exit(delete_env_lst(env_vars, NULL, NULL));
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
//	execute(node, &env_vars);
//}

