/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 17:54:28 by ekoljone          #+#    #+#             */
/*   Updated: 2023/07/11 12:47:02 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

char	*create_expanded_string(char *dst, char *src, int rm_len)
{
	int		src_len;
	char	*new_str;
	int		s_len;

	if (!src)
		src_len = 0;
	else
		src_len = ft_strlen(src);
	s_len = (ft_strlen(dst) - rm_len + src_len);
	new_str = (char *)malloc(sizeof(char) * (s_len + 1));
	if (!new_str)
		return (NULL);
	return (new_str);
}

int	add_exp(char **ar, char *dst, char *src, int rm_len)
{
	int		c[4];
	char	*new_str;

	c[0] = 0;
	c[1] = 0;
	c[2] = 0;
	c[3] = 0;
	while (ar[c[0]] && ar[c[0]] != dst)
		c[0]++;
	new_str = create_expanded_string(dst, src, rm_len);
	if (!new_str)
		return (-1);
	while (dst[c[1]])
	{
		if (!c[3])
			if (dst[c[1]] == '$' || (c[1] == 0 && dst[c[1]] == '~'
					&& (ar[c[0]][c[1] + 1] == '/' || !ar[c[0]][c[1] + 1])))
				expand_string(src, new_str, c, rm_len);
		if (dst[c[1]])
			new_str[c[2]++] = dst[c[1]++];
	}
	new_str[c[2]] = 0;
	free(ar[c[0]]);
	ar[c[0]] = new_str;
	return (free(src), 0);
}

int	expand_dollar_sign(char **ar, int *ctr, char **env, int len)
{
	char	*ptr;

	ptr = NULL;
	if (ar[ctr[0]][ctr[1] - 1] == '$' && (is_alpha_digit(ar[ctr[0]][ctr[1]])
		|| ar[ctr[0]][ctr[1]] == '?'))
	{
		len++;
		if (ar[ctr[0]][ctr[1]] == '?')
		{
			if (add_exp(ar, ar[ctr[0]], ft_itoa(g_exit_status), len + 1) == -1)
				return (-1);
		}
		else
		{
			ptr = get_string(ar, ctr, &len);
			if (!ptr)
				return (-1);
			if (add_exp(ar, ar[ctr[0]], get_env(ptr + 1, env), len) == -1)
				return (-1);
		}
		ctr[1] = 0;
		if (ptr)
			free(ptr);
	}
	return (0);
}

void	expand_double_quotes(char **ar, int *ctr)
{
	int	d_quote[2];

	d_quote[0] = -1;
	d_quote[1] = -1;
	get_double_quotes(ar, ctr, d_quote);
	if ((ctr[1] - 1) < d_quote[1] && (ctr[1] - 1) > d_quote[0])
		return ;
	if (ar[ctr[0]][ctr[1] - 1] == '\'')
		if (ft_strchr(&ar[ctr[0]][ctr[1]], '\''))
			iterate_quotes(ar[ctr[0]], &ctr[1], '\'', 0);
}

int	expand(char **ar, char **env)
{
	int		ctr[2];

	ctr[0] = 0;
	ctr[1] = -1;
	while (ar[ctr[0]])
	{
		while (ar[ctr[0]][++ctr[1]])
		{
			if (ar[ctr[0]][ctr[1] - 1] == '\'')
				expand_double_quotes(ar, ctr);
			if (ctr[1] == 0 && ar[ctr[0]][ctr[1]] == '~'
				&& (ar[ctr[0]][ctr[1] + 1] == '/'
					|| !ar[ctr[0]][ctr[1] + 1]))
				if (add_exp(ar, ar[ctr[0]], get_env("HOME", env), 1) == -1)
					return (-1);
			if (expand_dollar_sign(ar, ctr, env, 0) == -1)
				return (-1);
		}
		ctr[1] = -1;
		ctr[0]++;
	}
	return (0);
}
