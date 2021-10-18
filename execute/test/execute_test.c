#include "../execute.h"
#include "../../env/env.h"
#include "../../lexer/lexer.h"
#include "../../parser/parser.h"

#define BLUE    "\033[34m"      /* Blue */

int main()
{
	t_env_var	*env_vars;
//	t_executor *e;

	env_vars = init_env_lst();
	if (register_env_var(ft_strdup("?"), ft_strdup("0"), &env_vars) == MALLOC_ERROR)
		exit(delete_env_lst(env_vars, NULL, NULL));
//	if (!new_executor(&e, NULL, &env_vars))
//		exit(EXIT_FAILURE);
	char *path_from_env = get_env_value("PATH", env_vars);
	char **paths = split_path_from_env(path_from_env);
	for (int i = 0; paths[i]; i++) {
		printf("paths[%d]: %s \n", i, paths[i]);
	}
//	get_cmd_path(e, "cat")
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

