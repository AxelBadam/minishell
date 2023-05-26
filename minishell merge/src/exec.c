/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:06:37 by atuliara          #+#    #+#             */
/*   Updated: 2023/05/26 17:06:58 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

int check_for_parent_builtin(t_resrc *resrc, char **cmd_arr, int len)
{	
	if (!ft_strncmp(*cmd_arr, "cd", len))
 	   	return (execute_builtin_cd(resrc->list));
	else if (!ft_strncmp(*cmd_arr, "unset", len))
        return (execute_builtin_unset(resrc->list, resrc));
	else if (!ft_strncmp(*cmd_arr, "export", 6))
        return (execute_builtin_export(resrc->list, resrc));
	return (0);
}

void setup_pipe(int *fd)
{
	if (dup2(fd[1], STDOUT_FILENO) < 0)
		error_handling("pipe error");
	close(fd[1]);
	close(fd[0]);
}

void close_pipes(t_list *list, int *fd)
{
	close(fd[0]);
	if (list->command.input_fd > 2)
		close(list->command.input_fd);
	if (list->command.output_fd > 2)
		close(list->command.output_fd);
}

void execute_builtin(t_resrc *resrc, t_list *list) 
{
	char *cmd;
	int len;
	
	len = 0;
	(void)resrc; // remove later, add g_status
	cmd = *list->command.full_cmd;
	if (*list->command.full_cmd)
		len = ft_strlen(cmd);
    if (ft_strncmp(cmd, "pwd", len) == 0)
    	execute_builtin_pwd();
 	else if (ft_strncmp(cmd, "echo", len) == 0)
    	execute_builtin_echo(list->command);
	else if (ft_strncmp(cmd, "env", len) == 0)
    	execute_builtin_env(resrc->envp);
}

int is_builtin(t_command command)
{
	int		len;
	
	len = ft_strlen(*command.full_cmd);
	if (!*command.full_cmd)
		return (0);
	/*if ((*command.full_cmd && ft_strchr(*command.full_cmd, '/')) || \ 
		(command.full_path && ft_strchr(command.full_path, '/')))
		return (0);*/
	if (!ft_strncmp(*command.full_cmd, "pwd", len))
		return (1);
	if (!ft_strncmp(*command.full_cmd, "env", len))
		return (1);
	if (!ft_strncmp(*command.full_cmd, "cd", len))
		return (1);
	if (!ft_strncmp(*command.full_cmd, "export", len))
		return (1);
	if (!ft_strncmp(*command.full_cmd, "unset", len))
		return (1);
	if (!ft_strncmp(*command.full_cmd, "echo", len))
		return (1);
	if (!ft_strncmp(*command.full_cmd, "exit", len))
		return (1);
	return (0);
}

void execute_child(t_resrc *resrc, t_list *list)
{
	// signals 
	if (!is_builtin(list->command) && list->command.full_cmd)
		execve(list->command.full_path, list->command.full_cmd, resrc->envp);
	else if (is_builtin(list->command))
		execute_builtin(resrc, list);
	exit(g_exit_status);
}

int setup_redir(t_list *list)
{

	//printf("%d", list->command.input_fd);
	//printf("\n%d\n", list->command.output_fd);
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

void child_process(t_resrc *resrc, t_list *list, int *fd)
{
	setup_redir(list);
	if (list->next)
		setup_pipe(fd);
	execute_child(resrc, list);
}

void do_fork(t_resrc *resrc, t_list *list, int *fd)
{
	pid_t pid;
	
	pid = fork();
	if (pid < 0)
		error_handling("fork error");
	if (!pid)
		child_process(resrc, list, fd);
}

void exec_cmd(t_resrc *resrc, t_list *list)
{
	int fd[2];

	if (pipe(fd) < 0)
		error_handling("pipe error"); // close pipes too
	do_fork(resrc, list, fd);
	close(fd[1]);
	if (list->next && list->command.input_fd == 0)
		(list->command.input_fd = fd[0]);
	else
		close_pipes(list, fd);
}

void execution(t_resrc *resrc, t_list *list)
{	
	char **cmd_arr;
	int len;
	int lst_size; 

	lst_size = linked_list_count(&list);
	while (list)
	{
		cmd_arr = list->command.full_cmd;
		if (cmd_arr)
			len = ft_strlen(*cmd_arr);
		if (!ft_strncmp(*cmd_arr, "exit", len))
      	 	execute_builtin_exit();
		if (!list->next)
			check_for_parent_builtin(resrc, cmd_arr, len);
		//signals
		exec_cmd(resrc, list);
		list = list->next;	
	}
	wait_for_child(lst_size);
}	
