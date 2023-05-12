/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:36:40 by atuliara          #+#    #+#             */
/*   Updated: 2023/05/12 14:51:50 by atuliara         ###   ########.fr       */
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

int execute_builtin_exit()
{
	write(1, "exit\n", 5);
	exit (0);
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
    return 0;
}

int check_for_parent_builtin(Command *cmd)
{
	if (ft_strncmp(cmd->name, "cd", 2) == 0)
        return (execute_builtin_cd(cmd));
	else if (ft_strncmp(cmd->name, "exit", 4) == 0)
       	return (execute_builtin_exit());
    /*else if (ft_strncmp(cmd->name, "unset", 5) == 0)
        return (execute_builtin_unset(cmd));
    else if (ft_strncmp(cmd->name, "env", 3) == 0)
        return (execute_builtin_env(cmd));
	else if (ft_strncmp(cmd->name, "export", 6) == 0)
        return (execute_builtin_export(cmd));*/
	return 0;
}

void execute_commands(LinkedList *commands, int command_count)
{
    int pipefds[command_count * 2];
    int i;
    Command *cmd;
    pid_t pid;

	// might have to create pipes on the spot according to the command line...
    create_pipes(command_count, pipefds);
    i = 0;
    LinkedListNode *current_node = commands->head;
    while (current_node != NULL) 
    {
        cmd = (Command *)current_node->value;
		(check_for_parent_builtin(cmd) && has_no_pipes)
			do built_in and move on;
		//if has a pipe or redir, make a process
        pid = fork();
        if (pid < 0) 
        {
            write(1, "fork error", 9);
            exit(1);
        }
        if (pid == 0)
        {
			setup_redirections(cmd);
			setup_pipes(pipefds, i, command_count);
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
            if (i != 0) 
				close_pipes(command_count, pipefds);
        current_node = current_node->next;
        i++;
    }
	wait_for_child(command_count);
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
