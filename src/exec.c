/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:06:37 by atuliara          #+#    #+#             */
/*   Updated: 2023/06/15 17:47:54 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

void execute_child(t_resrc *resrc, t_list *list)
{
	if (list->command.full_path)
	{
		if (execve(list->command.full_path, list->command.full_cmd, resrc->envp) == -1)
			exit(EXIT_FAILURE);
	}
	else if (is_builtin(*list->command.full_cmd))
		execute_builtin(resrc, list);
	else
		execve(*list->command.full_cmd, list->command.full_cmd, resrc->envp);
}

void child_process(t_resrc *resrc, t_list *list, int *fd)
{
	signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	setup_redir(list);
	if (list->next && list->command.output_fd == STDOUT_FILENO)
		setup_pipe(fd);
	close(fd[0]);
	execute_child(resrc, list);
	exit(g_exit_status);
}

void do_fork(t_resrc *resrc, t_list *list, int *fd)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	pid = fork();
	if (pid < 0)
		print_error(": fork error", 1, "fork");
	if (!pid)
		child_process(resrc, list, fd);
	list->command.pid = pid;
}

void exec_cmd(t_resrc *resrc, t_list *list)
{
	if (list->next)
	{
		if (pipe(list->command.fd) < 0)
		{
			close_pipes(list, list->command.fd);
			error_handling("pipe error");
		}
		if (list->next->command.input_fd == 0)
			list->next->command.input_fd = list->command.fd[0];
	}
	do_fork(resrc, list, list->command.fd);
	close(list->command.fd[1]);
}

void execution(t_resrc *resrc, t_list *list)
{
    int		len;

    while (list)
    {
        if (list->command.full_cmd)
            len = ft_strlen(*list->command.full_cmd);
        if (!list->next)
            check_for_parent_builtin(resrc, list, len);
        if (cmd_check(list))
            exec_cmd(resrc, list);
        list = list->next;
    }
	close_wait(resrc->list);
	signal(SIGINT, signal_handler);
	signal(SIGTSTP, SIG_DFL);
}
