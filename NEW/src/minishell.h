/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:30:16 by ekoljone          #+#    #+#             */
/*   Updated: 2023/05/12 10:13:42 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <unistd.h>
# include <signal.h>
# include "libft/libft.h"
#include "linked_list.h"

/*
typedef struct s_resrc
{
	char	*line;
	char	**s_line;
	char	*buf;
	char	*history;
}			t_resrc;

typedef struct s_command
{
	char				*command;
	char				*command_argument;
	char				*operator;
	char				*filename;
	struct s_command	*next;
}						t_command;
*/
void handle_input(const char *input);
CommandType identify_command_type(const char *token);
void execute_commands(LinkedList *commands, int command_count);
void create_pipes(int command_count, int *pipefds);
void setup_pipes(int pipefds[], int current_command, int total_commands);
void close_pipes(int command_count, int *pipefds);

#endif