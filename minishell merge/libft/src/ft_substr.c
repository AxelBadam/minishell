/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:31:14 by ekoljone          #+#    #+#             */
/*   Updated: 2023/04/25 13:46:30 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	int		ctr;
	int		a;

	ctr = 0;
	if ((int)start > ft_strlen(s))
		a = 1;
	if ((int)len > ft_strlen(s))
		a = ft_strlen(s);
	else
		a = len;
	if (!s)
		return (NULL);
	ptr = (char *)malloc(sizeof(char) * (a + 1));
	if (!ptr)
		return (NULL);
	if ((int)start > ft_strlen(s))
	{
		ptr[ctr] = '\0';
		return (ptr);
	}
	while (s[start] != 0 && len-- > 0)
		ptr[ctr++] = s[start++];
	ptr[ctr] = 0;
	return (ptr);
}
