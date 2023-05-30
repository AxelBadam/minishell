/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 11:55:31 by ekoljone          #+#    #+#             */
/*   Updated: 2023/04/25 13:52:48 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_putstr(char *s)
{
	int	count;

	count = 0;
	if (!s)
	{
		write(1, "(null)", 6);
		count += 6;
	}
	else
	{
		while (s[count] != 0)
		{
			write(1, &s[count], 1);
			count++;
		}
	}
	return (count);
}

int	ft_putnbr(int nb)
{
	int	count;

	count = 0;
	if (nb == -2147483648)
	{
		count += ft_putchar('-');
		count += ft_putchar('2');
		nb = 147483648;
	}
	if (nb < 0)
	{
		count += ft_putchar('-');
		nb = nb * -1;
	}
	if (nb >= 10)
	{
		count += ft_putnbr(nb / 10);
		count += ft_putnbr(nb % 10);
	}
	else
	{
		count += ft_putchar(nb + 48);
	}
	return (count);
}
