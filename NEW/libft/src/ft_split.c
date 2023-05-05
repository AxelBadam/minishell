/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 12:52:02 by ekoljone          #+#    #+#             */
/*   Updated: 2023/04/25 13:45:27 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	strlenctr(const char *s, char c, int ctr)
{
	int	a;

	a = 0;
	while (s[ctr] == c)
		ctr++;
	while (s[ctr] != c && s[ctr] != 0)
	{
		ctr++;
		a++;
	}
	return (a);
}

static int	stringcounter(const char *s, char c)
{
	int	ctr;
	int	ctr2;

	ctr = 0;
	ctr2 = 0;
	if (s[ctr2] == 0)
		return (0);
	while (s[ctr2] != 0)
	{
		while (s[ctr2] == c)
			ctr2++;
		while (s[ctr2] != c && s[ctr2] != 0)
		{
			ctr2++;
			if ((s[ctr2] == c) || (s[ctr2] == 0))
				ctr++;
		}
	}
	return (ctr);
}

static char	**memoryallocation(char const *s, char c)
{
	char	**ptr;
	int		a;
	int		ctr;
	int		strings;

	strings = stringcounter(s, c);
	a = 0;
	ctr = 0;
	ptr = (char **)malloc(sizeof(char *) * (strings + 1));
	if (!ptr)
		return (NULL);
	while (a < strings)
	{
		while (s[ctr] == c)
			ctr++;
		ptr[a] = (char *)malloc(sizeof(char) * (strlenctr(s, c, ctr) + 1));
		if (!ptr)
			return (NULL);
		ctr += 1 + strlenctr(s, c, ctr);
		a++;
	}
	return (ptr);
}

static char	**addchars(const char *s, char **ptr, char c)
{
	int	ctr;
	int	ctr2;
	int	ctr3;
	int	strings;

	strings = stringcounter(s, c);
	ctr = 0;
	ctr2 = 0;
	ctr3 = 0;
	while (ctr2 < strings && s[ctr] != '\0')
	{
		while (s[ctr] == c)
			ctr++;
		while (s[ctr] != c && s[ctr] != 0)
		{
			ptr[ctr2][ctr3] = s[ctr];
			ctr3++;
			ctr++;
		}
		ptr[ctr2][ctr3] = '\0';
		ctr3 = 0;
		ctr2++;
	}
	ptr[strings] = 0;
	return (ptr);
}

char	**ft_split(char const *s, char c)
{
	char	**ptr;

	if (!s)
		return (NULL);
	ptr = memoryallocation(s, c);
	if (!ptr)
		return (NULL);
	ptr = addchars(s, ptr, c);
	return (ptr);
}
