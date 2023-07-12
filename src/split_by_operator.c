/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_by_operator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:01:17 by ekoljone          #+#    #+#             */
/*   Updated: 2023/07/12 14:09:02 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_strings(char **array)
{
	int	ctr[2];
	int	strings;

	strings = 0;
	ctr[0] = 0;
	ctr[1] = 0;
	while (array[ctr[0]])
	{
		while (array[ctr[0]][ctr[1]])
		{
			if (array[ctr[0]][ctr[1]] == '"' || array[ctr[0]][ctr[1]] == '\'')
			{
				ctr[1]++;
				iterate_quotes(array[ctr[0]],
					&ctr[1], array[ctr[0]][ctr[1] - 1], 0);
			}
			strings = count(array, ctr, strings);
			if (array[ctr[0]][ctr[1]] && (array[ctr[0]][ctr[1]] != '"'
				&& array[ctr[0]][ctr[1]] != '\''))
				ctr[1]++;
		}
		ctr[1] = 0;
		ctr[0]++;
	}
	return (strings);
}

int	fill(char **n_arr, char **o_arr, int *c, int *i)
{
	check_for_quotes(o_arr, c);
	if (o_arr[c[0]][c[1]] == '<' || o_arr[c[0]][c[1]] == '>'
		|| o_arr[c[0]][c[1]] == '|')
	{
		if (c[1])
		{
			n_arr[i[0]++] = ft_substr(o_arr[c[0]], i[1], c[1] - i[1]);
			if (!n_arr[i[0] - 1])
				return (-1);
			i[1] = c[1];
		}
		while (o_arr[c[0]][c[1]] && (o_arr[c[0]][c[1]] == '<'
			|| o_arr[c[0]][c[1]] == '>' || o_arr[c[0]][c[1]] == '|'))
			c[1]++;
		n_arr[i[0]++] = ft_substr(o_arr[c[0]], i[1], c[1] - i[1]);
		if (!n_arr[i[0] - 1])
			return (-1);
		i[1] = c[1];
		if (!o_arr[c[0]][c[1]])
			return (1);
	}
	if (get_last_string(o_arr, n_arr, c, i) == -1)
		return (-1);
	return (0);
}

int	fill_array_with_operators(char **n_arr, char **o_arr)
{
	int	ctr[2];
	int	index[2];
	int	fill_check;

	ctr[0] = 0;
	ctr[1] = -1;
	index[0] = 0;
	index[1] = 0;
	while (o_arr[ctr[0]])
	{
		while (o_arr[ctr[0]][++ctr[1]])
		{
			fill_check = fill(n_arr, o_arr, ctr, index);
			if (fill_check == -1)
				return (-1);
			else if (fill_check == 1)
				break ;
		}
		index[1] = 0;
		ctr[1] = -1;
		ctr[0]++;
	}
	n_arr[index[0]] = 0;
	return (0);
}

char	**make_array_with_operators(char **array, int strings)
{
	int		ctr;
	char	**n_arr;

	ctr = 0;
	while (array[ctr])
		ctr++;
	n_arr = (char **)malloc(sizeof(char *) * (ctr + strings + 1));
	if (!n_arr || fill_array_with_operators(n_arr, array) == -1)
		return (NULL);
	return (n_arr);
}

char	**split_by_operator(char **array)
{
	int		strings;
	char	**n_arr;

	strings = count_strings(array);
	if (!strings)
		return (array);
	n_arr = make_array_with_operators(array, strings);
	free_string_array(array);
	return (n_arr);
}
