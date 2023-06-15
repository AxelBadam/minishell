/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 12:42:01 by ekoljone          #+#    #+#             */
/*   Updated: 2023/03/14 12:43:29 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strlen_gnl(const char *s)
{
	int	length;

	length = 0;
	if (!s)
		return (0);
	while (s[length] != '\0')
		length++;
	return (length);
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*ptr;
	int		ctr;
	int		ctr2;

	if (!s1)
	{
		s1 = (char *)malloc(sizeof(char) * 1);
		s1[0] = '\0';
	}
	if (!s1 || !s2)
		return (NULL);
	ptr = (char *)malloc(sizeof(char)
			* (ft_strlen_gnl(s1) + ft_strlen_gnl(s2) + 1));
	if (!ptr)
		return (NULL);
	ctr = 0;
	ctr2 = -1;
	while (s1[++ctr2] != 0)
		ptr[ctr2] = s1[ctr2];
	while (s2[ctr] != 0)
		ptr[ctr2++] = s2[ctr++];
	ptr[ctr2] = '\0';
	free(s1);
	return (ptr);
}

char	*ft_strchr_gnl(const char *s, int c)
{
	int	a;

	a = 0;
	if (!s)
		return (NULL);
	while (s[a] != 0)
		a++;
	while (a > -1)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
		a--;
	}
	return (NULL);
}
