/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 12:04:53 by atuliara          #+#    #+#             */
/*   Updated: 2023/05/26 17:15:54 by atuliara         ###   ########.fr       */
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

void    free_string_array(char **array)
{
    int    ctr;

    ctr = 0;
    if (array)
    {
        while (array[ctr])
            free(array[ctr++]);
        free(array);
    }
}