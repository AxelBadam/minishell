/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:14:20 by ekoljone          #+#    #+#             */
/*   Updated: 2023/04/25 13:46:24 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	checkset(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ptr;
	int		i;
	int		first_part;
	int		last_part;

	first_part = 0;
	if (!s1)
		return (NULL);
	last_part = ft_strlen(s1);
	while (s1[first_part] && checkset(s1[first_part], set))
		first_part++;
	while (last_part > first_part && checkset(s1[last_part - 1], set))
		last_part--;
	ptr = (char *)malloc(sizeof(char) * (last_part - first_part + 1));
	if (!ptr)
		return (NULL);
	i = 0;
	while (first_part < last_part)
		ptr[i++] = s1[first_part++];
	ptr[i] = 0;
	return (ptr);
}
