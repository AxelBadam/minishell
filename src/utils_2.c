/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:19:40 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/22 14:16:05 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

void	iterate_over_spaces(char *line, int *tmp)
{
	while (ft_is_white_space(line[tmp[0]]))
		tmp[0]++;
}

int	is_a_directory(char *filename)
{
	struct stat	statbuf;
	int			dir;

	if (stat(filename, &statbuf) != 0)
		return (0);
	dir = S_ISDIR(statbuf.st_mode);
	return (dir);
}

int	get_array_size(char **array)
{
	int	ctr;

	ctr = 0;
	if (!array)
		return (0);
	while (array[ctr])
		ctr++;
	return (ctr);
}

t_list	*ft_lst_last(t_list *head)
{
	while (head->next != NULL)
		head = head->next;
	return (head);
}

void	ft_lstadd_back(t_list **head, t_list *new)
{
	t_list	*tmp;

	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	tmp = ft_lst_last(*head);
	tmp->next = new;
	new->prev = tmp;
	new->next = NULL;
}
