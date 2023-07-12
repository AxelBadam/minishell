/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:51:25 by atuliara          #+#    #+#             */
/*   Updated: 2023/07/12 12:15:26 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

void	execute_builtin_unset(t_list *list, t_resrc *resrc)
{
	int	ac;

	ac = 1;
	g_exit_status = 0;
	while (list->command.full_cmd[ac])
	{
		if (check_identifier(list->command.full_cmd[ac], 0)
			&& is_in_env(list->command.full_cmd[ac], resrc->envp)
			&& !ft_strchr(list->command.full_cmd[ac], '='))
		{
			resrc->envp = rmv_str_twod(resrc->envp, list->command.full_cmd[ac]);
			if (resrc->envp == NULL)
				error_exit("malloc error\n", resrc);
		}
		ac++;
	}
}

void	execute_builtin_echo(t_command cmd)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	while (*cmd.full_cmd != 0 && cmd.full_cmd[i] != 0 \
	&& check_for_option(cmd.full_cmd[i]))
	{
		newline = 0;
		i++;
	}
	while (cmd.full_cmd[i] != 0)
	{
		ft_putstr_fd(cmd.full_cmd[i], STDOUT_FILENO);
		if (cmd.full_cmd[++i] != 0)
			write(1, " ", 1);
	}
	if (newline)
		ft_putstr_fd("\n", 1);
	g_exit_status = 0;
}

void	execute_builtin_pwd(t_resrc *rs)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(cwd, sizeof(cwd));
	if (!cwd)
		cwd = get_env("PWD", rs->envp);
	if (!cwd)
		print_error(": pwd failed\n", 1, "pwd");
	else
		ft_putendl_fd(cwd, 1);
	free(cwd);
}

int	child_numeric(char *s, int child)
{
	int	ctr;

	ctr = 0;
	if (s[ctr] == '-' || s[ctr] == '+')
		ctr++;
	while (s[ctr])
	{
		if (!ft_isdigit(s[ctr]))
		{
			if (!child)
				ft_putendl_fd("exit", 2);
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(s, 2);
			ft_putendl_fd(": numeric argument required", 2);
			g_exit_status = 255;
			return (0);
		}
		ctr++;
	}
	return (1);
}

void	execute_builtin_exit(char **array, int child)
{
	int	ctr;

	ctr = 1;
	while (array[ctr])
		if (!child_numeric(array[ctr++], child))
			exit(g_exit_status);
	if (get_array_size(array) > 2)
	{
		if (!print_error("exit: too many arguments\n", 1, NULL))
			if (child)
				exit(g_exit_status);
	}
	else
	{
		if (!child)
			write(1, "exit\n", 5);
		if (array[1])
			exit(ft_atoi(array[1]));
		else
			exit(g_exit_status);
	}
}
