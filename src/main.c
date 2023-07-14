/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:20:59 by ekoljone          #+#    #+#             */
/*   Updated: 2023/07/14 13:15:41 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

void	command_line(struct termios *t, t_resrc *resrc, char *line)
{
	if (*line)
	{
		resrc->array = split_command(resrc, line);
		add_history(line);
		if (resrc->array)
		{
			if (!make_list(resrc, resrc->array))
			{
				open_echo_control(t);
				if (resrc->list)
				{
					set_env(resrc);
					execution(resrc, resrc->list);
					free_all_nodes(resrc);
				}
			}
		}
	}
}

void	minishell(t_resrc *resrc)
{
	char			*line;
	struct termios	t;

	tcgetattr(0, &t);
	while (1)
	{
		close_echo_control(&t);
		line = readline("minishell-1.0$ ");
		if (!line)
			break ;
		command_line(&t, resrc, line);
		free(line);
	}
	ft_putstr_fd("\033[1Aminishell-1.0$ exit\n", 1);
}

void	*init_resources(char **envp)
{
	t_resrc	*resrc;

	resrc = (t_resrc *)malloc(sizeof(t_resrc));
	if (!resrc)
		error_exit("minishell: fatal malloc error\n", NULL);
	resrc->envp = envp;
	resrc->list = NULL;
	resrc->array = NULL;
	return (resrc);
}

int	main(int argc, char **argv, char **env)
{
	t_resrc	*resrc;

	(void)argc;
	(void)argv;
	g_exit_status = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	resrc = init_resources(create_env(env));
	minishell(resrc);
	free_string_array(resrc->envp);
	if (resrc)
		free(resrc);
	return (g_exit_status);
}
