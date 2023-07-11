/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:38:18 by atuliara          #+#    #+#             */
/*   Updated: 2023/07/11 12:46:55 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

void	setup_pipe(int *fd)
{
	if (dup2(fd[1], STDOUT_FILENO) < 0)
	{
		perror("dup2 error");
		g_exit_status = 1;
		exit(g_exit_status);
	}
	close(fd[1]);
}

void	setup_redir(t_list *list)
{
	if (list->command.input_fd != STDIN_FILENO)
	{
		if (dup2(list->command.input_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 error");
			g_exit_status = 1;
			exit (g_exit_status);
		}
		close(list->command.input_fd);
	}
	if (list->command.output_fd != STDOUT_FILENO)
	{
		if (dup2(list->command.output_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 error");
			g_exit_status = 1;
			exit (g_exit_status);
		}
		close(list->command.output_fd);
	}
}

void	close_pipes(t_list *list, int *fd)
{
	if (fd[0])
		close(fd[0]);
	if (list->command.input_fd > 2)
		close(list->command.input_fd);
	if (list->command.output_fd > 2)
		close(list->command.output_fd);
}
