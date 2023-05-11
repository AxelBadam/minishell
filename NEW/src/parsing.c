/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 14:23:41 by atuliara          #+#    #+#             */
/*   Updated: 2023/05/11 16:02:58 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <ctype.h>
#include <stdbool.h>
#include "linked_list.h"
#include "minishell.h"

typedef enum {
    NORMAL,
    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    ESCAPE
} ParseState;

LinkedList *parse_tokens(const char *input, const char *delimiter) 
{
    ParseState state = NORMAL;
    LinkedList *tokens = linked_list_create();
    char *token = malloc(ft_strlen(input) + 1);
    size_t token_len = 0;

    while (*input) 
	{
        char c = *input;

        if (state == NORMAL) 
		{
            if (ft_strchr(delimiter, c)) 
			{
                if (token_len > 0) {
                    token[token_len] = '\0';
                    linked_list_append(tokens, token);
                    token = malloc(ft_strlen(input) + 1);
                    token_len = 0;
                }
            } else if (c == '\'') {
                state = SINGLE_QUOTE;
            } else if (c == '\"') {
                state = DOUBLE_QUOTE;
            } else if (c == '\\') {
                state = ESCAPE;
            } else if (c == '>' || c == '<') {
                // Add the current token to the list
                if (token_len > 0) {
                    token[token_len] = '\0';
                    linked_list_append(tokens, token);
                    token = malloc(strlen(input) + 1);
                    token_len = 0;
                }
                // Add the redirection operator as a separate token
                token[0] = c;
                if (*(input + 1) == c) { // Check for '>>' or '<<'
                    token[1] = c;
                    token[2] = '\0';
                    input++; // Skip the next character
                } else {
                    token[1] = '\0';
                }
                linked_list_append(tokens, token);
                token = malloc(strlen(input) + 1);
            } else {
                token[token_len++] = c;
            }
        } else if (state == SINGLE_QUOTE) {
            if (c == '\'') {
                state = NORMAL;
            } else {
                token[token_len++] = c;
            }
        } else if (state == DOUBLE_QUOTE) {
            if (c == '\"') {
                state = NORMAL;
            } else if (c == '\\') {
                state = ESCAPE;
            } else {
                token[token_len++] = c;
            }
        } else if (state == ESCAPE) {
            token[token_len++] = c;
            state = (state == ESCAPE) ? NORMAL : DOUBLE_QUOTE;
        }
        input++;
    }

    // Add the last token to the list
    if (token_len > 0) {
        token[token_len] = '\0';
        linked_list_append(tokens, token);
    } else {
        free(token);
    }

    return tokens;
}

LinkedList *build_command_structure(LinkedList *tokens, LinkedList *command_types) 
{
    LinkedList *commands = linked_list_create();
    Command *current_command = NULL;

    LinkedListNode *current_node = tokens->head;
    LinkedListNode *current_type_node = command_types->head;

    while (current_node != NULL && current_type_node != NULL) {
        char *token = (char *)current_node->value;
        CommandType cmd_type = (CommandType)current_type_node->value;

        if (cmd_type == CMD_NORMAL) {
            if (current_command == NULL) {
                current_command = malloc(sizeof(Command));
                memset(current_command, 0, sizeof(Command));
                current_command->name = token;
                current_command->args = linked_list_create();
                linked_list_append(current_command->args, token);
                linked_list_append(commands, (void *)current_command);
            } else {
                linked_list_append(current_command->args, token);
            }
        } else if (cmd_type == CMD_PIPE) 
		{
            current_command = NULL;
        } else if (cmd_type == CMD_REDIRECT_IN) {
            current_node = current_node->next;
            current_type_node = current_type_node->next;
            if (current_node != NULL) {
                current_command->input_file = (char *)current_node->value;
            }
        } else if (cmd_type == CMD_REDIRECT_OUT || cmd_type == CMD_REDIRECT_APPEND_OUT) {
             current_node = current_node->next;
            current_type_node = current_type_node->next;
            if (current_node != NULL) {
                current_command->output_file = (char *)current_node->value;
                current_command->append_output = (cmd_type == CMD_REDIRECT_APPEND_OUT);
            }
        }
        current_node = current_node->next;
        current_type_node = current_type_node->next;
    }

    return commands;
}


























/*
char **parse_by_space(char *char)
{
	char **split;

	split = ft_split
}

void parse(char *cmd_line)
{
	char **split;

	split = parse_by_space(cmd_line);

}

int		quotes(char *line, int index)
{
	int	i;
	int	open;

	i = 0;
	open = 0;
	while (line[i] && i != index)
	{
		if (i > 0 && line[i - 1] == '\\')
			;
		else if (open == 0 && line[i] == '\"')
			open = 1;
		else if (open == 0 && line[i] == '\'')
			open = 2;
		else if (open == 1 && line[i] == '\"')
			open = 0;
		else if (open == 2 && line[i] == '\'')
			open = 0;
		i++;
	}
	return (open);
}*/
