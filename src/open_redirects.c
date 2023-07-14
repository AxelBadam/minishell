/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:11:26 by ekoljone          #+#    #+#             */
/*   Updated: 2023/07/14 13:15:42 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

int	open_output_redirect(char *redirect, char *filename, int *fd)
{
	if (!*filename)
		if (!print_error(": no such file or directory\n", 1, filename))
			return (-1);
	if (access(filename, F_OK) == 0 && access(filename, W_OK) == -1
		&& (ft_strncmp(redirect, ">", SIZE_MAX) == 0
			|| ft_strncmp(redirect, ">>", SIZE_MAX) == 0
			|| ft_strncmp(redirect, ">|", SIZE_MAX) == 0))
		if (!print_error(": permission denied\n", 1, filename))
			return (-1);
	if (ft_strncmp(redirect, ">", SIZE_MAX) == 0 \
	|| ft_strncmp(redirect, ">|", SIZE_MAX) == 0)
		fd[1] = open(filename, O_CREAT | O_WRONLY, 0644);
	else if (ft_strncmp(redirect, ">>", SIZE_MAX) == 0
		|| ft_strncmp(redirect, "|>>", SIZE_MAX) == 0)
		fd[1] = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd[1] == -1 || fd[0] == -1)
		if (!print_error("fatal: open fail\n", 1, NULL))
			return (-1);
	return (0);
}

int	open_input_redirect(char *redirect, char *filename, int *fd)
{
	if (ft_strncmp(redirect, "<<", SIZE_MAX) == 0)
	{
		if (pipe(fd) == -1)
			if (!print_error("fatal: pipe fail\n", 1, NULL))
				return (-1);
		create_heredoc(fd, filename);
		if (g_exit_status == 1)
			return (-1);
	}
	else if (ft_strncmp(redirect, "<", SIZE_MAX) == 0)
	{
		if (access(filename, F_OK) != 0
			|| (access(filename, F_OK) == 0 && access(filename, R_OK) == -1))
		{
			if (access(filename, F_OK) == -1)
			{
				if (!print_error(": no such file or directory\n", 1, filename))
					return (-1);
			}
			else if (!print_error(": permission denied\n", 1, filename))
				return (-1);
		}
		fd[0] = open(filename, O_RDONLY);
	}
	return (0);
}

char	*get_filename(t_resrc *rs, char *filename)
{
	int		tmp;
	char	*f_name;

	if (!filename)
		if (!print_error("syntax error near unexpected token `newline'\n", \
			258, filename))
			return (NULL);
	f_name = ft_strdup(filename);
	if (!f_name)
		error_exit("minishell: fatal malloc error\n", rs);
	tmp = str_len_without_quotes(f_name);
	if (tmp != ft_strlen(f_name))
		f_name = make_new_str(f_name, tmp);
	if (!f_name)
		error_exit("minishell: fatal malloc error\n", rs);
	return (f_name);
}

int	open_file(t_resrc *rs, char *redirect, char *filename, int *fd)
{
	int		tmp;
	char	*f_name;
	int		prev_fd[2];

	prev_fd[0] = fd[0];
	prev_fd[1] = fd[1];
	tmp = 1;
	f_name = get_filename(rs, filename);
	if (!f_name)
		return (-1);
	if (is_a_directory(f_name))
	{
		if (!print_error(": is a directory\n", 258, f_name))
			tmp = -1;
	}
	else if (open_input_redirect(redirect, f_name, fd) == -1)
		tmp = -1;
	else if (open_output_redirect(redirect, f_name, fd) == -1)
		tmp = -1;
	if (prev_fd[0] != 0 && prev_fd[0] != fd[0])
		close(prev_fd[0]);
	if (prev_fd[1] != 1 && prev_fd[1] != fd[1])
		close(prev_fd[1]);
	free(f_name);
	return (tmp);
}
