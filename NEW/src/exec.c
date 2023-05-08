/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:36:40 by atuliara          #+#    #+#             */
/*   Updated: 2023/05/08 17:01:59 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linked_list.h"
#include "minishell.h"
#include <stdbool.h>

int execute_builtin_cd(Command *cmd) {
    // Check if the command has an argument
    /*if (cmd->args->count > 1) {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }*/

    // Change the working directory
	
    const char *path = cmd->args->head->next->value;
		
    if (chdir(path) == -1) 
	{
        perror("cd");
        return 1;
    }
    return 0;
}

int execute_builtin_echo(Command *cmd) 
{
    
	LinkedListNode *arg_node = cmd->args->head;
	int newline;
	newline = 1;
	
	if (ft_strncmp(arg_node->next->value, "-n", 2) == 0)
	{
		arg_node = arg_node->next;
		newline = 0;
	}
    while (arg_node->value != NULL) 
	{
        ft_putstr_fd(arg_node->next->value, cmd->output_fd);        
		arg_node = arg_node->next;
		if (arg_node->next)
			write(cmd->output_fd, " ", 1);
		else if (newline)
            ft_putstr_fd("\n", cmd->output_fd);
	}
    return 0;
}

int execute_builtin_pwd(Command *cmd) {
   char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if (cmd->output_fd != -1) {
            write(cmd->output_fd, cwd, strlen(cwd));
            write(cmd->output_fd, "\n", 1);
        } else {
            printf("%s\n", cwd);
        }
    } else {
        perror("getcwd");
        return false;
    }
	return (1);
}

void setup_pipes(int pipefds[], int current_command, int total_commands) {
    if (current_command > 0) {
        dup2(pipefds[(current_command - 1) * 2], STDIN_FILENO);
    }

    if (current_command < total_commands - 1) {
        dup2(pipefds[current_command * 2 + 1], STDOUT_FILENO);
    }
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

bool execute_builtin_command(Command *cmd) {
    
	
	if (strcmp(cmd->name, "cd") == 0) {
        return execute_builtin_cd(cmd);
    } else if (strcmp(cmd->name, "pwd") == 0) {
        return execute_builtin_pwd(cmd);
    } else if (strcmp(cmd->name, "echo") == 0) {
        return execute_builtin_echo(cmd);
    }
    return 0;
}


void execute_commands(LinkedList *commands) {
    size_t num_commands = linked_list_count(commands);
    int pipefds[num_commands * 2];

    // Create pipes
    for (size_t i = 0; i < num_commands - 1; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("pipe");
            return;
        }
    }

    size_t i = 0;
    LinkedListNode *current_node = commands->head;
    while (current_node != NULL) {
        Command *cmd = (Command *)current_node->value;
        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            setup_pipes(pipefds, i, num_commands);
            setup_redirections(cmd);

            // Execute built-in command or external command
            if (!execute_builtin_command(cmd)) {
                execvp(cmd->name, &cmd->args->head->value);
                perror("execvp");
                exit(1);
            } else {
                exit(0); // Exit with success status for built-in commands
            }
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }

        current_node = current_node->next;
        i++;
    }

    // Close all pipes
    for (size_t i = 0; i < num_commands * 2; i++) {
        close(pipefds[i]);
    }

    // Wait for child processes to finish
    for (size_t i = 0; i < num_commands; i++) {
        wait(NULL);
    }
}





