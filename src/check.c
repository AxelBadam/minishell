/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:27:29 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/13 16:32:24 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

int	check_pipe_syntax(char **array, int *ctr)
{
	int	error;

	error = 0;
	if (array[ctr[0]][1])
		error = 258;
	if (array[ctr[0] + 1])
		if (array[ctr[0] + 1][0] == '>' || array[ctr[0] + 1][0] == '<'
			|| array[ctr[0] + 1][0] == '|')
			error = 258;
	if (error)
		if (!print_error("syntax error near unexpected token `|'\n", 258, NULL))
			return (0);
	return (1);
}

int	print_syntax_error(char *s, int exit)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("'\n", 2);
	g_exit_status = exit;
	return (0);
}

int	check_syntax(char **ar, int *ctr, char d)
{
	int	error;

	error = 0;
	if (d == '|')
		if (!check_pipe_syntax(ar, ctr))
			return (0);
	if (((ar[ctr[0]][1] && d == '<' && ar[ctr[0]][1] != d) || ar[ctr[0]][2])
		|| (ar[ctr[0]][1] && d == '>'
			&& ar[ctr[0]][1] != '|' && ar[ctr[0]][1] != '>'))
	{
		if (ar[ctr[0]][2])
			print_syntax_error(&ar[ctr[0]][2], 258);
		else
			print_syntax_error(&ar[ctr[0]][1], 258);
		return (0);
	}
	if (ar[ctr[0] + 1])
	{
		if (ar[ctr[0] + 1][0] == '>' || ar[ctr[0] + 1][0] == '<'
			|| ar[ctr[0] + 1][0] == '|')
			if (!print_syntax_error(ar[ctr[0] + 1], 258))
				return (0);
	}
	return (1);
}

int	is_builtin(char *str)
{
	char	*tmp;
	int		ctr;
	int		re;

	tmp = ft_strdup(str);
	ctr = -1;
	re = 0;
	while (tmp[++ctr])
		tmp[ctr] = ft_tolower(tmp[ctr]);
	if (!ft_strncmp(tmp, "pwd", ctr))
		re = 1;
	else if (!ft_strncmp(tmp, "env", ctr))
		re = 1;
	else if (!ft_strncmp(tmp, "cd", ctr))
		re = 1;
	else if (!ft_strncmp(tmp, "export", ctr))
		re = 1;
	else if (!ft_strncmp(tmp, "unset", ctr))
		re = 1;
	else if (!ft_strncmp(tmp, "echo", ctr))
		re = 1;
	else if (!ft_strncmp(tmp, "exit", ctr))
		re = 1;
	free(tmp);
    return (re);
}
