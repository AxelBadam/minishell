/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 12:50:33 by atuliara          #+#    #+#             */
/*   Updated: 2023/05/26 18:08:55 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

int execute_builtin_cd(t_list *list)
{
	char *path;
	
	path = list->command.full_cmd[1];
    if (chdir(path) != 0)
	{
        error_handling("cd erore");
    }
    g_exit_status = 1337;
	return (1);
}

void execute_builtin_pwd()
{
    char cwd[4096];

    if (getcwd(cwd, sizeof(cwd)) != NULL) 
    {
        write(STDOUT_FILENO, cwd, ft_strlen(cwd)); // using STDOUT_FILENO
        write(STDOUT_FILENO, "\n", 1); // using STDOUT_FILENO
    } 
    else 
		error_handling("pwd error");
	g_exit_status = 1338;
}

int execute_builtin_exit()
{
	write(1, "exit\n", 5);
	exit (0);
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
}

int is_in_env(char *str, char **envp)
{
	int len;
	int count;
	int i;

	i = 0;
	count = 0;
	len = ft_strlen(str);
	while (envp[i] && str[0] != 0)
	{
		if (ft_strncmp(envp[i], str, len) == 0 && envp[i][len] == '=')
			count++;
		i++;
	}
	return (count);
}

char **append_twod(char **twod, char *str_to_add)
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
	new[i] = str_to_add;
	new[++i] = 0;
	free_string_array(twod);
	return (new);
}

int execute_builtin_export(t_list *list, t_resrc *resrc)
{
	char *str;
	int j;

	j = 1;
	while (list->command.full_cmd[j])
	{
		str = list->command.full_cmd[j];
		if (ft_strchr(str, '=') != NULL)
			resrc->envp = append_twod(resrc->envp, str);
		j++;
	}
	return (1);
}

int execute_builtin_unset(t_list *list, t_resrc *resrc)
{
	int i = 0;
	int j = 0;
	char **new_env;
	int len;
	
	len = ft_strlen(list->command.full_cmd[1]);
	if (is_in_env(list->command.full_cmd[1], resrc->envp))
	{
		while(resrc->envp[i])
			i++;
		new_env = (char **)malloc(sizeof(char *) * \
		(i + 1 - is_in_env(list->command.full_cmd[1], resrc->envp)));
		i = 0;
		while (resrc->envp[i])
		{
			if (ft_strnstr(resrc->envp[i], list->command.full_cmd[1], len))
				i++;
		new_env[j++] = ft_strdup(resrc->envp[i]);
		free(resrc->envp[i]);
		i++;
		}
	new_env[j] = 0;
	free(resrc->envp);
	resrc->envp = new_env;
	}
	return (1);
}

int execute_builtin_echo(t_command cmd)
{
    int newline;
	int i;

	newline = 1;
	i = 1;
    if (*cmd.full_cmd != 0 && cmd.full_cmd[i] != 0 && ft_strncmp(cmd.full_cmd[i], "-n", 2) == 0)
    {
		newline = 0;
		i++;
	}
    while (cmd.full_cmd[i] != 0)
    {
    	ft_putstr_fd(cmd.full_cmd[i], STDOUT_FILENO); // using STDOUT_FILENO
        if (cmd.full_cmd[++i] != 0)
            write(1, " ", 1); // using STDOUT_FILENO
    }
    if (newline)
    {
        ft_putstr_fd("\n", 1); // using STDOUT_FILENO
    }
    return 0;
}
