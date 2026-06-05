/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:17:30 by jguillem          #+#    #+#             */
/*   Updated: 2025/04/20 15:17:38 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/*                                INCLUDES                                    */
/* ************************************************************************** */

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <errno.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include "libft.h"

/* ************************************************************************** */
/*                                DEFINES                                     */
/* ************************************************************************** */

static inline const char	*get_prompt(void)
{
	return ("\001\e[1;34m\002█"\
			"\001\e[1;37m\002█"\
			"\001\e[1;31m\002█"\
			"\001\e[1;36m\002 Minishell> \001\e[0m\002");
}
# define BAD_EOL ": syntax error near unexpected token `newline'\n"
# define UNKNOWN_CMD ": command not found\n"
# define HDOC_WRNG_BEG "here-document at line"
# define HDOC_WRNG_MED "delimited by end-of-file (wanted `"
# define FORBIDDEN ": Permission denied\n"
# define IS_DIR ": Is a directory\n"
# define NO_EXIST ": No such file or directory\n"
# define PIPE_ERROR ": syntax error near unexpected token `|'\n"
# define REDIR_ERROR ": syntax error near unexpected token `"
# define EOF_ERR "minishell: unexpected EOF while looking for matching `"
# define SYNTAX_LINE "minishell: syntax error: unexpected end of file\n"

/* ************************************************************************** */
/*                                STRUCTURES                                  */
/* ************************************************************************** */

typedef struct s_cmd_line
{
	char	***commands;
	char	**paths;
	char	**envp;
	int		is_interrupted;
	int		n_commands;
	pid_t	*pids;
}	t_cmd_line;

typedef struct s_cmd_meta
{
	int	output_fd;
	int	input_fd;
	int	pipefd[2];
	int	prev_fd;
	int	saved_stdin;
	int	saved_stdout;
}	t_cmd_meta;

/* ************************************************************************** */
/*                                 ERROR                                      */
/* ************************************************************************** */

void	error_handle(char *name);
void	check_command_validity(char *cmde);

/* ************************************************************************** */
/*                                 PARSING                                    */
/* ************************************************************************** */

// path.c
char	**load_paths(char **envp);
char	*find_path(char *pgme, char **paths);

// utils.c
void	free_command_line(t_cmd_line *cmd_line);
void	free_envp(char **envp);

// pipeline_utils.c
void	pipeline(t_cmd_line *cmd_line);

// pipeline.c
pid_t	*launch_pipeline(t_cmd_line *cmd_line, t_cmd_meta *cmd_meta);

// commands.c
void	split_commands(t_cmd_line *cmd_line, char *txt);

// commands_utils.c
int		execute_command(t_cmd_line *cmd_line, int i);

// handle_exec.c
char	*resolve_or_exit(t_cmd_line *cmd_line, int i);
char	*resolve_command_path(t_cmd_line *cmd_line, int i);

/* ************************************************************************** */
/*                                 TOKENIZE                                   */
/* ************************************************************************** */

// token.c
int		count_tokens(const char *line);
char	*extract_token(const char **line_ptr);
char	**tokenize_args(char *line);

// token_utils.c
void	extract_token_body(const char **str_ptr, char *token, int *i);

// expand.c
char	*expand_variables_in_token(char *token, char **envp);
void	expand_tokens(char ***commands, char **envp);

// expand_utils.c
char	*get_env_value_str(char **envp, char *key);
char	*handle_literal(const char *token, int *i, char *result);
char	*append_char(char *result, char c);
void	process_tok(char **token, char **envp);

/* ************************************************************************** */
/*                                 REDIRECT                                   */
/* ************************************************************************** */

// redirect.c
void	in_redir(t_cmd_line *cmd_line, t_cmd_meta *cmd_meta, int i, int j);
void	out_redir(t_cmd_line *cmd_line, t_cmd_meta *cmd_meta, int i, int j);
void	out_append_redir(t_cmd_line *cmd_line,
								t_cmd_meta *cmd_meta, int i, int j);

// heredoc.c
void	heredoc(t_cmd_line *cmd_line, t_cmd_meta *cmd_meta, int i, int j);

// redirect_analysis.c
void	redirect_analysis(t_cmd_line *cmd_line, t_cmd_meta *cmd_meta, int i);
int		is_redirection(char *str);
void	clean_command(t_cmd_line *cmd_line, int i);

/* ************************************************************************** */
/*                                 BUILT_IN                                   */
/* ************************************************************************** */

// except.c
int		except_built_in(char **command, t_cmd_line *cmd_line);
int		is_parent_builtin(char *cmd);

// echo.c
int		ft_echo(char **argv);

// pwd.c
int		ft_pwd(char **argv);

// exit.c
int		ft_exit(t_cmd_line *cmd_line);
void	reorder_last_exit(char ***envp);
void	update_last_exit(char ***envp, int status);

// env.c
int		ft_env(char **argv, char **envp);

// export_utils.c
int		update_env(char ***envp, char *assigment);
void	print_sorted_env(char **env);

// export.c
int		ft_export(char **args, char ***envp);

// unset.c
int		ft_unset(char **args, char ***envp);
int		is_valid_id(const char *str);

// cd.c
int		ft_cd(char **argv, char ***envp);

/* ************************************************************************** */
/*                                 SIGNAL                                     */
/* ************************************************************************** */

// signal.c
void	ignore_signals(void);
void	sighandler(int sig);

/* ************************************************************************** */
/*                                 MAIN_UTILS                                 */
/* ************************************************************************** */

//main_utils.c
void	process_cmd_line(char *txt, char ***envp_ptr);

/* ************************************************************************** */
/*                                 SYNTAX                                     */
/* ************************************************************************** */

//syntax.c
int		cmd_line_syntax_analysis(char *txt, t_cmd_line *cmd_line,
								char ***envp_ptr);

#endif
