/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 12:04:53 by atuliara          #+#    #+#             */
/*   Updated: 2023/06/16 12:44:58 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

int linked_list_count(t_list **lst)
{
    size_t count;
	t_list *head;
	
	head = *lst;
	count = 0;
    while (head->next != NULL)
	{
        count++;
		head = head->next;
	}
    return (count + 1);
}

int	get_2d_array_size(char **array)
{
    int	ctr;
	
	ctr = 0;
	if (!array)
    	return (0);
	while (array[ctr])
    	ctr++;
	return (ctr);
}

char *str_to_lower(char *tmp)
{
	int ctr;

	ctr = -1;
	while (tmp[++ctr])
		tmp[ctr] = ft_tolower(tmp[ctr]);
	return (tmp);
}

void	close_wait(t_list *list)
{
	t_list	*tmp;

	tmp = list;
	while (tmp)
	{
		if (tmp->command.fd[0] != -2)
			close(tmp->command.fd[0]);
		tmp = tmp->next;
	}
	tmp = list;
	while (tmp)
	{
		if (tmp->command.pid != -2)
		{
			waitpid(tmp->command.pid, &g_exit_status, WUNTRACED);
			check_signal(list);
		}
		tmp = tmp->next;
	}
}
