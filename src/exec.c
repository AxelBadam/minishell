/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:06:37 by atuliara          #+#    #+#             */
/*   Updated: 2023/06/14 17:18:30 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

int check_input(char **cmd_arr)
{
	int i;

	i = 0;
	while (cmd_arr[i])
	{
		if (ft_strchr(cmd_arr[i], '=') != NULL)
			{
				write(1, "invalid identifier\n", 19);
				return (0);
			}
		i++;
	}
	return (1);
}

void execute_builtin(t_resrc *resrc, t_list *list) 
{
	char *cmd;
	int len;
	
	len = 0;
	cmd = ft_strdup(*list->command.full_cmd);
	if (!cmd)
		error_exit("malloc error", resrc);
	cmd = str_to_lower(cmd);
	if (*list->command.full_cmd)
		len = ft_strlen(cmd);
    if (!ft_strncmp(cmd, "pwd", len) && len == 3)
    	execute_builtin_pwd();
 	else if (!ft_strncmp(cmd, "echo", len) && len == 4)
    	execute_builtin_echo(list->command);
	else if (!ft_strncmp(cmd, "env", len) && len == 3)
    	execute_builtin_env(resrc->envp);
	else if (!ft_strncmp(cmd, "export", 6) && len == 6)
        execute_builtin_export(resrc->list, resrc);
	free(cmd);
}

void execute_child(t_resrc *resrc, t_list *list)
{
	if (list->command.full_path)
	{
		if (execve(list->command.full_path, list->command.full_cmd, resrc->envp) == -1)
			exit(EXIT_FAILURE);
	}
	else if (is_builtin(*list->command.full_cmd))
		execute_builtin(resrc, list);
	else if (execve(*list->command.full_cmd, list->command.full_cmd, resrc->envp) == -1)
		exit(EXIT_FAILURE);
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
	execute_child(resrc, list);
	close(fd[0]);
	close(fd[1]);
	close(list->command.output_fd);
	close(list->command.input_fd);
	exit(g_exit_status);

}

void do_fork(t_resrc *resrc, t_list *list, int *fd)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	pid = fork();
	if (pid < 0)
		print_error(": error", 1, "fork");
	if (!pid)
		child_process(resrc, list, fd);
	list->command.pid = pid;
	signal(SIGINT, signal_handler);
	signal(SIGTSTP, SIG_DFL);
}

int check_for_parent_builtin(t_resrc *resrc, t_list *list, int len)
{	
	char	*tmp;
	int		ret;

	ret = 0;
	tmp = ft_strdup(*list->command.full_cmd);
	tmp = str_to_lower(tmp);
	if (!ft_strncmp(tmp, "exit", len) && len == 4)
      	execute_builtin_exit(list->command.full_cmd);
	if (!ft_strncmp(tmp, "cd", len) && len == 2)
	{
 	   	execute_builtin_cd(resrc, list->command);
		ret = 1;
	}
	else if (!ft_strncmp(tmp, "unset", len) && len == 5 && check_input(list->command.full_cmd))
	{
        execute_builtin_unset(list, resrc);
		ret = 1;
	}
	else if (!ft_strncmp(tmp, "export", 6) && len == 6)
    {    
		execute_builtin_export(list, resrc);
		ret = 1;
	}
	return (free(tmp), ret);
}

void execution(t_resrc *resrc, t_list *list)
{
    int		len;
    int		lst_size;
	t_list	*tmp;
	int 	fd[2];

	tmp = list;
    lst_size = linked_list_count(&list);
    while (list)
    {
        if (*list->command.full_cmd)
            len = ft_strlen(*list->command.full_cmd);
        if (!list->next)
            check_for_parent_builtin(resrc, list, len);
        if (cmd_check(list))
		{
			if (list->next)
			{
				if (pipe(fd) < 0)
					error_exit("pipe error", resrc);
				else if (list->next->command.input_fd == 0)
					list->next->command.input_fd = fd[0];
			}
			do_fork(resrc, list, fd);
			close(fd[1]);
		}
        list = list->next;
    }
	while (tmp)
	{
		if (tmp->command.pid != -2)
		{
			waitpid(tmp->command.pid, &g_exit_status, WUNTRACED);
			check_signal(list);
		}
		tmp = tmp->next;
	}
}
