/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_by_operator_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 16:31:26 by ekoljone          #+#    #+#             */
/*   Updated: 2023/07/12 13:44:50 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_for_quotes(char **o_arr, int *c)
{
	if (o_arr[c[0]][c[1]] == '"' || o_arr[c[0]][c[1]] == '\'')
	{
		c[1]++;
		iterate_quotes(o_arr[c[0]], &c[1], o_arr[c[0]][c[1] - 1], 0);
	}
}

int	get_last_string(char **o_arr, char **n_arr, int *c, int *i)
{
	if (!o_arr[c[0]][c[1] + 1])
	{
		n_arr[i[0]++] = ft_substr(o_arr[c[0]], i[1], c[1] + 1 - i[1]);
		if (!n_arr[i[0] - 1])
			return (-1);
	}
	return (0);
}
