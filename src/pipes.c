/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:38:18 by atuliara          #+#    #+#             */
/*   Updated: 2023/06/15 17:44:16 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

void setup_pipe(int *fd)
{
	if (dup2(fd[1], STDOUT_FILENO) < 0)
	{
		print_error(": dup2 error", 1, "dup2");
		exit(g_exit_status);
	}
	close(fd[1]);
}

int setup_redir(t_list *list)
{
    if (list->command.input_fd != STDIN_FILENO) 
	{
		dup2(list->command.input_fd, STDIN_FILENO); //handle error later
		close(list->command.input_fd);
    }
	if (list->command.output_fd != STDOUT_FILENO) 
	{
        dup2(list->command.output_fd, STDOUT_FILENO); // handle error later
		close(list->command.output_fd);
    }
    return 0;
}

void close_pipes(t_list *list, int *fd)
{
	if (fd[0])
		close(fd[0]);
	if (list->command.input_fd > 2)
		close(list->command.input_fd);
	if (list->command.output_fd > 2)
		close(list->command.output_fd);
}