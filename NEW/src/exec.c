/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:36:40 by atuliara          #+#    #+#             */
/*   Updated: 2023/05/10 16:24:56 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linked_list.h"
#include "minishell.h"
#include <stdbool.h>

int execute_builtin_cd(Command *cmd) 
{
    LinkedListNode *arg_node = cmd->args->head;

    // Get the path from the command arguments
    char *path = arg_node->next->value;

    // Change the current working directory to the path
    if (chdir(path) != 0) 
	{
        write(2, "cd, error", 8);
        return (1);
    }

    return 0;
}

int execute_builtin_echo(Command *cmd) 
{
    LinkedListNode *arg_node = cmd->args->head;
    int newline = 1;
    
    if (arg_node != NULL && arg_node->next != NULL && ft_strncmp(arg_node->next->value, "-n", 2) == 0)
    {
        arg_node = arg_node->next;
        newline = 0;
    }

    while (arg_node->next != NULL) 
    {
        if (arg_node->value != NULL)
        {
            ft_putstr_fd(arg_node->next->value, cmd->output_fd);
        }

        if (arg_node->next->next != NULL)
        {
            write(cmd->output_fd, " ", 1);
        }

        arg_node = arg_node->next;
    }

    if (newline)
    {
        ft_putstr_fd("\n", cmd->output_fd);
    }
    return 0;
}


int execute_builtin_pwd(Command *cmd) 
{

   	char cwd[4096];
    
	if (getcwd(cwd, sizeof(cwd)) != NULL) 
	{
        if (cmd->output_fd != -1) // when -1?
		{
            write(cmd->output_fd, cwd, strlen(cwd));
            write(cmd->output_fd, "\n", 1);
        } 
		else 
            printf("%s\n", cwd);
    } 
	else 
	{
       	write(2, "pwd, error", 8);
        return (0);
    }
	return (1);
}

int setup_redirections(Command *cmd) {
    if (cmd->input_file) {
        cmd->input_fd = open(cmd->input_file, O_RDONLY);
        if (cmd->input_fd == -1) {
            perror("open");
            return -1;
        }
        dup2(cmd->input_fd, STDIN_FILENO);
    }

    if (cmd->output_file) {
        cmd->output_fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (cmd->output_fd == -1) {
            perror("open");
            return -1;
        }
        dup2(cmd->output_fd, STDOUT_FILENO);
    }
    return 0;
}

bool execute_builtin(Command *cmd) {
    
	
	if (ft_strncmp(cmd->name, "cd", 2) == 0)
        return (execute_builtin_cd(cmd));
    else if (ft_strncmp(cmd->name, "pwd", 3) == 0)
        return (execute_builtin_pwd(cmd));
 	else if (ft_strncmp(cmd->name, "echo", 4) == 0) 
        return (execute_builtin_echo(cmd));
	else if (ft_strncmp(cmd->name, "exit", 4) == 0) 
        		exit(0);
    /*else if (ft_strncmp(cmd->name, "unset", 5) == 0)
        return (execute_builtin_unset(cmd));
    else if (ft_strncmp(cmd->name, "env", 3) == 0)
        return (execute_builtin_env(cmd));
	else if (ft_strncmp(cmd->name, "export", 6) == 0)
        return (execute_builtin_export(cmd));*/
    return 0;
}

int is_builtin(Command *cmd)
{
		
	if (ft_strncmp(cmd->name, "cd", 2) == 0)
        return (1);
	else if (ft_strncmp(cmd->name, "exit", 4) == 0)
        return (1);
	else if (ft_strncmp(cmd->name, "pwd", 3) == 0)
        return (1);
    else if (ft_strncmp(cmd->name, "echo", 4) == 0)
        return (1);
	else if (ft_strncmp(cmd->name, "export", 6) == 0)
        return (1);
    else if (ft_strncmp(cmd->name, "unset", 5) == 0)
        return (1);
    else if (ft_strncmp(cmd->name, "env", 3) == 0)
        return (1); 
    return 0;
}

void execute_cmd(Command *cmd)
{
	pid_t pid;
	
	pid = fork();
	
	if (pid == 0) 
    	execvp(cmd->name, &cmd->args->head->value);
	else if (pid < 0) 
	{
		write(1, "fork error", 9);
    	exit(1);
	}
}

void wait_for_child(int command_count)
{
	int i;

	i = -1;
	while (++i < command_count)
		wait(NULL); // store the value in an int
}

void close_pipes(int command_count, int *pipefds)
{
	int i;

	i = -1;
	while (++i < command_count * 2)
		close(pipefds[i]);
}


void setup_pipes(int pipefds[], int current_command, int total_commands) 
{
    if (current_command > 0)
        dup2(pipefds[(current_command - 1) * 2], STDIN_FILENO);
    if (current_command < total_commands - 1)
        dup2(pipefds[current_command * 2 + 1], STDOUT_FILENO);
}

void create_pipes(int command_count, int *pipefds)
{
	int i;
	
	i = 0;
	while (i < command_count - 1)
	{
		if (pipe(pipefds + i * 2) < 0)
		{
			write(1, "error pipe", 10);
			return ;
		}
		i++;
	}
}

void execute_commands(LinkedList *commands, int command_count) 
{
	int pipefds[command_count * 2];
	int i;

	create_pipes(command_count, pipefds);
	i = 0;
    LinkedListNode *current_node = commands->head;
    while (current_node != NULL) 
    {
		Command *cmd;
		cmd = (Command *)current_node->value;
		//print_commands(commands);	
		setup_redirections(cmd);
		setup_pipes(pipefds, i++, command_count);
        if (is_builtin(cmd) == 1) 
			execute_builtin(cmd);
		else
			execute_cmd(cmd);
		close_pipes(command_count, pipefds);
		wait_for_child(command_count);
		current_node = current_node->next;
	}
}
