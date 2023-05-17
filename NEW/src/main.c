/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:20:59 by ekoljone          #+#    #+#             */
/*   Updated: 2023/05/17 18:40:00 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "linked_list.h"

CommandType identify_command_type(const char *token) {
   
    if (ft_strncmp(token, "|", 1) == 0) {
        return CMD_PIPE;
    } else if (ft_strncmp(token, "<", 1) == 0) {
        return CMD_REDIRECT_IN;
    } else if (ft_strncmp(token, ">", 1) == 0) {
        return CMD_REDIRECT_OUT;
    } else if (ft_strncmp(token, ">>", 2) == 0) {
        return CMD_REDIRECT_APPEND_OUT;
    } else if (ft_strncmp(token, "<<", 2) == 0) {
        return CMD_REDIRECT_IN_OUT;
    } else {
        return CMD_NORMAL;
    }
}

void handle_input(const char *input) 
{
    // Parse the input string into tokens (commands and arguments)
    LinkedList *tokens = parse_tokens(input, " \t\n");
	LinkedListNode *current_node = tokens->head;
	LinkedList *command_types = linked_list_create();
	int command_count;
	
    while (current_node != NULL) 
	{
        current_node->type = identify_command_type(current_node->value);
		current_node = current_node->next;
	}
	current_node = tokens->head;
	while (current_node)
	{
		CommandType type = current_node->type;
		linked_list_append(command_types, (void *)(intptr_t)type);
		current_node = current_node->next;
	}
	//linked_list_print(tokens);
	LinkedList *commands = build_command_structure(tokens, command_types);
	print_commands(commands);
	command_count = linked_list_count(commands);
	execute_commands_pipes(commands, command_count);
	
	// Iterate through the tokens and execute appropriate actions
    // based on the type of token (command, argument, I/O redirection, pipeline, etc.)
    linked_list_free(tokens);
}

int main(int ac, char **av, char **env) 
{
	(void)ac;
	(void)av;
    char *input;
    char *prompt = "minishell> ";
	t_global envi;
	envi.env = create_env(env);
	/*while (*envi.env)
	{
		printf("%s\n", *envi.env++);
	}*/
	
    while (1) 
	{
        input = readline(prompt);
        if (!input) 
		{
            printf("\n");
            break;
        }
        if (input[0] != '\0') 
		{
            add_history(input);
            handle_input(input);
        }
        free(input);
    }
    return 0;
}
