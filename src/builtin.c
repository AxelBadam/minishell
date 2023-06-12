/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 12:50:33 by atuliara          #+#    #+#             */
/*   Updated: 2023/06/12 13:29:49 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

int update_env(char *var, char *val, t_resrc *resrc)
{
	char *env_var;

	env_var = ft_strjoin(var, val);
	if (is_in_env(env_var, resrc->envp))
		resrc->envp = replace_str(env_var, resrc->envp);
	else if (ft_strchr(env_var, '=') != NULL)
		resrc->envp = append_2d(resrc->envp, env_var);
	free(env_var);
	return (1);
}
	
void cd_error(char *path)
{
	if (!is_a_directory(path) && access(path, F_OK) == 0)
		print_error(": not a directory\n", 1, path);
	if (access(path, F_OK) == -1)
		print_error(": no such file or directory\n", 2, path);
	else if (access(path, X_OK) == -1)
		print_error(": permission denied\n", 1, path);
}

void execute_builtin_cd(t_resrc *resrc, t_command command)
{
	char *path;
	char pwd[4096];
	
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

void execute_builtin_pwd()
{
    char *cwd;

	cwd = NULL;
	cwd = getcwd(cwd, sizeof(cwd));
    if (cwd != NULL) 
    {
        write(STDOUT_FILENO, cwd, ft_strlen(cwd));
        write(STDOUT_FILENO, "\n", 1);
		g_exit_status = 0;
    } 
    else 
		print_error(": getcwd failed", 1, "pwd");
	free(cwd);
}

void execute_builtin_exit()
{
	write(1, "exit\n", 5);
	exit (g_exit_status);
}

void execute_builtin_env(char **envp)
{
	char **tmp;

	tmp = envp;
	while (*tmp)
	{
		ft_putstr_fd(*tmp++, 1);
		ft_putstr_fd("\n", 1);
	}
	g_exit_status = 0;
}

int is_in_env(char *str, char **envp)
{
	int len;
	int count;
	int i;

	i = 0;
	count = 0;
	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	while (envp[i] && str[0] != 0)
	{
		if (!ft_strncmp(envp[i], str, len) && envp[i][len] == '=')
			count++;
		i++;
	}
	return (count);
}

char **replace_str(char *str, char **envp)
{
	int len;
	int count;
	int i;

	len = 0;
	i = 0;
	count = 0;
	while (str[len] != '=')
		len++;
	while (envp[i] && str[0] != 0)
	{
		if (!ft_strncmp(envp[i], str, len) && envp[i][len] == '=')
		{
			free(envp[i]);
			envp[i] = ft_strdup(str);
		}
		i++;
	}
	return (envp);
}

char **append_2d(char **twod, char *str_to_add)
{
	char **new;
	int i;

	i = 0;
	while(twod[i])
		i++;
	new = (char **)malloc(sizeof(char *) * (i + 2));
	i = -1;
	while (twod[++i])
		new[i] = ft_strdup(twod[i]);
	new[i] = ft_strdup(str_to_add);
	new[++i] = 0;
	free_string_array(twod);
	return (new);
}

void execute_builtin_export(t_list *list, t_resrc *resrc)
{
	int j;
	int i;

	j = 1;
	while (list->command.full_cmd[j])
	{
		if (is_in_env(list->command.full_cmd[j], resrc->envp))
			resrc->envp = replace_str(list->command.full_cmd[j], resrc->envp);
		else if (ft_strchr(list->command.full_cmd[j], '=') != NULL)
			resrc->envp = append_2d(resrc->envp, list->command.full_cmd[j]);
		j++;
	}
	i = 0;
	if (!list->command.full_cmd[1])
		while (resrc->envp[i])
			printf("declare -x %s\n", resrc->envp[i++]);
	g_exit_status = 0;
}

char **rmv_str_twod(char **env, char *to_rmv)
{
	char **new;
	int i;
	int len;
	int j;

	i = 0;
	j = 0;
	len = ft_strlen(to_rmv);
	while(env[i])
		i++;
	new = (char **)malloc(sizeof(char *) * \
	(i + 1 - is_in_env(to_rmv, env)));
	i = 0;
	while (env[i])
	{			
		if (ft_strnstr(env[i], to_rmv, len))
			i++;
		new[j++] = ft_strdup(env[i++]);
	}
	new[j] = 0;
	free_string_array(env);
	return(new);
}

void execute_builtin_unset(t_list *list, t_resrc *resrc)
{
	int ac;

	ac = 1;
	while (list->command.full_cmd[ac] && is_in_env(list->command.full_cmd[ac], resrc->envp))
	{
		resrc->envp = rmv_str_twod(resrc->envp, list->command.full_cmd[ac]);
		ac++;
	}
	g_exit_status = 0;
}

void execute_builtin_echo(t_command cmd)
{
    int newline;
	int i;

	newline = 1;
	i = 1;
    if (*cmd.full_cmd != 0 && cmd.full_cmd[i] != 0 \
	&& ft_strncmp(cmd.full_cmd[i], "-n", 2) == 0)
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
