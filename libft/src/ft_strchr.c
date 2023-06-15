/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 14:30:29 by ekoljone          #+#    #+#             */
/*   Updated: 2023/04/25 13:45:31 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	a;

	a = ft_strlen(s);
	while (a > -1)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
		a--;
	}
	return (NULL);
}
