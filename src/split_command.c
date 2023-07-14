/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 17:50:55 by ekoljone          #+#    #+#             */
/*   Updated: 2023/07/14 13:15:48 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

int	iterate_quotes(char *line, int *ctr, char d, int count)
{
	if (ft_strchr(&line[ctr[0]], d))
	{
		while (line[ctr[0]] != d)
		{
			ctr[0]++;
			if (count)
				ctr[1]++;
		}
		ctr[0]++;
		if (count)
			ctr[1]++;
		return (1);
	}
	print_error("syntax error unclosed quotes\n", 69, NULL);
	return (0);
}

int	count_words(char *line)
{
	int	ctr[2];

	ctr[0] = 0;
	ctr[1] = 0;
	while (line[ctr[0]])
	{
		while (ft_is_white_space(line[ctr[0]]))
			ctr[0]++;
		while (line[ctr[0]] && !ft_is_white_space(line[ctr[0]]))
		{
			ctr[0]++;
			if (line[ctr[0] - 1] == '"' || line[ctr[0] - 1] == '\'')
				if (!iterate_quotes(line, ctr, line[ctr[0] - 1], 0))
					return (0);
			if (ft_is_white_space(line[ctr[0]]) || !line[ctr[0]])
				ctr[1]++;
		}
	}
	return (ctr[1]);
}

void	fill_array(char *line, char **array)
{
	int	index[2];
	int	words;
	int	len;
	int	tmp[2];

	tmp[0] = 0;
	tmp[1] = 0;
	index[0] = 0;
	index[1] = 0;
	len = 0;
	words = count_words(line);
	while (words > index[0])
	{
		iterate_over_spaces(line, tmp);
		len = len_ctr(&line[tmp[0]]);
		tmp[1] = tmp[0];
		tmp[0] += len;
		while (--len > -1)
			array[index[0]][index[1]++] = line[tmp[1]++];
		array[index[0]][index[1]] = 0;
		index[1] = 0;
		index[0]++;
		iterate_over_spaces(line, tmp);
	}
	array[index[0]] = 0;
}

char	**make_array(t_resrc *rs, char *line)
{
	int		tmp[3];
	int		ctr;
	char	**array;

	ctr = 0;
	tmp[0] = 0;
	tmp[1] = 0;
	tmp[2] = count_words(line);
	if (!tmp[2])
		return (NULL);
	array = (char **)malloc(sizeof(char *) * (tmp[2] + 1));
	if (!array)
		error_exit("minishell: fatal malloc error\n", rs);
	while (tmp[2] > 0)
	{
		tmp[1] = len_ctr(&line[tmp[0]]);
		tmp[0] += tmp[1];
		array[ctr] = (char *)malloc(sizeof(char) * (tmp[1] + 1));
		if (!array[ctr++])
			error_exit("minishell: fatal malloc error\n", rs);
		while (ft_is_white_space(line[tmp[0]]))
			tmp[0]++;
		tmp[2]--;
	}
	return (array);
}

char	**split_command(t_resrc *rs, char *line)
{
	char	**array;

	array = make_array(rs, line);
	if (!array)
		return (NULL);
	fill_array(line, array);
	if (expand(array, rs->envp) == -1)
		error_exit("minishell: fatal malloc error\n", rs);
	array = split_by_operator(array);
	array = split_pipes(array);
	if (!array)
		error_exit("minishell: fatal malloc error\n", rs);
	return (array);
}
