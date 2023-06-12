/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:25:27 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/12 13:16:42 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

int	print_error(char *str, int exit_status, char *filename)
{
	ft_putstr_fd("minishell: ", 2);
	if (filename)
		ft_putstr_fd(filename, 2);
	ft_putstr_fd(str, 2);
	g_exit_status = exit_status;
	return (0);
}

void	error_exit(char *str, t_resrc *resource)
{
	ft_putstr_fd(str, 2);
	if (resource)
	{
		if (resource->list)
			free_all_nodes(&resource->list);
		if (resource->array)
			free_string_array(resource->array);
		free(resource);
	}
	exit(1);
}

void	free_all_nodes(t_list **head)
{
	t_list	*tmp;

	tmp = *head;
	while (*head)
	{
		if ((*head)->command.full_cmd)
			free_string_array((*head)->command.full_cmd);
		if ((*head)->command.full_path)
			free((*head)->command.full_path);
		*head = (*head)->next;
		if (tmp)
			free(tmp);
		tmp = *head;
	}
	if (*head)
		free(*head);
}

void	free_string_array(char **array)
{
	int	ctr;

	ctr = 0;
	if (array)
	{
		while (array[ctr])
			free(array[ctr++]);
		free(array);
	}
}