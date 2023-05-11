/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:36:40 by atuliara          #+#    #+#             */
/*   Updated: 2023/05/11 17:09:31 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linked_list.h"
#include "minishell.h"
#include <stdbool.h>

int execute_builtin_cd(Command *cmd) 
{
    LinkedListNode *arg_node = cmd->args->head;
    char *path = arg_node->next->value;

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
            ft_putstr_fd(arg_node->next->value, STDOUT_FILENO); // using STDOUT_FILENO
        if (arg_node->next->next != NULL)
            write(STDOUT_FILENO, " ", 1); // using STDOUT_FILENO
        arg_node = arg_node->next;
    }
    if (newline)
    {
        ft_putstr_fd("\n", STDOUT_FILENO); // using STDOUT_FILENO
    }
    return 0;
}


int execute_builtin_pwd() 
{
    char cwd[4096];
    
    if (getcwd(cwd, sizeof(cwd)) != NULL) 
    {
        write(STDOUT_FILENO, cwd, strlen(cwd)); // using STDOUT_FILENO
        write(STDOUT_FILENO, "\n", 1); // using STDOUT_FILENO
    } 
    else 
    {
        write(2, "pwd, error", 8);
        return (0);
    }
    return (1);
}


int setup_redirections(Command *cmd) {
    if (cmd->input_file) 
	{
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

bool execute_builtin(Command *cmd) 
{
	if (ft_strncmp(cmd->name, "cd", 2) == 0)
        return (execute_builtin_cd(cmd));
    else if (ft_strncmp(cmd->name, "pwd", 3) == 0)
        return (execute_builtin_pwd());
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
/*
void execute_cmd(Command *cmd)
{
	pid_t pid;
	
	pid = fork();
	
	if (!execute_builtin(cmd))
    	execvp(cmd->name, &cmd->args->head->value);
	if (pid < 0) 
	{
		write(1, "fork error", 9);
    	exit(1);
	}
}*/

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
    if (current_command > 0) // check if first
        dup2(pipefds[(current_command - 1) * 2], STDIN_FILENO);
    if (current_command < total_commands - 1) // check if not last
        dup2(pipefds[current_command * 2 + 1], STDOUT_FILENO);
}

void create_pipes(int command_count, int *pipefds)
{
	int i;
	
	i = 0;
	while (i < command_count - 1)
	{
		if (pipe(&pipefds[i * 2]) < 0)
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
    Command *cmd;
    pid_t pid;

    create_pipes(command_count, pipefds);
    i = 0;

    LinkedListNode *current_node = commands->head;
    while (current_node != NULL) 
    {
        cmd = (Command *)current_node->value;
        pid = fork();

        if (pid < 0) 
        {
            write(1, "fork error", 9);
            exit(1);
        }

        if (pid == 0)
        { 
            // Child process
            if (i != 0) 
            {
                // If not the first command, need to get input from previous command
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            }

            if (i != command_count - 1) 
            {
                // If not the last command, need to send output to next command
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
            }

            // Close all pipes in child process
            for (int j = 0; j < 2 * command_count; j++)
            {
                close(pipefds[j]);
            }

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
        else 
        {
            // Parent process
            // Close unused ends of pipes
            if (i != 0) 
            {
                close(pipefds[(i - 1) * 2]);
            }

            if (i != command_count - 1) 
            {
                close(pipefds[i * 2 + 1]);
            }
        }

        current_node = current_node->next;
        i++;
    }

    // Wait for all child processes to finish
    for (i = 0; i < command_count; i++) 
    {
        wait(NULL);
    }
}

/*
void execute_commands(LinkedList *commands, int command_count)
{
	int pipefds[command_count * 2];
	int i;
	Command *cmd;
	pid_t pid;

	create_pipes(command_count, pipefds);
	i = 0;
	while (pipefds[i])
		printf("%d", pipefds[i++]);
    LinkedListNode *current_node = commands->head;
    while (current_node != NULL) 
    {
		cmd = (Command *)current_node->value;
		pid = fork();
		if (pid < 0) 
		{
			write(1, "fork error", 9);
    		exit(1);
		}
		if (pid == 0)
		{ 
			setup_redirections(cmd);
			setup_pipes(pipefds, i, command_count - 1);
			//printf("%d", command_count);
			printf("input fd = %d\n", cmd->input_fd);
			printf("output fd = %d\n", cmd->output_fd);
			printf("output file = %s\n", cmd->output_file);
			printf("input file = %s\n", cmd->input_file);
			if (is_builtin(cmd))
			{
				execute_builtin(cmd);
				write(1, "lel", 3);
				exit (0);
			}
			else 
			{
				execvp(cmd->name, &cmd->args->head->value);
				exit(1);
			}
			
		}
		current_node = current_node->next;
		i++;
	}
	wait_for_child(command_count);	
	close_pipes(command_count, pipefds);
}
*/
