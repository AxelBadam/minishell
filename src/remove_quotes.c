/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:22:44 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/20 17:29:33 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*make_new_str(char *old_str, int len)
{
	char	*new_str;
	int		ctr[2];
	char	d;

	ctr[0] = 0;
	ctr[1] = 0;
	new_str = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (NULL);
	while (old_str[ctr[0]])
	{
		if (old_str[ctr[0]] == '"' || old_str[ctr[0]] == '\'')
		{
			d = old_str[ctr[0]++];
			while (old_str[ctr[0]] && old_str[ctr[0]] != d)
				new_str[ctr[1]++] = old_str[ctr[0]++];
			ctr[0]++;
		}
		if (old_str[ctr[0]] && (old_str[ctr[0]] != '"'
				&& old_str[ctr[0]] != '\''))
			new_str[ctr[1]++] = old_str[ctr[0]++];
	}
	new_str[ctr[1]] = 0;
	free(old_str);
	return (new_str);
}

int	str_len_without_quotes(char *str)
{
	int		ctr[2];
	char	d;

	ctr[0] = 0;
	ctr[1] = 0;
	while (str[ctr[0]])
	{
		if (str[ctr[0]] == '\'' || str[ctr[0]] == '"')
		{
			ctr[1] += 2;
			d = str[ctr[0]++];
			while (str[ctr[0]] && str[ctr[0]] != d)
				ctr[0]++;
			if (!str[ctr[0]])
				break ;
			ctr[0]++;
		}
		else
			ctr[0]++;
	}
	return (ctr[0] - ctr[1]);
}

void	remove_quotes(t_resrc *rs, char **array)
{
	int		ctr[2];
	int		len;

	ctr[0] = 0;
	ctr[1] = 0;
	len = 0;
	while (array[ctr[0]])
	{
		len = str_len_without_quotes(array[ctr[0]]);
		if (len == ft_strlen(array[ctr[0]]))
			ctr[0]++;
		else
		{
			array[ctr[0]] = make_new_str(array[ctr[0]], len);
			if (!array[ctr[0]++])
				error_exit("minishell: fatal malloc error\n", rs);
		}
	}
}
