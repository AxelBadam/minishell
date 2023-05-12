/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:30:16 by ekoljone          #+#    #+#             */
/*   Updated: 2023/05/12 13:22:42 by ekoljone         ###   ########.fr       */
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
	char	**envp;
}			t_resrc;

typedef struct s_command
{
	char				*command;
	char				*command_option;
	char				*string;
	int					output_fd;
	struct s_command	*next;
}						t_command;

void	print_list(t_command **head);
#endif