/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 12:50:33 by atuliara          #+#    #+#             */
/*   Updated: 2023/06/19 12:52:29 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

void	cd_error(char *path)
{
	if (!is_a_directory(path) && access(path, F_OK) == 0)
		print_error(": not a directory\n", 1, path);
	if (access(path, F_OK) == -1)
		print_error(": no such file or directory\n", 2, path);
	else if (access(path, X_OK) == -1)
		print_error(": permission denied\n", 1, path);
}

void	execute_builtin_cd(t_resrc *resrc, t_command command)
{
	char	*path;
	char	pwd[4096];

	if (!command.full_cmd[1])
		path = get_env("HOME", resrc->envp);
	else
		path = command.full_cmd[1];
	if (path == NULL)
		print_error(": HOME not set", 1, "cd");
	if (is_a_directory(path) && access(path, X_OK) == 0)
	{
		getcwd(pwd, sizeof(pwd));
		update_env("OLDPWD=", pwd, resrc);
		chdir(path);
		g_exit_status = 0;
		getcwd(pwd, sizeof(pwd));
		update_env("PWD=", pwd, resrc);
	}
	else
		cd_error(path);
	if (!command.full_cmd[1])
		free(path);
}

void	execute_builtin_env(char **envp)
{
	char	**tmp;

	tmp = envp;
	while (*tmp)
	{
		ft_putstr_fd(*tmp++, 1);
		ft_putstr_fd("\n", 1);
	}
	g_exit_status = 0;
}

void	execute_builtin_export(t_list *list, t_resrc *resrc)
{
	int	j;

	j = 1;
	g_exit_status = 0;
	while (list->command.full_cmd[j])
	{
		if (export_check(list->command.full_cmd[j]))
		{
			if (is_in_env(list->command.full_cmd[j], resrc->envp))
				resrc->envp = replace_str(list->command.full_cmd[j], \
				resrc->envp);
			else if (ft_strchr(list->command.full_cmd[j], '=') != NULL)
				resrc->envp = append_2d(resrc->envp, list->command.full_cmd[j]);
		}
		j++;
	}
	if (!list->command.full_cmd[1])
		declare_env(resrc);
}

void	execute_builtin(t_resrc *resrc, t_list *list)
{
	char	*cmd;
	int		len;

	len = 0;
	cmd = ft_strdup(*list->command.full_cmd);
	if (!cmd)
		error_exit("malloc error", resrc);
	cmd = str_to_lower(cmd);
	if (*list->command.full_cmd)
		len = ft_strlen(cmd);
	if (!ft_strncmp(cmd, "pwd", len) && len == 3)
		execute_builtin_pwd();
	else if (!ft_strncmp(cmd, "echo", len) && len == 4)
		execute_builtin_echo(list->command);
	else if (!ft_strncmp(cmd, "env", len) && len == 3)
		execute_builtin_env(resrc->envp);
	else if (!ft_strncmp(cmd, "export", 6) && len == 6 && \
	!list->command.full_cmd[1])
		execute_builtin_export(list, resrc);
	free(cmd);
}
