/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 14:15:44 by ekoljone          #+#    #+#             */
/*   Updated: 2023/04/25 13:45:50 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	a;
	size_t	b;
	char	*ptr;

	ptr = (char *)src;
	a = 0;
	while (a < dstsize && *dst)
	{
		dst++;
		a++;
	}
	if (a == dstsize)
		return (a + ft_strlen(src));
	b = 0;
	while (ptr[b])
	{
		if (b < dstsize - a - 1)
			*dst++ = ptr[b];
		b++;
	}
	*dst = 0;
	return (b + a);
}
