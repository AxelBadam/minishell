/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 14:52:25 by ekoljone          #+#    #+#             */
/*   Updated: 2023/04/25 13:46:19 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	ctr;

	ctr = ft_strlen(s);
	while (ctr > -1)
	{
		if (s[ctr] == (char)c)
			return ((char *)s + ctr);
		ctr--;
	}
	return (NULL);
}
