/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:06:37 by atuliara          #+#    #+#             */
/*   Updated: 2023/06/14 17:01:55 by ekoljone         ###   ########.fr       */
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
		return ;
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
	close(list->command.output_fd);
	close(list->command.input_fd);
	exit(g_exit_status);
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
	signal(SIGINT, signal_handler);
	signal(SIGTSTP, SIG_DFL);
}

void exec_cmd(t_resrc *resrc, t_list *list)
{
	int fd[2];

	if (list->next)
	{
		if (pipe(fd) < 0)
		{
			close_pipes(list, fd);
			error_handling("pipe error");
		}
		else if (list->next->command.input_fd == 0)
			list->next->command.input_fd = fd[0];
	}
	do_fork(resrc, list, fd);
	//close(fd[1]);
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


int check_for_parent_builtin(t_resrc *resrc, t_list *list, char **cmd_arr, int len)
{	
	char	*tmp;
	int		ret;

	ret = 0;
	tmp = ft_strdup(*cmd_arr);
	tmp = str_to_lower(tmp);
	if (!ft_strncmp(tmp, "exit", len))
      	execute_builtin_exit(cmd_arr);
	if (!ft_strncmp(tmp, "cd", len))
	{
 	   	execute_builtin_cd(resrc, list->command);
		ret = 1;
	}
	else if (!ft_strncmp(tmp, "unset", len) && check_input(cmd_arr))
	{
        execute_builtin_unset(list, resrc);
		ret = 1;
	}
	else if (!ft_strncmp(tmp, "export", 6))
    {    
		execute_builtin_export(list, resrc);
		ret = 1;
	}
	return (free(tmp), ret);
}

void execution(t_resrc *resrc, t_list *list)
{
    char	**cmd_arr;
    int		len;
    int		lst_size;
	t_list	*tmp;

	tmp = list;
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
