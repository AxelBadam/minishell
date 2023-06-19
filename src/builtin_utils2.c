/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 12:34:29 by atuliara          #+#    #+#             */
/*   Updated: 2023/06/19 12:39:30 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_digit(char c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

int	is_alpha_digit(char c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || is_digit(c))
		return (1);
	return (0);
}

int	export_check(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '=' || is_digit(arg[0]))
	{
		print_error(" : export: not a valid identifier\n", 1, arg);
		return (0);
	}
	while (arg[i] != '=')
	{
		if (!is_alpha_digit(arg[i]))
		{
			print_error(" : export: not a valid identifier\n", 1, arg);
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
