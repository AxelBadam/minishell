/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:20:59 by ekoljone          #+#    #+#             */
/*   Updated: 2023/04/26 17:49:34 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_output(char *file_name, char *line)
{
	int	fd;

	if (file_name)
	{
		fd = open(file_name, O_WRONLY | O_CREAT, 0644);
		ft_putstr_fd(line, fd);
	}
	else
		ft_printf("zsh: parse error near `\\n'\n");
}

/*void	redirect_echo_output(char *file_name, char **s_line)
{
	int	fd;

	if (file_name)
	{
		fd = open(file_name, O_WRONLY | O_CREAT, 0644);
		ft_putstr_fd(line, fd);
	}
	else
		ft_printf("zsh: parse error near `\\n'\n");
}*/

void	change_directory(char **arg)
{
	if (!arg[1])
		return ;
	else if (ft_strncmp(arg[1], ">", SIZE_MAX) == 0)
		redirect_output(arg[2], "");
	else if (ft_strncmp(arg[2], ">", SIZE_MAX) == 0)
	{
		if (ft_strncmp(arg[1], "..", SIZE_MAX) != 0)
			redirect_output(arg[3], "");
		if (chdir(arg[1]) != 0)
			ft_printf("cd: no such file or directory: %s\n", arg[3]);
	}
}

void	pwd(char **arg)
{
	char	*buf;

	buf = NULL;
	buf = getcwd(buf, SIZE_MAX);
	if (arg[1])
	{
		if (ft_strncmp(arg[1], ">", SIZE_MAX) == 0)
			redirect_output(arg[2], buf);
		else
			ft_printf("pwd: too many arguments\n");
	}
	else
		ft_printf("%s\n", buf);
	free(buf);
}

void	echo(char **arg)
{
	int	counter;

	counter = 0;
	if (!arg[1])
		return ;
	if (ft_strncmp(arg[1], "-n", SIZE_MAX) == 0)
	{
		if (!arg[2])
			return ;
		else if (ft_strncmp(arg[2], ">", SIZE_MAX) == 0)
			redirect_output(arg[3], "");
		ft_printf("%s", arg[2]);
	}
	else
	{
		if (ft_strncmp(arg[1], ">", SIZE_MAX) == 0)
			redirect_output(arg[2], "\n");
		while (arg[++counter])
		{
			ft_printf("%s", arg[counter]);
			if (arg[counter + 1])
				ft_putchar_fd(' ', 1);
		}
		ft_putchar_fd('\n', 1);
	}
}

void	check_argument(char **arg)
{
	if (ft_strncmp(arg[0], "pwd", SIZE_MAX) == 0)
		pwd(arg);
	else if (ft_strncmp(arg[0], "cd", SIZE_MAX) == 0)
		change_directory(arg);
	else if (ft_strncmp(arg[0], "echo", SIZE_MAX) == 0)
		echo(arg);
}

void	minishell(t_resrc *resrc)
{
	resrc->line = readline("");
	while (resrc->line)
	{
		add_history(resrc->line);
		resrc->s_line = ft_split(resrc->line, ' ');
		check_argument(resrc->s_line);
		free(resrc->line);
		resrc->line = readline("");
	}
}

void	*init_resources(void)
{
	t_resrc	*resrc;

	resrc = (t_resrc *)malloc(sizeof(t_resrc));
	if (!resrc)
		return NULL;
	resrc->buf = NULL;
	resrc->line = NULL;
	resrc->s_line = NULL;
	resrc->history = NULL;
	return (resrc);
}

int	main()
{
	t_resrc	*resrc;

	resrc = init_resources();
	minishell(resrc);
	return (0);
}
