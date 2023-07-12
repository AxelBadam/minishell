/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:27:29 by ekoljone          #+#    #+#             */
/*   Updated: 2023/07/12 14:00:10 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

int	check_pipe_syntax(char **array, int *ctr)
{
	int	error;

	error = 0;
	if (ft_strlen(array[ctr[0]]) > 1 || ctr[0] == 0)
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
	if (d == '|')
	{
		if (!check_pipe_syntax(ar, ctr))
			return (0);
		return (1);
	}
	if (ar[ctr[0]][1] && (((d == '<' && ar[ctr[0]][1] != d) || ar[ctr[0]][2])
		|| (ar[ctr[0]][1] && d == '>'
			&& ar[ctr[0]][1] != '|' && ar[ctr[0]][1] != '>')))
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
	if (!ft_strncmp(tmp, "pwd", ctr) && ctr == 3)
		re = 1;
	else if (!ft_strncmp(tmp, "env", ctr) && ctr == 3)
		re = 1;
	else if (!ft_strncmp(tmp, "cd", ctr) && ctr == 2)
		re = 1;
	else if (!ft_strncmp(tmp, "export", ctr) && ctr == 6)
		re = 1;
	else if (!ft_strncmp(tmp, "unset", ctr) && ctr == 5)
		re = 1;
	else if (!ft_strncmp(tmp, "echo", ctr) && ctr == 4)
		re = 1;
	else if (!ft_strncmp(tmp, "exit", ctr) && ctr == 4)
		re = 1;
	free(tmp);
	return (re);
}

int	check_for_parent_builtin(t_resrc *resrc, t_list *list, int len)
{	
	char	*tmp;

	tmp = ft_strdup(*list->command.full_cmd);
	tmp = str_to_lower(tmp);
	if (!ft_strncmp(tmp, "exit", len) && len == 4 \
	&& linked_list_count(&resrc->list) == 1)
		execute_builtin_exit(list->command.full_cmd, 0);
	if (!ft_strncmp(tmp, "cd", len) && len == 2)
		return (execute_builtin_cd(resrc, list->command), free(tmp), 1);
	else if (!ft_strncmp(tmp, "unset", len) && len == 5)
		return (execute_builtin_unset(list, resrc), free(tmp), 1);
	else if (!ft_strncmp(tmp, "export", 6) && len == 6 \
	&& get_array_size(list->command.full_cmd) > 1)
		return (execute_builtin_export(list, resrc), free(tmp), 1);
	return (free(tmp), 0);
}
