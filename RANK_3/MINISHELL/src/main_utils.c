/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:11:10 by alex              #+#    #+#             */
/*   Updated: 2025/04/20 12:57:44 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* handle_parent_builtin function :
*	- executes a builtin command that must run in the parent process
*	- updates the LAST_EXIT environment variable with the return status
*/
static void	handle_parent_builtin(t_cmd_line *cmd_line)
{
	int		ret;
	char	*code;
	char	*tmp;

	ret = except_built_in(cmd_line->commands[0], cmd_line);
	code = ft_itoa(ret);
	tmp = ft_strjoin("LAST_EXIT=", code);
	free(code);
	update_env(&(cmd_line->envp), tmp);
	free(tmp);
}

/*
 * detect_unmatched_quote:
 *   returns '\'' if a single-quote is unclosed,
 *   '"' if a double-quote is unclosed, or 0 if none
 */
static char	detect_unmatched_quote(const char *s)
{
	bool	in_single;
	bool	in_double;

	in_single = false;
	in_double = false;
	while (*s)
	{
		if (*s == '\'' && !in_double)
			in_single = !in_single;
		else if (*s == '"' && !in_single)
			in_double = !in_double;
		s++;
	}
	if (in_single)
		return ('\'');
	if (in_double)
		return ('"');
	return (0);
}

/*
* init_cmd_line function :
*	- initializes the command line structure with paths and commands
*	- checks for unmatched quotes and prints a syntax error if found
*	- returns 1 on successful initialization, 0 on failure
*/
static int	init_cmd_line(t_cmd_line *cmd_line, char *txt, char ***envp_ptr)
{
	char	unq;

	cmd_line->commands = NULL;
	cmd_line->paths = load_paths(cmd_line->envp);
	cmd_line->n_commands = 0;
	unq = detect_unmatched_quote(txt);
	if (unq)
	{
		ft_putstr_fd(EOF_ERR, 2);
		ft_putchar_fd(unq, 2);
		ft_putstr_fd("'\n", 2);
		ft_putstr_fd(SYNTAX_LINE, 2);
		free_command_line(cmd_line);
		return (0);
	}
	split_commands(cmd_line, txt);
	if (cmd_line_syntax_analysis(txt, cmd_line, envp_ptr))
		return (0);
	if (!cmd_line->commands || !cmd_line->commands[0]
		|| !cmd_line->commands[0][0])
	{
		free_command_line(cmd_line);
		return (0);
	}
	return (1);
}

/*
* try_parent_builtin function :
*	- checks if the command is a single, parent-only builtin
*	- if so, expands tokens, executes the builtin, and updates the environment
*	- returns 1 if handled, 0 otherwise
*/
static int	try_parent_builtin(t_cmd_line *cmd_line, char ***envp_ptr)
{
	if (cmd_line->n_commands == 1
		&& is_parent_builtin(cmd_line->commands[0][0]))
	{
		expand_tokens(cmd_line->commands, cmd_line->envp);
		handle_parent_builtin(cmd_line);
		*envp_ptr = cmd_line->envp;
		free_command_line(cmd_line);
		return (1);
	}
	return (0);
}

/*
* process_cmd_line function :
*	- initializes and processes a command line input
*	- attempts to execute parent-only builtins, otherwise runs a pipeline
*	- updates the environment pointer and frees command resources
*/
void	process_cmd_line(char *txt, char ***envp_ptr)
{
	t_cmd_line	cmd_line;

	cmd_line.envp = *envp_ptr;
	if (!init_cmd_line(&cmd_line, txt, envp_ptr))
		return ;
	cmd_line.is_interrupted = 0;
	if (try_parent_builtin(&cmd_line, envp_ptr))
		return ;
	pipeline(&cmd_line);
	*envp_ptr = cmd_line.envp;
	free_command_line(&cmd_line);
}
