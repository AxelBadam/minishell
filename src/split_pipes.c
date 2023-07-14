/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 13:43:52 by ekoljone          #+#    #+#             */
/*   Updated: 2023/07/14 13:15:40 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(char **array)
{
	int	ctr[2];
	int	count;

	ctr[0] = 0;
	ctr[1] = 0;
	count = 0;
	while (array[ctr[0]])
	{
		if (array[ctr[0]][0] == '>' || array[ctr[0]][0] == '<'
			|| array[ctr[0]][0] == '|')
		{
			if (array[ctr[0]][0] == '|' && array[ctr[0]][1])
				count++;
		}
		ctr[0]++;
	}
	return (ctr[0] + count);
}

char	**fill_pipe_array(char **n_arr, char **o_arr)
{
	int		ctr[2];
	int		i;

	ctr[0] = 0;
	ctr[1] = 0;
	i = 0;
	while (o_arr[ctr[0]])
	{
		if (o_arr[ctr[0]][ctr[1]] == '|' && ft_strlen(o_arr[ctr[0]]) > 1)
		{
			while (o_arr[ctr[0]][ctr[1]] == '|')
				ctr[1]++;
			n_arr[i++] = ft_substr(o_arr[ctr[0]], 0, ctr[1]);
			if (!n_arr[i - 1])
				return (NULL);
		}
		n_arr[i++] = ft_substr(o_arr[ctr[0]++], ctr[1], SIZE_MAX);
		if (!n_arr[i - 1])
			return (NULL);
		ctr[1] = 0;
	}
	n_arr[i] = 0;
	return (n_arr);
}

char	**split_pipes(char **array)
{
	char	**n_arr;
	int		strings;

	if (!array)
		return (NULL);
	strings = count_pipes(array);
	if (strings == get_array_size(array))
		return (array);
	n_arr = (char **)malloc(sizeof(char *) * (strings + 1));
	if (!n_arr)
		return (NULL);
	fill_pipe_array(n_arr, array);
	free_string_array(array);
	return (n_arr);
}
