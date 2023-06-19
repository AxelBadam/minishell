/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:31:57 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/19 13:51:12 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

void	siginthandler(int signal)
{
	if (signal == SIGINT)
		write(1, "\n", 1);
}

void	close_echo_control(struct termios *t)
{
	t->c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, t);
}

void	open_echo_control(struct termios *t)
{
	t->c_lflag |= ECHOCTL;
	tcsetattr(0, TCSANOW, t);
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
