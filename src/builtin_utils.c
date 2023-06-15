/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 12:50:33 by atuliara          #+#    #+#             */
/*   Updated: 2023/06/15 18:05:06 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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