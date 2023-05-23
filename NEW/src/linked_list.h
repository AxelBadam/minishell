/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:21:54 by atuliara          #+#    #+#             */
/*   Updated: 2023/05/18 17:50:11 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H


typedef enum {
    CMD_NORMAL,
    CMD_PIPE,
    CMD_REDIRECT_IN,
    CMD_REDIRECT_OUT,
    CMD_REDIRECT_APPEND_OUT,
    CMD_REDIRECT_IN_OUT
} CommandType;

typedef struct LinkedListNode {
    char *value;
    CommandType type;
    struct LinkedListNode *next;
} LinkedListNode;

typedef struct LinkedList {
    LinkedListNode *head;
	char **env;
    int size;
} LinkedList;
/*
typedef struct s_global
{
	char **env;
} t_global;
*/

typedef struct {
    char *name;
    LinkedList *args;
	char **env;
    char *input_file;
    char *output_file;
	int input_fd;
    int output_fd;
    int append_output;
} Command;

// Function prototypes
LinkedList *linked_list_create();
void linked_list_append(LinkedList *list, char *value);
void linked_list_free(LinkedList *list);
void linked_list_print(LinkedList *list);
LinkedList *parse_tokens(const char *input, const char *delimiter);
void add_type(LinkedList *tokens);
LinkedList *build_command_structure(LinkedList *tokens, LinkedList *command_types);
void print_commands(LinkedList *commands);
int linked_list_count(LinkedList *list);

#endif
