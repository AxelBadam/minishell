/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:25:15 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/06 12:42:54 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putunsnbr(unsigned int nb)
{
	int	count;

	count = 0;
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

int	convertohexa(char string, unsigned long long int a)
{
	int	count;

	count = 0;
	if (a >= 16)
	{
		count += convertohexa(string, a / 16);
		count += convertohexa(string, a % 16);
	}
	else
	{
		if (a < 10)
			count += ft_putchar(a + '0');
		else if (string == 'X')
			count += ft_putchar(a - 10 + 'A');
		else
			count += ft_putchar(a - 10 + 'a');
	}
	return (count);
}

int	checkconversion(const char *string, va_list ptr)
{
	int	count;

	count = 0;
	if (*string == '%')
		count += ft_putchar(*string);
	else if (*string == 'c')
		count += ft_putchar(va_arg(ptr, int));
	else if (*string == 's')
		count += ft_putstr(va_arg(ptr, char *));
	else if (*string == 'i' || *string == 'd')
		count += ft_putnbr(va_arg(ptr, int));
	else if (*string == 'u')
		count += ft_putunsnbr(va_arg(ptr, unsigned int));
	else if (*string == 'p')
	{
		count += ft_putstr("0x");
		count += convertohexa(*string, va_arg(ptr, unsigned long long));
	}
	else if (*string == 'x' || *string == 'X')
		count += convertohexa(*string, va_arg(ptr, unsigned int));
	return (count);
}

int	ft_printf(const char *string, ...)
{
	va_list	ptr;
	int		count;

	count = 0;
	va_start(ptr, string);
	while (*string)
	{
		if (*string == '%')
		{
			count += checkconversion(++string, ptr);
			if (*string == '\0')
				return (count);
			string++;
		}
		else
			count += ft_putchar(*string++);
	}
	va_end(ptr);
	return (count);
}
