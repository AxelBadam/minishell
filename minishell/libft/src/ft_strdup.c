/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 17:15:54 by ekoljone          #+#    #+#             */
/*   Updated: 2023/04/25 13:45:36 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		ctr;
	char	*ptr;

	ctr = 0;
	ptr = (char *)malloc(sizeof(char) * ft_strlen(s1) + 1);
	if (!ptr)
		return (NULL);
	while (*s1)
		ptr[ctr++] = *s1++;
	ptr[ctr] = 0;
	return (ptr);
}
