#include "../execute.h"

#define GENERAL_CASE -1
#define ERROR_CASE -2

#define BLUE    "\033[34m"      /* Blue */

typedef struct s_test {
	t_node_type expected_type;
	int expected_level;
	char expected_literal[52];
} test;

void test_execution(char input[], test *expected, int test_type);

int main() {
	{
		char input[] = "echo -n hello > res | cat && echo success || echo failure";
		test expected[] = {
				{AND_IF_NODE,           0, ""},
				{PIPE_NODE,             1, ""},
				{COMMAND_ARG_NODE,      2, "echo"},
				{COMMAND_ARG_NODE,      3, "-n"},
				{COMMAND_ARG_NODE,      4, "hello"},
				{REDIRECT_OUT_NODE,     5, ""},
				{REDIRECT_OPERAND_NODE, 6, "res"},
				{COMMAND_ARG_NODE,      2, "cat"},
				{OR_IF_NODE,            1, ""},
				{COMMAND_ARG_NODE,      2, "echo"},
				{COMMAND_ARG_NODE,      3, "success"},
				{COMMAND_ARG_NODE,      2, "echo"},
				{COMMAND_ARG_NODE,      3, "failure"},
		};
		test_execution(input, expected, GENERAL_CASE);
	}
	// ls -l | wc -l
	// (ls -l) | wc -l
}