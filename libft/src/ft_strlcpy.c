/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 11:02:41 by ekoljone          #+#    #+#             */
/*   Updated: 2023/04/25 13:45:56 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	int	ctr;
	int	ctr2;

	ctr = 0;
	ctr2 = 0;
	while (dst[ctr2] != 0)
		ctr2++;
	while (src[ctr] != 0)
		ctr++;
	if (dstsize > 0)
	{
		while (dstsize > 1 && *src != '\0')
		{
			*dst = *src;
			dst++;
			src++;
			dstsize--;
		}
		*dst = 0;
	}
	return (ctr);
}
