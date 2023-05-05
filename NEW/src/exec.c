/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:36:40 by atuliara          #+#    #+#             */
/*   Updated: 2023/05/05 17:26:47 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linked_list.h"
#include "minishell.h"
#include <stdbool.h>

bool execute_builtin_command(Command *cmd) {
    if (strcmp(cmd->name, "cd") == 0) {
        if (linked_list_count(cmd->args) == 1) {
            fprintf(stderr, "cd: missing argument\n");
        } else {
            char *path = (char *)cmd->args->head->next->value;
            if (chdir(path) == -1) {
                perror("cd");
            }
        }
        return true;
    } else if (strcmp(cmd->name, "pwd") == 0) {
        char path[4096];
        if (getcwd(path, sizeof(path)) == NULL) {
            perror("pwd");
        } else {
            printf("%s\n", path);
        }
        return true;
    } else if (strcmp(cmd->name, "echo") == 0) {
        LinkedListNode *current_node = cmd->args->head->next;
        while (current_node != NULL) {
            printf("%s", (char *)current_node->value);
            if (current_node->next != NULL) {
                printf(" ");
            }
            current_node = current_node->next;
        }
        printf("\n");
        return true;
    }
    // Add more built-in commands here as needed

    return false;
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
        int input_fd = open(cmd->input_file, O_RDONLY);
        if (input_fd == -1) {
            perror("open");
            return -1;
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }

    if (cmd->output_file) {
        int output_fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd == -1) {
            perror("open");
            return -1;
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }

    return 0;
}

// someting going on here

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
        if (!execute_builtin_command(cmd)) {
            pid_t pid = fork();

            if (pid == 0) {
                // Child process
                setup_pipes(pipefds, i, num_commands);
                setup_redirections(cmd);
				LinkedListNode *arg_node = cmd->args->head;
                while (arg_node != NULL) {
                    printf("Argument: %s\n", (char *)arg_node->value);
                    arg_node = arg_node->next;
                }
				
                // Execute the command
                execvp(cmd->name, &cmd->args->head->value);
                perror("execvp");
                exit(1);
            } else if (pid < 0) {
                perror("fork");
				exit(1);
            }
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




