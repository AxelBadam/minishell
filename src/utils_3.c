/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:31:52 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/09 18:33:29 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

int	len_ctr(char *line)
{
	int	ctr[2];

	ctr[0] = 0;
	ctr[1] = 0;
	while (line[ctr[0]])
	{
		while (line[ctr[0]] == ' ')
			ctr[0]++;
		while (line[ctr[0]] && line[ctr[0]] != ' ')
		{
			ctr[0]++;
			ctr[1]++;
			if (line[ctr[0] - 1] == '"' || line[ctr[0] - 1] == '\'')
				iterate_quotes(line, ctr, line[ctr[0] - 1], 1);
			if (line[ctr[0]] == ' ' || !line[ctr[0]])
				return (ctr[1]);
		}
	}
	return (ctr[1]);
}

int	count(char **array, int *ctr, int strings)
{
	int	start;

	start = 0;
	if (array[ctr[0]][ctr[1]] == '>' || array[ctr[0]][ctr[1]] == '<'
		|| array[ctr[0]][ctr[1]] == '|')
	{
		start = ctr[1];
		if (ctr[1] > 0)
		strings++;
		while (array[ctr[0]][ctr[1]] && (array[ctr[0]][ctr[1]] == '>'
			|| array[ctr[0]][ctr[1]] == '<' || array[ctr[0]][ctr[1]] == '|'))
			ctr[1]++;
		if (array[ctr[0]][ctr[1]])
			strings++;
	}
	return (strings);
}

int	get_len_without_redirects(t_resrc *rs, char **ar, int *fd)
{
	int	c[2];

	c[0] = 0;
	c[1] = 0;
	while (ar[c[0]])
	{
		if (ar[c[0]][0] == '>' || ar[c[0]][0] == '<' || ar[c[0]][0] == '|')
		{
			if (!check_syntax(ar, c, ar[c[0]][0]))
				return (0);
			if (ar[c[0]][0] == '|')
				break ;
			if (fd[1] != 1)
				close(fd[1]);
			if (open_file(rs, ar[c[0]], ar[c[0] + 1], fd) == -1)
				return (0);
			if (ar[c[0] + 1])
				c[1] -= 2;
			else
				c[1]--;
		}
		c[0]++;
		c[1]++;
	}
	return (c[1]);
}