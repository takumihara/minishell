## 概要

GNU bash (version 3.2.57) の再実装です。

## デモ

![demo.gif](https://github.com/tacomeet/minishell/blob/master/gif/demo.gif?raw=true)

## インストール方法

### 必須

- GNU make (version 3.81)
- GCC (Apple clang version 13.0.0)

これらのバージョンを開発中に使用。

### Mac

1. `readline` のインストール

```bash
$ brew install readline
```

1. `minishell` のインストール

```bash
$ git clone https://github.com/tacomea/minishell.git
$ cd minishell
$ make
```

## 機能

- コマンド履歴を持つプロンプト
- `-c` オプションまたは `|` を利用したコマンドとしての実行
- ビルトインコマンド
  - `echo` : `-n` オプション
  - `cd` : 絶対／相対パス
  - `pwd` : オプションなし
  - `export` : オプションなし
  - `unset`: オプションなし
  - `env` : オプションなし
  - `exit` : オプションなし
- 引用符`'` `"` の解釈
- リダイレクト
  - `<` , `>` : 入出力リダイレクト
  - `<<` : ヒアドキュメント
  - `>>` : 追記モードでの出力リダイレクト
- パイプ `|` : 各コマンドの出力と次のコマンドの入力を接続
- サブシェル `()` : 括弧内のコマンドを別プロセスで処理
- 変数展開 `$`
  - `$(characters)` : 環境変数
  - `$?` : 最後に実行したコマンドの終了ステータス
- シグナル
  - `ctrl-C` : 改行後、新しいプロンプトを表示
  - `ctrl-D` : シェルを終了
  - `ctrl-\` : 何もしない
- 制御演算子
  - `&&` : `cmd1 && cmd2` コマンド 1 が終了ステータス 0 で終了した場合のみ、コマンド 2 を実行
  - `||` : `cmd1 || cmd2` コマンド 1 が終了ステータス 0 以外で終了した場合のみ、コマンド 2 を実行
- ワイルドカード `*` : カレントディレクトリ内でのみ動作
- 非対応
  - 閉じられていない引用符
  - `\` や `;` などの特殊な文字の解釈

## 工夫した点

### 全体

- 機能ごとにモジュールを分割し、依存関係を明確にした。各ディレクトリ内に `internal` ディレクトリを作成。外部モジュールの `internal` 内のヘッダーファイルは読み込まないようにした。

### Lexer

- 入力された文字列を解釈して各トークンに分割する際、後の Parser などでも解析しやすいように、トークンをリスト構造を用いて管理するようにした。

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
  // read_char()関数で文字列を読み進めていく。
	read_char(l);
	while (1)
	{
    // 解析した各トークンを、リストとして繋げていく。
		tmp->next = next_token(l);
		if (tmp->next->type == EOL)
			break ;
		tmp = tmp->next;
	}
	free(l);
  // 作成したリストの先頭ポインタを返し、Parserに渡す。
	return (token.next);
}
```

```c
// lexer/internal/lexer_internal.h

// t_lexer　構造体で、解析している文字列の位置などを管理。
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

// 各トークンのタイプや文字列を保持するリスト
typedef struct s_token {
	enum e_token_type	type;
	t_string			literal;
	struct s_token		*next;
}	t_token;
```

### Parser

- BNF (バッカス・ナウア記法) で文法を記した上でそれに則った実装にすることで高い拡張性を実現

```c
// parser/minishell.bnf
<command_line>	::= <pipeline> '&&' <newline> <command_line> //パターン1
		|   <pipeline> '||' <newline> <command_line> //パターン2
		|   <pipeline> //パターン3

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

	// 一つ目の要素はpipelineなので、それを読み取る
	if (!assign_ast_node(&pipeline_, pipeline(p)))
		return (NULL);
	// tokenが無ければパターン3なのでリターン
	if (p->token->type == EOL)
		return (pipeline_);
	new_ast_node(&result);
	// tokenが存在する場合は、パターン1か2で'&&','||'しかあり得ないのでそれを確認
	if (consume_token(p, AND_IF, NULL))
		result->type = AND_IF_NODE;
	else if (consume_token(p, OR_IF, NULL))
		result->type = OR_IF_NODE;
	else
	{
		p->err = ERR_UNEXPECTED_TOKEN;
		return (delete_ast_nodes(pipeline_, result));
	}
	// 右側にくるcommand lineを読み取る。
	if (!assign_ast_node(&commandline_, command_line(p)))
	{
		p->err = ERR_UNEXPECTED_EOF;
		return (delete_ast_nodes(pipeline_, result));
	}
	// 二分木の左側にpipeline, 右側にcommand lineをセットしてreturn
	set_ast_nodes(result, pipeline_, commandline_);
	return (result);
}
```

### Expander

- [Bash のアーキテクチャ](https://www.aosabook.org/en/bash.html)に沿うように、Expander の各処理を分割し、Parser で解析された各ノードを展開。

```c
// expander/expander.c

void	search_expandable_node(t_expander *e, t_ast_node *node)
{
	t_ast_node	*original_right;
	char		*original_data;

	// ノードの終端にたどり着いたらreturn
	if (!node)
		return ;
  // 各ノードを再帰的に探索し、展開処理をしていく。
	search_expandable_node(e, node->right);
	search_expandable_node(e, node->left);
　　　　// ノードが展開できない場合(&&,||など）、return
	if (node->type != COMMAND_ARG_NODE
		&& node->type != REDIRECT_IN_NODE
		&& node->type != REDIRECT_OUT_NODE
		&& node->type != REDIRECT_APPEND_NODE)
		return ;
	original_right = node->right;
	original_data = x_strdup(node->data);
　　　　// 1.変数展開
	node->data = variable_expansion(e, node->data);
	if (!is_empty_data(e, node, original_data))
	{
    // 2.文字分割
		word_splitting(node, e, original_data, original_right);
    // 3.ファイル名展開
		filename_expansion(node, e, original_data, original_right);
    // 4.クオート(', ")除去
		quotes_removal(node, original_right);
	}
	free(original_data);
}
```

- クオート`’`, `“`の内外で展開の可否が決まるため、解析している文字列の状態を管理するための関数を作成

```c
// expander/internal/expander_utils.c

int	quotation_status(char c, int status)
{
  // 今読んでいる文字が　"　の場合
	if (c == '\"')
	{
    //　既に　"　の中にいる場合は、クオートから抜け出す
		if (status == IN_DOUBLE_QUOTE)
			status = OUTSIDE;
    //　既に　'　の中にいる場合は、'の中にいる状態を維持("を解釈しない)
		else if (status == IN_SINGLE_QUOTE)
			status = IN_SINGLE_QUOTE;
    // クオートの外にいる場合は、"の中に入る
		else
			status = IN_DOUBLE_QUOTE;
	}
  // "と同様に状態を管理する
	else if (c == '\'')
	{
		if (status == IN_DOUBLE_QUOTE)
			status = IN_DOUBLE_QUOTE;
		else if (status == IN_SINGLE_QUOTE)
			status = OUTSIDE;
		else
			status = IN_SINGLE_QUOTE;
	}
　　　　// 現在の状態を返す
	return (status);
}
```

### Executor

- Parser に揃えた再帰で実装し、可読性と拡張性の向上を目指した

```c
// execute/internal/execute_command_line.c

int	execute_command_line(t_executor *e, t_ast_node *node)
{
	t_ast_node	*pipeline_node;

	// パターン1, 2('&&', '||'がある)時は、左のnodeを実行する
	if (node->type == AND_IF_NODE || node->type == OR_IF_NODE)
		pipeline_node = node->left;
	else
		pipeline_node = node;
	// 1個前の終了ステータスを確認し、実行するかをチェックする
	if (is_execute_condition(e->condition, e->exit_status))
	{
		// 全てパターンの左側のpipelineを評価・実行
		eval_pipeline(e, &e->pipeline, pipeline_node);
		execute_pipeline(e, e->pipeline);
		// 終了ステータスを設定
		register_env_var_from_literal("?", NULL, e->exit_status, e->env_vars);
		// 不必要になったnodeを再帰的に`free()`と`close()`
		delete_execute_list(e->pipeline, T_PIPELINE);
	}
	// パターン1, 2の時はconditionを設定して、右側のnodeを実行する
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

- リダイレクトが複数あった際に重複しているものは上書きするようにし、事後処理を楽にした

```c
void	new_redirect_in(t_simple_command *sc, char *data, t_node_type type)
{
		// 2個目以降のreadirect inの場合は`close()`
	if (sc->r_in != UNSET_FD)
		close(sc->r_in);
	// 新しいファイルディスクリプターで上書き
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
	// heredocの処理を行い、ファイルディスクリプターを上書き
	else if (type == HEREDOC_NODE)
		sc->r_in = execute_heredoc(data);
}
```

## 著者

- [tacomeet](https://github.com/tacomeet)
- [moromin](https://github.com/moromin)

## 参考サイト

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

## テスター

[https://github.com/nafuka11/42_minishell_tester](https://github.com/nafuka11/42_minishell_tester)

[https://github.com/ryo-manba/minishell](https://github.com/ryo-manba/minishell)
