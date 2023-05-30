/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 10:47:49 by ekoljone          #+#    #+#             */
/*   Updated: 2023/04/25 13:44:53 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char	*ptr;
	char	*ptr1;

	ptr = (char *)dst;
	ptr1 = (char *)src;
	if (dst == 0 && src == 0)
		return (dst);
	while (n != 0)
	{
		*ptr++ = *ptr1++;
		n--;
	}
	return (dst);
}
