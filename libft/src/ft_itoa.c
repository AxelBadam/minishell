/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:13:28 by ekoljone          #+#    #+#             */
/*   Updated: 2023/04/25 13:44:39 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	heightcounter(int a)
{
	int	ctr;

	ctr = 0;
	if (a == 0)
		return (1);
	if (a == -2147483648)
		return (11);
	if (a < 0)
	{
		ctr += 1;
		a *= -1;
	}
	while (a != 0)
	{
		a = a / 10;
		ctr++;
	}
	return (ctr);
}

static int	intminconversion(int n, char *ptr, int ctr)
{
	if (n == -2147483648)
	{
		n += 1;
		n *= -1;
		ptr[ctr--] = '1' + (n % 10);
		ptr[0] = '-';
		n /= 10;
	}
	return (n);
}

char	*ft_itoa(int n)
{
	char	*ptr;
	int		ctr;

	ctr = heightcounter(n);
	ptr = (char *)malloc(sizeof(char) * ctr + 1);
	if (!ptr)
		return (NULL);
	if (n == 0)
		ptr[0] = '0';
	ptr[ctr--] = '\0';
	if (n == -2147483648)
		n = intminconversion(n, ptr, ctr--);
	if (n < 0)
	{
		ptr[0] = '-';
		n *= -1;
	}
	while (n > 0)
	{
		ptr[ctr--] = '0' + (n % 10);
		n = n / 10;
	}
	return (ptr);
}
