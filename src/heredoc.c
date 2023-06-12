/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:30:05 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/09 18:31:12 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

void	add_array_to_array(t_resrc *resource, char **array, char **pipe_command)
{
	char	**n_arr;
	int		ctr[2];

	ctr[0] = 0;
	ctr[1] = 0;
	n_arr = (char **)malloc(sizeof(char *)
			* (get_array_size(array) + get_array_size(pipe_command) + 1));
	if (!n_arr)
		error_exit("minishell: fatal malloc error\n", resource);
	while (array[ctr[0]])
		n_arr[ctr[0]++] = ft_strdup(array[ctr[0]]);
	while (pipe_command[ctr[1]])
		n_arr[ctr[0]++] = ft_strdup(pipe_command[ctr[1]++]);
	n_arr[ctr[0]] = 0;
	free_string_array(array);
	free_string_array(pipe_command);
	resource->array = n_arr;
}

int	get_new_command(t_resrc *resource, char **array)
{
	char	*line;
	char	**pipe_command;

	g_exit_status = 0;
	line = readline("> ");
	if (!line)
	{
		write(1, "\033[1A\033[2C", 9);
		print_error("syntax error: unexpected end of file\n", 258, NULL);
	}
	if (line && g_exit_status != 1)
	{
		while (!*line)
			line = readline("> ");
		pipe_command = split_command(resource, line);
		add_array_to_array(resource, array, pipe_command);
		free(line);
		return (1);
	}
	free_all_nodes(&resource->list);
	return (0);
}

void	create_heredoc(int *fd, char *delimitor)
{
	char	*line;
	int		len;

	g_exit_status = 0;
	len = ft_strlen(delimitor);
	while (g_exit_status != 1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimitor, len) == 0)
		{
			if (!line)
				write(1, "\033[1A\033[2C", 9);
			break ;
		}
		ft_putendl_fd(line, fd[1]);
		free(line);
	}
	close(fd[1]);
	if (g_exit_status == 1)
		close(fd[0]);
	fd[1] = 1;
	if (line)
		free(line);
}