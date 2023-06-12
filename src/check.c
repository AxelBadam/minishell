/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:27:29 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/12 13:18:13 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

int	check_pipe_syntax(char **array, int *ctr)
{
	int	error;

	error = 0;
	if (array[ctr[0]][1])
		error = 258;
	if (array[ctr[0] + 1])
		if (array[ctr[0] + 1][0] == '>' || array[ctr[0] + 1][0] == '<'
			|| array[ctr[0] + 1][0] == '|')
			error = 258;
	if (error)
		if (!print_error("syntax error near unexpected token `|'\n", 258, NULL))
			return (0);
	return (1);
}

int	check_syntax(char **array, int *ctr, char d)
{
	int	error;

	error = 0;
	if (d == '|')
		if (!check_pipe_syntax(array, ctr))
			return (0);
	if ((array[ctr[0]][1] && array[ctr[0]][1] != d) || array[ctr[0]][2])
		return (0);
	if (array[ctr[0] + 1])
	{
		if (array[ctr[0] + 1][0] == '>' || array[ctr[0] + 1][0] == '<'
			|| array[ctr[0] + 1][0] == '|')
		{
			print_error("syntax error near unexpected token `>'\n", 258, NULL);
			return (0);
		}
	}
	return (1);
}

int	is_builtin(char *str)
{
	char	*tmp;
	int		ctr;
	int		re;

	tmp = ft_strdup(str);
	ctr = -1;
	re = 0;
	while (tmp[++ctr])
		tmp[ctr] = ft_tolower(tmp[ctr]);
	if (!ft_strncmp(tmp, "pwd", ctr) && ctr == 3)
		re = 1;
	else if (!ft_strncmp(tmp, "env", ctr) && ctr == 3)
		re = 1;
	else if (!ft_strncmp(tmp, "cd", ctr) && ctr == 2)
		re = 1;
	else if (!ft_strncmp(tmp, "export", ctr) && ctr == 6)
		re = 1;
	else if (!ft_strncmp(tmp, "unset", ctr) && ctr == 5)
		re = 1;
	else if (!ft_strncmp(tmp, "echo", ctr) && ctr == 4)
		re = 1;
	else if (!ft_strncmp(tmp, "exit", ctr) && ctr == 4)
		re = 1;
	free(tmp);
    return (re);
}

void check_signal(t_list *list)
{
	int signal;
	
	signal = 0;
	if (WIFEXITED(g_exit_status))
		g_exit_status = WEXITSTATUS(g_exit_status);
	else if (WIFSIGNALED(g_exit_status))
		{
			signal = WTERMSIG(g_exit_status);
			if (signal == 3)
				ft_putstr_fd("Quit: 3\n", 2);
			g_exit_status = 128 + signal;
		}
	else if (WIFSTOPPED(g_exit_status))
	{
		write(STDOUT_FILENO, "\r\033[K", 4);
		ft_putstr_fd(*list->command.full_cmd, 2);
		g_exit_status = 146;
		ft_putstr_fd(" was stopped\n", 2);
	}
}

int cmd_check(t_list *list)
{
	if (is_a_directory(*list->command.full_cmd))
		return(print_error(": is a directory\n", 126, *list->command.full_cmd));
	if (!is_builtin(*list->command.full_cmd) && !list->command.full_path &&\
	access(*list->command.full_cmd, F_OK) == -1)
	{
		if (ft_strchr(*list->command.full_cmd, '/'))
			return(print_error(": no such file or directory\n", 127, *list->command.full_cmd));
		else
			return(print_error(": command not found\n", 127, *list->command.full_cmd));
	}
	else if (!is_builtin(*list->command.full_cmd) && !list->command.full_path &&\
	access(*list->command.full_cmd, X_OK) == -1)
		return(print_error(": permission denied\n", 127, *list->command.full_cmd));
	return (1);
}
