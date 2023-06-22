/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 17:31:19 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/22 12:25:21 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_double_quotes(char **ar, int *ctr, int *d_quote)
{
	int	tmp;

	tmp = 0;
	while (ar[ctr[0]][tmp])
	{
		if (ar[ctr[0]][tmp] == '"')
		{
			if (d_quote[0] == -1)
				d_quote[0] = tmp;
			else
				d_quote[1] = tmp;
		}
		if (d_quote[1] > -1)
		{
			if ((ctr[1] - 1) > d_quote[1])
			{
				d_quote[0] = -1;
				d_quote[1] = -1;
			}
			else
				break ;
		}
		tmp++;
	}
}

int	ft_is_white_space(char d)
{
	if (d == 32 || (d >= 9 && d <= 13))
		return (1);
	return (0);
}

char	*get_string(char **ar, int *ctr, int *len)
{
	char	*ptr;

	ptr = NULL;
	while (ft_isalnum(ar[ctr[0]][ctr[1]])
		|| ar[ctr[0]][ctr[1]] == '_')
	{
		ctr[1]++;
		*len += 1;
	}
	ptr = ft_substr(ar[ctr[0]], ctr[1] - *len, *len);
	return (ptr);
}

void	expand_string(char *src, char *new_str, int *ctr, int rm_len)
{
	if (src)
		while (src[ctr[3]])
			new_str[ctr[2]++] = src[ctr[3]++];
	while (rm_len > 0)
	{
		ctr[1]++;
		rm_len--;
	}
}
