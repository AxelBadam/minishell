/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 12:04:53 by atuliara          #+#    #+#             */
/*   Updated: 2023/05/30 13:09:36 by atuliara         ###   ########.fr       */
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

void wait_for_child(int command_count)
{
	while (command_count-- > 0)
		wait(&g_exit_status);
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