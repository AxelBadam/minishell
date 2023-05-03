/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:31:12 by ekoljone          #+#    #+#             */
/*   Updated: 2023/04/25 13:45:41 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	a;

	a = 0;
	if (s != 0)
	{
		while (s[a] != 0)
		{
			f(a, &s[a]);
			a++;
		}
	}
}
