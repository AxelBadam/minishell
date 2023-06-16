/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:20:59 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/16 14:14:43 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "termios.h"

int	g_exit_status;

void	print_list(t_list **head)
{
	t_list	*tmp;
	int		ctr;

	ctr = 0;
	tmp = *head;
	while (tmp)
	{
		printf("FULL CMD = ");
		while (tmp->command.full_cmd[ctr])
			printf("%s ", tmp->command.full_cmd[ctr++]);
		printf("\nFULL PATH = %s\n", tmp->command.full_path);
		printf("OUTPUT_FD = %i\nINPUT_FD = %i\n",
			tmp->command.output_fd, tmp->command.input_fd);
		tmp = tmp->next;
		ctr = 0;
	}
}

void	minishell(t_resrc *resrc)
{
	char	*line;

	line = readline("minishell-1.0$ ");
	while (line)
	{
		if (*line)
		{
			resrc->array = split_command(resrc, line);
			add_history(line);
			if (resrc->array)
			{
				make_list(resrc, resrc->array);
				if (resrc->list)
				{
					set_env(resrc);
					execution(resrc, resrc->list);
				}
				free_string_array(resrc->array);
				free_all_nodes(&resrc->list);
			}
		}
		free(line);
		line = readline("minishell-1.0$ ");
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

void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		g_exit_status = 1;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
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
	free(resrc);
	return (g_exit_status);
}
