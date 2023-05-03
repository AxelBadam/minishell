/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:30:16 by ekoljone          #+#    #+#             */
/*   Updated: 2023/05/03 17:26:25 by ekoljone         ###   ########.fr       */
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

t_command	*parse_list(char *line);
void	print_list(t_command **head);
#endif