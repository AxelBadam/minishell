/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:30:16 by ekoljone          #+#    #+#             */
/*   Updated: 2023/05/18 17:11:23 by ekoljone         ###   ########.fr       */
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

typedef struct s_command
{
	char	*full_path;
	char	**full_cmd;
	int		output_fd;
	int		input_fd;
}	t_command;


typedef struct s_list
{
	t_command		command;
	struct s_list	*next;
}	t_list;

typedef struct s_resrc
{
	t_list	*list;
	char	**array;
	char	**envp;
}	t_resrc;

void	ft_lstadd_back(t_list **head, t_list *new);
t_list	*ft_lst_last(t_list *head);


#endif