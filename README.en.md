*日本語版はREADMEは[こちら](https://github.com/tacomeet/minishell/blob/master/README.md)

## Summary

A reimplementation of GNU bash (version 3.2.57).

## Demo

![demo.gif](https://github.com/tacomeet/minishell/blob/master/gif/demo.gif?raw=true)

## Installation

### Requirement

- GNU make (version 3.81)
- GCC (Apple clang version 13.0.0)

Used these versions during development.

### Mac

1. installment of `readline` 

```bash
$ brew install readline
```

1. installment of `minishell`

```bash
$ git clone https://github.com/tacomea/minishell.git
$ cd minishell
$ make
```

## Features

- prompt with command history
- `-c` option or running command using `|`
- builtin
  - `echo` : `-n` option
  - `cd` :  relative or absolute path
  - `pwd` : no option
  - `export` : no option
  - `unset`: no option
  - `env` : no option
  - `exit` : no option
- quotes `'` `"` 
- redirections
  - `<` , `>` : redirect input/output
  - `<<` : here document
  - `>>` : redirect output in append mode
- pipes `|` 
- subshell `()`
- variables expansion `$`
  - `$(characters)` : environment variable
  - `$?` : exit status
- signal
  - `ctrl-C`
  - `ctrl-D`
  - `ctrl-\`
- control operator
  - `&&`
  - `||`
- wildcards `*` : only in current working directory
- unsupported
  - unclosed quotation marks
  - interpretation of special characters such as `\` and `;`

## Where we worked hard

### through project

- Split modules by their functionality and clarify dependencies. Created `internal` directory in each directory. Made sure not to include header files in `internal` of external modules.

### Lexer

- When interpreting the input string and dividing it into tokens, the tokens are managed using a linked-list so that they can be easily parsed later.

```c
// lexer/lexer.c

t_token	*lex(char *input)
{
	t_token	token;
	t_token	*tmp;
	t_lexer	*l;

	new_lexer(&l, input);
	token.next = NULL;
	tmp = &token;
  	// read_char() read through the string.
	read_char(l);
	while (1)
	{
    		// Connect each analyzed token as a list
		tmp->next = next_token(l);
		if (tmp->next->type == EOL)
			break ;
		tmp = tmp->next;
	}
	free(l);
  	// Returns the first pointer of the created list and passes it to the Parser。
	return (token.next);
}
```

```c
// lexer/internal/lexer_internal.h

// t_lexer manages the position of the string being parsed.
typedef struct s_lexer
{
	char	*input;
	size_t	position;
	size_t	read_position;
	char	ch;
	bool	is_subshell;
	bool	is_redirect;
}	t_lexer;

// token/token.h

// t_token holds the type and string of each token
typedef struct s_token {
	enum e_token_type	type;
	t_string			literal;
	struct s_token		*next;
}	t_token;
```

### Parser

- Achieved high extensibility by writing the grammar in BNF (Backus–Naur form) and implementing it accordingly.

```c
// parser/minishell.bnf
<command_line>	::= <pipeline> '&&' <newline> <command_line> //pattern 1
		|   <pipeline> '||' <newline> <command_line> //pattern 2
		|   <pipeline> //pattern 3

<pipeline>	::=  <command> '|' <newline> <pipeline>
		|    <command>

<command>	::= <subshell>
		|   <simple_command>

<subshell>	::= '(' <compound_list> ')' <redirection_list>
		|   '(' <compound_list> ')'

<compound_list>	::= <pipeline> '&&' <newline> <compound_list>
       		|   <pipeline> '||' <newline> <compound_list>
      		|   <pipeline> '\n' <newline> <compound_list>
        	|   <pipeline>

<simple_command> ::= <simple_command_element> <simple_command>
		 |   <simple_command_element>

<simple_command_element> ::= <word>
			 |   <redirection>

<redirection_list> ::= <redirection> <redirection_list>
                   |   <redirection>

<redirection>	::= <number>? '>' <word>
		|   <number>? '<' <word>
		|   <number>? '>>' <word>
		|   <number>? '<<' <word>
```

```c
// parser/internal/command_line.c

t_ast_node	*command_line(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*pipeline_;
	t_ast_node	*commandline_;

	// The first element is always a pipeline, so read it
	if (!assign_ast_node(&pipeline_, pipeline(p)))
		return (NULL);
	// If there is no token, it is pattern 3, so return
	if (p->token->type == EOL)
		return (pipeline_);
	new_ast_node(&result);
	// If a token exists, check it for pattern 1 or 2, which can only be '&&','||'.
	if (consume_token(p, AND_IF, NULL))
		result->type = AND_IF_NODE;
	else if (consume_token(p, OR_IF, NULL))
		result->type = OR_IF_NODE;
	else
	{
		p->err = ERR_UNEXPECTED_TOKEN;
		return (delete_ast_nodes(pipeline_, result));
	}
	// Read the command line.
	if (!assign_ast_node(&commandline_, command_line(p)))
	{
		p->err = ERR_UNEXPECTED_EOF;
		return (delete_ast_nodes(pipeline_, result));
	}
	// Set pipeline on the left side of the binary tree, command line on the right side, and return
	set_ast_nodes(result, pipeline_, commandline_);
	return (result);
}
```

### Expander

- Split each Expander process to follow [Bash's design](https://www.aosabook.org/en/bash.html) and expand each node parsed by Parser.

```c
// expander/expander.c

void	search_expandable_node(t_expander *e, t_ast_node *node)
{
	t_ast_node	*original_right;
	char		*original_data;

	// When reached the end of the node, return
	if (!node)
		return ;
	// Explore each node and perform the expansion process recursively.
	search_expandable_node(e, node->right);
	search_expandable_node(e, node->left);
	// If the node cannot be expanded (&&,|| etc.), return
	if (node->type != COMMAND_ARG_NODE
		&& node->type != REDIRECT_IN_NODE
		&& node->type != REDIRECT_OUT_NODE
		&& node->type != REDIRECT_APPEND_NODE)
		return ;
	original_right = node->right;
	original_data = x_strdup(node->data);
	// 1.Variable Expansion
	node->data = variable_expansion(e, node->data);
	if (!is_empty_data(e, node, original_data))
	{
    		// 2.Word Splitting
		word_splitting(node, e, original_data, original_right);
    		// 3.Filename Expansion
		filename_expansion(node, e, original_data, original_right);
    		// 4.Quotes Removal
		quotes_removal(node, original_right);
	}
	free(original_data);
}
```

- Create a function to manage the state of the string being parsed.

```c
// expander/internal/expander_utils.c

int	quotation_status(char c, int status)
{
  	// If the character you are reading is "
	if (c == '\"')
	{
   		//　If you are already in ", get out of the quote
		if (status == IN_DOUBLE_QUOTE)
			status = OUTSIDE;
    		//　If already in ', keep being in ' (do not interpret "")
		else if (status == IN_SINGLE_QUOTE)
			status = IN_SINGLE_QUOTE;
    		// If you are outside the quote, go inside the "
		else
			status = IN_DOUBLE_QUOTE;
	}
	else if (c == '\'')
	{
		if (status == IN_DOUBLE_QUOTE)
			status = IN_DOUBLE_QUOTE;
		else if (status == IN_SINGLE_QUOTE)
			status = OUTSIDE;
		else
			status = IN_SINGLE_QUOTE;
	}
	return (status);
}
```

### Executor

- Implemented Parser-aligned recursion to improve readability and extensibility

```c
// execute/internal/execute_command_line.c

int	execute_command_line(t_executor *e, t_ast_node *node)
{
	t_ast_node	*pipeline_node;

	// When patterns 1 and 2 ('&&', '||' are present), the left node is executed
	if (node->type == AND_IF_NODE || node->type == OR_IF_NODE)
		pipeline_node = node->left;
	else
		pipeline_node = node;
	// Check the exit status of the previous one to see if it runs.
	if (is_execute_condition(e->condition, e->exit_status))
	{
		// Evaluate and execute pipeline on left side
		eval_pipeline(e, &e->pipeline, pipeline_node);
		execute_pipeline(e, e->pipeline);
		// Set exit status
		register_env_var_from_literal("?", NULL, e->exit_status, e->env_vars);
		// recursively `free()` and `close()` nodes that are no longer needed
		delete_execute_list(e->pipeline, T_PIPELINE);
	}
	// When pattern 1 or 2 is used, set condition and execute the node on the right side
	if (node->type == AND_IF_NODE || node->type == OR_IF_NODE)
	{
		if (node->type == AND_IF_NODE)
			e->condition = CONDITION_AND_IF;
		else if (node->type == OR_IF_NODE)
			e->condition = CONDITION_OR_IF;
		return (execute_command_line(e, node->right));
	}
	return (e->exit_status);
}
```

- When there are multiple redirections, duplicate redirections are overwritten to ease post-processing.

```c
void	new_redirect_in(t_simple_command *sc, char *data, t_node_type type)
{
	// close() the previous one if this is the second or subsequent input redirection
	if (sc->r_in != UNSET_FD)
		close(sc->r_in);
	// Overwrite with new file descriptor
	if (type == REDIRECT_IN_NODE)
	{
		sc->r_in = open(data, O_RDONLY);
		if (sc->r_in == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(data);
			sc->err = REDIRECT_ERR;
		}
	}
	// Process here doc and overwrite file descriptor
	else if (type == HEREDOC_NODE)
		sc->r_in = execute_heredoc(data);
}
```

## Authors

- [tacomeet](https://github.com/tacomeet)
- [moromin](https://github.com/moromin)

## Reference

- The Architecture of Open Source Applications
  - [原文](https://www.aosabook.org/en/bash.html)
  - [日本語訳 (p. 53)](https://m-takagi.github.io/aosa-ja/aosa.pdf)
- [shell の文字列分解と環境変数展開を再実装した](https://qiita.com/ToYeah0102/items/4c980440ed790d110a7f)
- [bash 再実装の振り返り](https://nafuka.hatenablog.com/entry/2021/03/29/194200)
- [パイプを実装してみたというお話](https://rakiyama0229.hatenablog.jp/entry/2021/07/29/135959)
- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/html_node/index.html)
- [C 言語で glob](https://qiita.com/sassy_watson/items/03c16bd03b56e5579477)
- [gnu libc manual](https://www.gnu.org/software/libc/manual/html_node/Freeing-after-Malloc.html)
- [Go 言語でつくるインタプリタ](https://www.oreilly.co.jp/books/9784873118222/)
- [詳解UNIXプログラミング](https://www.amazon.co.jp/dp/B00KRB9U8K/ref=dp-kindle-redirect?_encoding=UTF8&btkr=1)

## Tester

[https://github.com/nafuka11/42_minishell_tester](https://github.com/nafuka11/42_minishell_tester)

[https://github.com/ryo-manba/minishell](https://github.com/ryo-manba/minishell)
