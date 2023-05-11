/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:20:59 by ekoljone          #+#    #+#             */
/*   Updated: 2023/05/11 14:34:47 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	iterate_quotes(char *line, int *ctr, char d, int count)
{
	if (ft_strchr(&line[ctr[0]], d))
	{
		while (line[ctr[0]] != d)
		{
			ctr[0]++;
			if (count)
				ctr[1]++;
		}
		ctr[0]++;
		if (count)
			ctr[1]++;
		return (1);
	}
	ft_putstr_fd("\033[0;31mERROR: Unclosed quotes\n\033[0m", 2);
	return (0);
}

int	len_ctr(char *line)
{
	int	ctr[2];

	ctr[0] = 0;
	ctr[1] = 0;
	while (line[ctr[0]])
	{
		while (line[ctr[0]] == ' ')
			ctr[0]++;
		while (line[ctr[0]] && line[ctr[0]] != ' ')
		{
			ctr[0]++;
			ctr[1]++;
			if (line[ctr[0] - 1] == '"')
				iterate_quotes(line, ctr, '"' , 1);
			if (line[ctr[0] - 1] == '\'')
				iterate_quotes(line, ctr, '\'', 1);
			if (line[ctr[0]] == ' ' || !line[ctr[0]])
				return (ctr[1]);
		}
	}
	return (ctr[1]);
}


int	count_words(char *line)
{
	int	ctr[2];

	ctr[0] = 0;
	ctr[1] = 0;
	while (line[ctr[0]])
	{
		while (line[ctr[0]] == ' ')
			ctr[0]++;
		while (line[ctr[0]] && line[ctr[0]] != ' ')
		{
			ctr[0]++;
			if (line[ctr[0] - 1] == '"')
				if (!iterate_quotes(line, ctr, '"' , 0))
					return (0);
			if (line[ctr[0] - 1] == '\'')
				if (!iterate_quotes(line, ctr, '\'', 0))
					return (0);
			if (line[ctr[0]] == ' ' || !line[ctr[0]])
				ctr[1]++;
		}
	}
	return (ctr[1]);
}

char	**make_array(char *line)
{
	int		words;
	int		tmp[2];
	int		ctr;
	char	**array;

	ctr = 0;
	tmp[0] = 0;
	tmp[1] = 0;
	words = count_words(line);
	if (!words)
		return (NULL);
	array = (char **)malloc(sizeof(char *) * (words + 1));
	if (!array)
		return (NULL);
	while (words > 0)
	{
		tmp[1] = len_ctr(&line[tmp[0]]);
		tmp[0] += tmp[1];
		array[ctr] = (char *)malloc(sizeof(char) * (tmp[1] + 1));
		if (!array[ctr])
			return (NULL);
		while (line[tmp[0]] == ' ')
			tmp[0]++;
		ctr++;
		words--;
	}
	return (array);
}

void	fill_array(char *line, char **array)
{
	int	index[2];
	int	words;
	int	len;
	int	tmp[2];

	tmp[0] = 0;
	tmp[1] = 0;
	index[0] = 0;
	index[1] = 0;
	len = 0;
	words = count_words(line);
	while (words > index[0])
	{
		len = len_ctr(&line[tmp[0]]);
		tmp[1] = tmp[0];
		tmp[0] += len;
		while (--len > -1)
			array[index[0]][index[1]++] = line[tmp[1]++];
		array[index[0]][index[1]] = 0;
		index[1] = 0;
		while (line[tmp[0]] == ' ')
			tmp[0]++;
		index[0]++;
	}
	array[index[0]] = 0;
}

char	*get_env(char *d_string, char **env, int l)
{
	int		ctr[3];
	int		len;
	char	*str;

	ctr[0] = 0;
	ctr[1] = 0;
	ctr[2] = 0;
	len = 0;
	str = NULL;
	while (env[ctr[0]] && ft_strncmp(&d_string[1], env[ctr[0]], l - 1) != 0)
		ctr[0]++;
	if (env[ctr[0]])
	{
		while (env[ctr[0]][ctr[1]] != '=')
			ctr[1]++;
		ctr[1]++;
		len = ft_strlen(&env[ctr[0]][ctr[1]]);
		str = (char *)malloc(sizeof(char) * (len + 1));
		if (!str)
			return (NULL);
		while (env[ctr[0]][ctr[1]])
			str[ctr[2]++] = env[ctr[0]][ctr[1]++];
		str[ctr[2]] = 0;
	}
	return (str);
}

void	add_expansion(char **array, char *dst, char *src, int rm_len)
{
	int		ctr[4];
	int		s_len;
	char	*new_str;
	int		src_len;

	ctr[0] = 0;
	ctr[1] = 0;
	ctr[2] = 0;
	ctr[3] = 0;
	if (!src)
		src_len = 0;
	else
		src_len = ft_strlen(src);
	s_len = (ft_strlen(dst) - rm_len + src_len);
	while (array[ctr[0]] && array[ctr[0]] != dst)
		ctr[0]++;
	new_str = (char *)malloc(sizeof(char) * (s_len + 1));
	while (dst[ctr[1]])
	{
		if (!ctr[3])
		{
			if (dst[ctr[1]] == '$' || (dst[ctr[1]] == '~' && (array[ctr[0]][ctr[1] + 1] == '/' || !array[ctr[0]][ctr[1]] + 1)))
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
		}
		if (dst[ctr[1]])
			new_str[ctr[2]++] = dst[ctr[1]++];
	}
	new_str[ctr[2]] = 0;
	free (array[ctr[0]]);
	array[ctr[0]] = new_str;
}

void	expand(char **array, char **env)
{
	int		ctr[2];
	int		len;
	char	*ptr;
	char	*e;

	len = 0;
	ctr[0] = 0;
	ctr[1] = 0;
	(void)env;
	while (array[ctr[0]])
	{
		while (array[ctr[0]][ctr[1]])
		{
			ctr[1]++;
			if (array[ctr[0]][ctr[1] - 1] == '$')
			{
				len++;
				while (array[ctr[0]][ctr[1]] && array[ctr[0]][ctr[1]] != ' ' && array[ctr[0]][ctr[1]] != '$'
					&& array[ctr[0]][ctr[1]] != '\'' && array[ctr[0]][ctr[1]] != '"')
				{
					ctr[1]++;
					len++;
				}
				ptr = ft_substr(array[ctr[0]], ctr[1] - len, len);
				//e = get_env(ptr, env, len[0]);
				e = getenv(ptr + 1);
				add_expansion(array, array[ctr[0]], e, len);
				ctr[1] = 0;
				if (ptr)
					free(ptr);
			}
			if (array[ctr[0]][ctr[1] - 1] == '~' && (array[ctr[0]][ctr[1]] == '/' || !array[ctr[0]][ctr[1]]))
				add_expansion(array, array[ctr[0]], getenv("HOME"), 1);
			if (array[ctr[0]][ctr[1] - 1] == '\'')
				iterate_quotes(array[ctr[0]], &ctr[1], '\'', 0);
			if (array[ctr[0]][ctr[1] - 1] == '"')
				iterate_quotes(array[ctr[0]], &ctr[1], '"', 0);	
			len = 0;
		}
		ctr[1] = 0;
		ctr[0]++;
	}
}

char	**split_command(char *line, char **env)
{
	char	**array;

	array = make_array(line);
	if (!array)
		return (NULL);
	fill_array(line, array);
	expand(array, env);
	/*while (*array)
		printf("%s\n", *array++);*/
	//printf("%i\n", len_ctr(line));
	return (array);
}

void	parse_command(char *line, char **env)
{
	char	**array;

	array = split_command(line, env);
	if (array)
	{
		while (*array)
			printf("%s\n", *array++);
	}
}

void	minishell(t_resrc *resrc, char **env)
{
	//t_command *head;

	//head = NULL;
	resrc->line = readline("minishell: ");
	while (resrc->line)
	{
		add_history(resrc->line);
		parse_command(resrc->line, env);
		free(resrc->line);
		resrc->line = readline("minishell: ");
	}
}

void	*init_resources(void)
{
	t_resrc	*resrc;

	resrc = (t_resrc *)malloc(sizeof(t_resrc));
	if (!resrc)
		return NULL;
	resrc->buf = NULL;
	resrc->line = NULL;
	resrc->s_line = NULL;
	resrc->history = NULL;
	return (resrc);
}

t_command	*ft_lst_last(t_command *head)
{
	while (head->next != NULL)
		head = head->next;
	return (head);
}

void	ft_lstadd_back(t_command **head, t_command *new)
{
	t_command	*tmp;

	if (*head == NULL)
		*head = new;
	tmp = ft_lst_last(*head);
	tmp->next = new;
	new->next = NULL;
}

char	**create_env(char **env)
{
	int		ctr[2];
	char	**envp;
	//int		lvl;

	ctr[0] = 0;
	ctr[1] = 0;
	while (env[ctr[0]])
		ctr[0]++;
	envp = (char **)malloc(sizeof(char *) * (ctr[0] + 1));
	if (!envp)
		return (NULL);
	ctr[0] = 0;
	while (env[ctr[0]])
	{
		/*if (ft_strncmp("SHLVL", env[ctr[0]], 5) == 0)
		{
			while (env[ctr[0]][ctr[1]] != '=')
				ctr[1]++;
			ctr[1]++;
			lvl = ft_atoi(&env[ctr[0]][ctr[1]]);
			lvl++;
		}*/
		envp[ctr[0]] = ft_strdup(env[ctr[0]]);
		ctr[0]++;
	}
	envp[ctr[0]] = 0;
	return (envp);
}

int	main(int argc, char **argv, char **env)
{
	t_resrc	*resrc;

	(void)argc;
	(void)argv;
	/*while (*env)
		printf("%s\n", *env++);*/
	create_env(env);
	resrc = init_resources();
	minishell(resrc, env);
	return (0);
}

//hello man