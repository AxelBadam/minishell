/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 15:00:10 by ekoljone          #+#    #+#             */
/*   Updated: 2023/04/25 13:46:10 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	ctr;

	ctr = 0;
	if (n == 0)
		return (0);
	while (ctr < n - 1 && s1[ctr] == s2[ctr]
		&& s1[ctr] != '\0' && s2[ctr] != '\0')
		ctr++;
	return ((unsigned char)s1[ctr] - (unsigned char)s2[ctr]);
}
