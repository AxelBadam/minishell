/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:30:05 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/16 13:06:25 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

char	**get_new_command(t_resrc *resource)
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
		{
			line = readline("> ");
			if (!*line)
				free(line);
		}
		pipe_command = split_command(resource, line);
		free(line);
		return (pipe_command);
	}
	free_all_nodes(&resource->list);
	return (NULL);
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
		line = NULL;
	}
	close(fd[1]);
	if (g_exit_status == 1)
		close(fd[0]);
	fd[1] = 1;
	if (line)
		free(line);
}
