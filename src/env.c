/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:07:24 by ekoljone          #+#    #+#             */
/*   Updated: 2023/07/14 13:15:47 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_env(t_resrc *resrc)
{
	char	*prefix;
	char	*last_arg;
	int		size;
	char	*str;

	if (resrc->list->next)
	{
		replace_str("_=", resrc->envp);
		return ;
	}
	prefix = "_=";
	size = get_2d_array_size(resrc->list->command.full_cmd);
	last_arg = resrc->list->command.full_cmd[size - 1];
	if (resrc->list->command.full_path)
		str = ft_strjoin(prefix, resrc->list->command.full_path);
	else
		str = ft_strjoin(prefix, last_arg);
	if (!str)
		error_exit("minishell: fatal malloc error\n", resrc);
	if (is_in_env(str, resrc->envp))
		resrc->envp = replace_str(str, resrc->envp);
	else if (ft_strchr(str, '=') != NULL)
		resrc->envp = append_2d(resrc->envp, str);
	free(str);
}

void	iterate_env(char **env, char *d_string, int *ctr)
{
	char	*str;

	ctr[1] = 0;
	while (env[ctr[0]])
	{
		while (env[ctr[0]][ctr[1]] != '=')
			ctr[1]++;
		str = ft_substr(env[ctr[0]], 0, ctr[1]);
		if (ft_strncmp(str, d_string, SIZE_MAX) == 0)
		{
			free(str);
			break ;
		}
		free(str);
		ctr[0]++;
		ctr[1] = 0;
	}
}

char	*get_env(char *d_string, char **env)
{
	int		ctr[3];
	int		len;
	char	*str;

	ctr[0] = 0;
	ctr[2] = 0;
	len = 0;
	str = NULL;
	iterate_env(env, d_string, ctr);
	ctr[1] = 0;
	if (env[ctr[0]])
	{
		while (env[ctr[0]][ctr[1]] != '=')
			ctr[1]++;
		len = ft_strlen(&env[ctr[0]][++ctr[1]]);
		str = (char *)malloc(sizeof(char) * (len + 1));
		if (!str)
			return (NULL);
		while (env[ctr[0]][ctr[1]])
			str[ctr[2]++] = env[ctr[0]][ctr[1]++];
		str[ctr[2]] = 0;
	}
	return (str);
}

char	*shlvl(char *sys_shlvl)
{
	char	shlvl[7];
	int		lvl;
	char	*level;
	char	*program_shlvl;

	lvl = ft_atoi(&sys_shlvl[6]);
	lvl++;
	ft_strlcpy(shlvl, sys_shlvl, 7);
	level = ft_itoa(lvl);
	program_shlvl = ft_strjoin(shlvl, level);
	free(level);
	if (!program_shlvl)
		error_exit("minishell: fatal malloc error\n", NULL);
	return (program_shlvl);
}

char	**create_env(char **env)
{
	int		ctr[2];
	char	**envp;

	ctr[0] = 0;
	ctr[1] = 0;
	envp = (char **)malloc(sizeof(char *) * (get_array_size(env) + 1));
	if (!envp)
		error_exit("minishell: fatal malloc error\n", NULL);
	while (env[ctr[0]])
	{
		if (ft_strncmp("SHLVL", env[ctr[0]], 5) == 0)
			envp[ctr[0]] = shlvl(env[ctr[0]]);
		else
			envp[ctr[0]] = ft_strdup(env[ctr[0]]);
		if (!envp[ctr[0]++])
			error_exit("minishell: fatal malloc error\n", NULL);
	}
	envp[ctr[0]] = 0;
	return (envp);
}
