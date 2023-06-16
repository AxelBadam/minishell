/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 17:54:28 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/16 12:42:07 by atuliara         ###   ########.fr       */
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

void	expand_string(char *src, char *new_str, int *ctr, int rm_len)
{
	if (src)
		while (src[ctr[3]])
			new_str[ctr[2]++] = src[ctr[3]++];
	while (rm_len > 0)
	{
		ctr[1]++;
		rm_len--;
	}
}

void	add_expansion(char **ar, char *dst, char *src, int rm_len)
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
		return ;
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
	free(src);
	ar[c[0]] = new_str;
}

void	expand_dollar_sign(char **ar, int *ctr, char **env)
{
	char	*ptr;
	int		len;

	len = 0;
	ptr = NULL;
	if (ar[ctr[0]][ctr[1] - 1] == '$')
	{
		len++;
		if (ar[ctr[0]][ctr[1]] == '?')
			add_expansion(ar, ar[ctr[0]], ft_itoa(g_exit_status), len + 1);
		else
		{
			while (ft_isalpha(ar[ctr[0]][ctr[1]])
				|| ar[ctr[0]][ctr[1]] == '_')
			{
				ctr[1]++;
				len++;
			}
			ptr = ft_substr(ar[ctr[0]], ctr[1] - len, len);
			add_expansion(ar, ar[ctr[0]], get_env(ptr + 1, env), len);
		}
		ctr[1] = 0;
		if (ptr)
			free(ptr);
	}
}

void	expand(char **array, char **env)
{
	int		ctr[2];
	char	*ptr;

	ctr[0] = 0;
	ctr[1] = -1;
	ptr = NULL;
	while (array[ctr[0]])
	{
		while (array[ctr[0]][++ctr[1]])
		{
			if (ctr[1] == 0 && array[ctr[0]][ctr[1]] == '~'
				&& (array[ctr[0]][ctr[1] + 1] == '/'
					|| !array[ctr[0]][ctr[1] + 1]))
				add_expansion(array, array[ctr[0]], get_env("HOME", env), 1);
			expand_dollar_sign(array, ctr, env);
			if (array[ctr[0]][ctr[1] - 1] == '\'')
				if (ft_strchr(&array[ctr[0]][ctr[1]], '\''))
					iterate_quotes(array[ctr[0]], &ctr[1], '\'', 0);
		}
		ctr[1] = -1;
		ctr[0]++;
	}
}
