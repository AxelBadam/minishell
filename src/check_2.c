/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:36:44 by atuliara          #+#    #+#             */
/*   Updated: 2023/06/22 11:43:19 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

int	cmd_check(t_list *list)
{
	if (is_a_directory(*list->command.full_cmd))
		return (print_error(": is a directory\n", 126, *list->command.full_cmd));
	if (!is_builtin(*list->command.full_cmd) && !list->command.full_path && \
	access(*list->command.full_cmd, F_OK) == -1)
	{
		if (ft_strchr(*list->command.full_cmd, '/'))
			return (print_error \
			(": no such file or directory\n", 127, *list->command.full_cmd));
		else
			return (print_error \
			(": command not found\n", 127, *list->command.full_cmd));
	}
	else if (!is_builtin(*list->command.full_cmd) && \
	!list->command.full_path && access(*list->command.full_cmd, X_OK) == -1)
		return (print_error \
		(": permission denied\n", 127, *list->command.full_cmd));
	return (1);
}

void	check_signal(t_list *list)
{
	int	signal;

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
		write(1, "\n", 1);
		ft_putstr_fd(*list->command.full_cmd, 2);
		g_exit_status = 146;
		ft_putstr_fd(" was stopped\n", 2);
	}
}

int	check_for_option(char *str)
{
	int	ctr;

	ctr = 0;
	if (!str)
		return (0);
	if (str[ctr] && str[ctr] == '-')
	{
		ctr++;
		while (str[ctr] == 'n')
		{
			ctr++;
			if (str[ctr] == 0)
				return (1);
		}
	}
	return (0);
}
