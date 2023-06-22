/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 12:34:29 by atuliara          #+#    #+#             */
/*   Updated: 2023/06/22 12:32:10 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_alpha_digit(char c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || ft_isdigit(c)
		|| c == '_')
		return (1);
	return (0);
}

int	check_identifier(char *arg, int export)
{
	int	i;

	i = 0;
	if (ft_isdigit(arg[0]) || arg[0] == '=')
	{
		print_error(": not a valid identifier\n", 1, arg);
		return (0);
	}
	if (export == 0 && ft_strchr(arg, '=') != NULL)
	{
		print_error(": not a valid identifier\n", 1, arg);
		return (0);
	}
	while (arg[i] != '=' && arg[i])
	{
		if (!is_alpha_digit(arg[i]))
		{
			print_error(": not a valid identifier\n", 1, arg);
			return (0);
		}
	i++;
	}
	return (1);
}

void	declare_env(t_resrc *resrc)
{
	int	j;

	j = 0;
	while (resrc->envp[j])
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(resrc->envp[j++], 1);
		ft_putstr_fd("\n", 1);
	}	
}
