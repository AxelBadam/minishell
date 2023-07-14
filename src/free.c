/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:25:27 by ekoljone          #+#    #+#             */
/*   Updated: 2023/07/14 13:15:39 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

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
			free_all_nodes(resource);
		if (resource->array)
			free_string_array(resource->array);
		free(resource);
	}
	exit(1);
}

void	free_all_nodes(t_resrc *rs)
{
	t_list	*tmp;

	tmp = rs->list;
	while (rs->list)
	{
		if (rs->list->command.full_cmd)
			free_string_array(rs->list->command.full_cmd);
		if (rs->list->command.full_path)
			free(rs->list->command.full_path);
		if (rs->list->command.input_fd != 0)
			close(rs->list->command.input_fd);
		if (rs->list->command.output_fd != 1)
			close(rs->list->command.output_fd);
		rs->list = rs->list->next;
		if (tmp)
			free(tmp);
		tmp = rs->list;
	}
	if (rs->list)
		free(rs->list);
	if (rs->array)
		free_string_array(rs->array);
	rs->array = NULL;
	rs->list = NULL;
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
		array = NULL;
	}
}
