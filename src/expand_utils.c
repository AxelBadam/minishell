/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 17:31:19 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/20 15:37:38 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

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
