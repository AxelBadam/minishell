/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:36:40 by atuliara          #+#    #+#             */
/*   Updated: 2023/05/24 14:13:12 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linked_list.h"
#include "minishell.h"
#include <stdbool.h>


bool execute_builtin(Command *cmd) 
{
    if (ft_strncmp(cmd->name, "pwd", 3) == 0)
        return (execute_builtin_pwd());
 	else if (ft_strncmp(cmd->name, "echo", 4) == 0) 
        return (execute_builtin_echo(cmd));
    return 0;
}

int is_builtin(Command *cmd)
{
	if (ft_strncmp(cmd->name, "pwd", 3) == 0)
   	  	return (1);
   	else if (ft_strncmp(cmd->name, "echo", 4) == 0)
        return (1);
	else if (ft_strncmp(cmd->name, "env", 3) == 0)
        return (1);
	else if (ft_strncmp(cmd->name, "export", 6) == 0)
        return (1);
	else if (ft_strncmp(cmd->name, "unset", 5) == 0)
        return (1);
    return 0;
}

int check_for_parent_builtin(Command *cmd, LinkedList *commands)
{	
	if (ft_strncmp(cmd->name, "cd", 2) == 0)
        return (execute_builtin_cd(cmd));
	else if (ft_strncmp(cmd->name, "exit", 4) == 0)
       	return (execute_builtin_exit());
	else if (ft_strncmp(cmd->name, "env", 3) == 0)
        return (execute_builtin_env(cmd, &commands->env));
	/*else if (ft_strncmp(cmd->name, "export", 6) == 0)
        return (execute_builtin_export(cmd, &commands->env));*/
	else if (ft_strncmp(cmd->name, "unset", 5) == 0)
        return (execute_builtin_unset(cmd, &commands->env));	
	return 0;
}

void create_pipes(int command_count, int *pipefds)
{
	int i = 0;
	
	while (i < command_count - 1)
	{
		if (pipe(&pipefds[i * 2]) < 0)
        {
            error_handling("pipes");
            exit(1);
        }
	i++;
	}
}

void setup_pipes(int pipefds[], int current_command, int total_commands) 
{
    // If not the first command, redirect input from the previous pipe.
    if (current_command > 0)
    {
        dup2(pipefds[(current_command - 1) * 2], STDIN_FILENO);
    }
    // If not the last command, redirect output to the next pipe.
    if (current_command < total_commands - 1)
    {
        dup2(pipefds[current_command * 2 + 1], STDOUT_FILENO);
    }
}

void close_pipes(int command_count, int *pipefds)
{
    int i;

	i = 0;
	while (i < 2 * command_count)
	{
		close(pipefds[i]);
		i++;
	}
}

void execute_commands_pipes(LinkedList *commands, int command_count)
{
	int pipefds[command_count * 2];
	int i;
	Command *cmd;
	pid_t pid;
	

	i = 0;
	LinkedListNode *current_node = commands->head;
    while (current_node != NULL) 
    {
		cmd = (Command *)current_node->value;
		if (!check_for_parent_builtin(cmd, commands))
		{
			create_pipes(command_count, pipefds);
			pid = fork();
			if (pid < 0) 
				error_handling("forkerror");
			if (pid == 0)
			{ 
				if (command_count > 1)
				{
				setup_redirections(cmd);
				setup_pipes(pipefds, i, command_count);
				close_pipes(command_count, pipefds);
				}
				if (is_builtin(cmd))
				{
					execute_builtin(cmd);
					exit (0);
				}
				else 
				{
					execvp(cmd->name, &cmd->args->head->value);
					exit(1);
				}		
		}
	}
	current_node = current_node->next;
	i++;
	close_pipes(command_count, pipefds);
	wait_for_child(command_count);
	}
}

/*
void execute_commands(LinkedList *commands, int command_count)
{
    int pipefds[2];
    Command *cmd;
    pid_t pid;

    LinkedListNode *current_node = commands->head;
    while (current_node != NULL) 
    {
        cmd = (Command *)current_node->value;
		if (check_for_parent_builtin(cmd) && current_node->next != NULL)
			{
				current_node = current_node->next;
				cmd = (Command *)current_node->value;
			}
		//if has a pipe, make process, int pip
        if (pipe(pipefds) < 0)
			error_handling("pipes");
		pid = fork();
        if (pid < 0) 
			error_handling("fork");
        if (pid == 0)
        {
			//write(1, "hello", 5);
			setup_pipes(cmd, pipefds);
			setup_redirections(cmd);
            if (is_builtin(cmd))
            {
                execute_builtin(cmd);
                exit(0);
            }
            else 
            {
                execvp(cmd->name, &cmd->args->head->value);
                exit(1);
            }
        }
        current_node = current_node->next;
    } 
	close_pipes(pipefds);
	wait_for_child(command_count);
}

	int pipefds[2];
	Command *cmd;
    pid_t pid;

    LinkedListNode *current_node = commands->head;
	
	if (pipe(pipefds) < 0)
		error_handling("pipes");
    while (current_node != NULL) // and cmd->pipe = 1) 
    {
        cmd = (Command *)current_node->value;
		pid = fork();
        if (pid < 0) 
			error_handling("fork");
        if (pid == 0)
        {
			setup_pipes(cmd, pipefds);
            if (is_builtin(cmd))
            {
                execute_builtin(cmd);
                exit(0);
            }
            else 
            {
                execvp(cmd->name, &cmd->args->head->value);
                exit(1);
            }
        }
        current_node = current_node->next;
    } 
	close_pipes(pipefds);
	wait_for_child(command_count);
}
*/

    		/*dup2(pipefds[1], STDOUT_FILENO);
			close(pipefds[1]);
			close(pipefds[0]);*/

/*
			//printf("%d", command_count);
			printf("input fd = %d\n", cmd->input_fd);
			printf("output fd = %d\n", cmd->output_fd);
			printf("output file = %s\n", cmd->output_file);
			printf("input file = %s\n", cmd->input_file);
*/

