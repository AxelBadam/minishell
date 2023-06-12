/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 12:04:53 by atuliara          #+#    #+#             */
/*   Updated: 2023/06/12 11:44:21 by atuliara         ###   ########.fr       */
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

