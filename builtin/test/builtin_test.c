#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"
#include "../builtin.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define BLUE    "\033[34m"      /* Blue */

void test_cd(char *expected_path, int argc, char *arg1, char *arg2, char *arg3);

int main() {
	// test cd
	{
		test_cd("/obj", 2, "obj", NULL, NULL);
	}
	{
		test_cd("", 2, "", NULL, NULL);
	}
	{
		test_cd("", 2, ".", NULL, NULL);
	}
	{
		test_cd("/obj", 3, "obj", "a", NULL);
	}
	{
		test_cd("/obj", 4, "obj", "a", "b");
	}
	{
		test_cd(NULL, 1, NULL, NULL, NULL);
	}
}


void test_cd(char *expected_path, int argc, char *arg1, char *arg2, char *arg3) {
	printf("\n---------------------------------\n");
	printf("	 [cd] TEST\n");
	printf("---------------------------------\n");
	char **argv = malloc(sizeof(*argv) * 4);
	argv[0] = "cd";
	argv[1] = (arg1) ? arg1 : NULL;
	argv[2] = (arg1) ? arg2 : NULL;
	argv[3] = (arg1) ? arg3 : NULL;
	char *cwd = getcwd(NULL, 0);
	char *start_wd = getcwd(NULL, 0);
	cd(argc, argv);
	if (expected_path)
		expected_path = strappend(cwd, expected_path, strlen(expected_path));
	else
		expected_path = strdup(getenv("HOME"));
	cwd = getcwd(NULL, 0);
	if (strcmp(cwd, expected_path) != 0)
		printf(RED "cd wrong! expected=%s got=%s\n" RESET, expected_path, cwd);
	else
		printf("success!\n");
	chdir(start_wd);
	free(argv);
	free(expected_path);
	free(cwd);
	free(start_wd);
}