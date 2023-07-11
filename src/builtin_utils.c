/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 12:50:33 by atuliara          #+#    #+#             */
/*   Updated: 2023/07/11 13:26:02 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**rmv_str_twod(char **env, char *to_rmv)
{
	char	**new;
	int		i;
	int		len;
	int		j;

	i = 0;
	j = 0;
	len = ft_strlen(to_rmv);
	while (env[i])
		i++;
	new = (char **)malloc(sizeof(char *)
			* (i + 1 - is_in_env(to_rmv, env)));
	i = 0;
	while (env[i])
	{			
		if (!ft_strncmp(env[i], to_rmv, len) && env[i][len] == '=')
			i++;
		if (env[i])
			new[j++] = ft_strdup(env[i++]);
		if (new[j - 1] == NULL)
			return (NULL);
	}
	new[j] = 0;
	free_string_array(env);
	return (new);
}

char	**append_2d(char **twod, char *str_to_add)
{
	char	**new;
	int		i;

	i = 0;
	while (twod[i])
		i++;
	new = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new)
		return (NULL);
	i = -1;
	while (twod[++i])
	{
		new[i] = ft_strdup(twod[i]);
		if (!new[i])
			return (NULL);
	}
	new[i] = ft_strdup(str_to_add);
	if (!new[i])
		return (NULL);
	new[++i] = 0;
	free_string_array(twod);
	return (new);
}

char	**replace_str(char *str, char **envp)
{
	int	len;
	int	count;
	int	i;

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
			if (!envp[i])
				return (NULL);
		}
		i++;
	}
	return (envp);
}

int	is_in_env(char *str, char **envp)
{
	int	len;
	int	count;
	int	i;

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

int	update_env(char *var, char *val, t_resrc *resrc)
{
	char	*env_var;

	if (val == NULL)
		return (1);
	env_var = ft_strjoin(var, val);
	if (is_in_env(env_var, resrc->envp))
		resrc->envp = replace_str(env_var, resrc->envp);
	else if (ft_strchr(env_var, '=') != NULL)
		resrc->envp = append_2d(resrc->envp, env_var);
	if (resrc->envp == NULL)
		error_exit("malloc error", resrc);
	free(env_var);
	return (1);
}
