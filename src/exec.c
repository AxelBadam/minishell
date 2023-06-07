/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:06:37 by atuliara          #+#    #+#             */
/*   Updated: 2023/06/07 17:45:06 by atuliara         ###   ########.fr       */
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

void close_pipes(t_list *list, int *fd)
{
	if (fd[0])
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
	cmd = ft_strdup(*list->command.full_cmd);
	if (!cmd)
		return ; // g_states
	cmd = str_to_lower(cmd);
	if (*list->command.full_cmd)
		len = ft_strlen(cmd);
    if (!ft_strncmp(cmd, "pwd", len))
    	execute_builtin_pwd();
 	else if (!ft_strncmp(cmd, "echo", len))
    	execute_builtin_echo(list->command);
	else if (!ft_strncmp(cmd, "env", len))
    	execute_builtin_env(resrc->envp);
	else if (!ft_strncmp(cmd, "export", 6))
        execute_builtin_export(resrc->list, resrc);
	free(cmd);
}

void execute_child(t_resrc *resrc, t_list *list)
{
	// signals
	if (list->command.full_path)
		execve(list->command.full_path, list->command.full_cmd, resrc->envp);
	else if (is_builtin(*list->command.full_cmd))
		execute_builtin(resrc, list);
	else
		execve(*list->command.full_cmd, list->command.full_cmd, resrc->envp);
	exit(g_exit_status);
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

void setup_pipe(int *fd)
{
	if (dup2(fd[1], STDOUT_FILENO) < 0)
	{
		print_error(": dup2 error", 1, "dup2");
		exit(g_exit_status);
	}
	close(fd[1]);
	close(fd[0]);
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
		print_error(": fork error", 1, "fork");
	if (!pid)
		child_process(resrc, list, fd);
}

void exec_cmd(t_resrc *resrc, t_list *list)
{
	int fd[2];

	if (pipe(fd) < 0)
	{
		close_pipes(list, fd);
		print_error(": error creating pipe", -1, "pipe");
		exit(g_exit_status); // exit gracefully?
	}
	do_fork(resrc, list, fd);
	close(fd[1]);
	if (list->next && list->next->command.input_fd == 0)
		list->next->command.input_fd = fd[0];
	else
		close_pipes(list, fd);
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


void check_for_parent_builtin(t_resrc *resrc, t_list *list, char **cmd_arr, int len)
{	
	char	*tmp;

	tmp = ft_strdup(*cmd_arr);
	tmp = str_to_lower(tmp);
	if (!ft_strncmp(str_to_lower(tmp), "exit", len))
      	execute_builtin_exit();
	if (!ft_strncmp(str_to_lower(tmp), "cd", len))
 	   	execute_builtin_cd(resrc, list->command);
	else if (!ft_strncmp(str_to_lower(tmp), "unset", len) && check_input(cmd_arr))
        execute_builtin_unset(list, resrc);
	else if (!ft_strncmp(str_to_lower(tmp), "export", 6))
        execute_builtin_export(list, resrc);
	free(tmp);
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
		if (!list->next)
			check_for_parent_builtin(resrc, list, cmd_arr, len);
		if (cmd_check(list))
			exec_cmd(resrc, list);
		list = list->next;
		wait_for_child(lst_size);
	}
}	 
