/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 12:50:33 by atuliara          #+#    #+#             */
/*   Updated: 2023/06/01 17:22:17 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

int execute_builtin_cd(t_list *list)
{
	char *path;
	
	path = list->command.full_cmd[1];
	if (is_a_directory(path) && access(path, R_OK | X_OK) == 0) 
	{
		g_exit_status = chdir(path);
	}
	if (g_exit_status != 0)
		print_error("cd error", g_exit_status, path);
	return (g_exit_status);
}

int execute_builtin_pwd()
{
    char cwd[4096];

    if (getcwd(cwd, sizeof(cwd)) != NULL) 
    {
        write(STDOUT_FILENO, cwd, ft_strlen(cwd));
        write(STDOUT_FILENO, "\n", 1);
    } 
    else 
		error_handling("pwd error");
	g_exit_status = 1338; //?
	return (g_exit_status);
}

int execute_builtin_exit()
{
	write(1, "exit\n", 5);
	exit (0);
}

int execute_builtin_env(char **envp)
{
	char **tmp;

	tmp = envp;
	while (*tmp)
	{
		ft_putstr_fd(*tmp++, 1);
		ft_putstr_fd("\n", 1);
	}
	return (0);
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

int execute_builtin_export(t_list *list, t_resrc *resrc)
{
	int j;

	j = 1;
	while (list->command.full_cmd[j])
	{
		if (is_in_env(list->command.full_cmd[j], resrc->envp))
			resrc->envp = replace_str(list->command.full_cmd[j], resrc->envp);
		else if (ft_strchr(list->command.full_cmd[j], '=') != NULL)
			resrc->envp = append_2d(resrc->envp, list->command.full_cmd[j]);
		j++;
	}
	return (1);
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

int execute_builtin_unset(t_list *list, t_resrc *resrc)
{
	int ac;

	ac = 1;
	while (list->command.full_cmd[ac] && is_in_env(list->command.full_cmd[ac], resrc->envp))
	{
		resrc->envp = rmv_str_twod(resrc->envp, list->command.full_cmd[ac]);
		ac++;
	}
	return (1);
}

int execute_builtin_echo(t_command cmd)
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
    return (0);
}
