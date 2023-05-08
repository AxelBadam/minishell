/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:20:59 by ekoljone          #+#    #+#             */
/*   Updated: 2023/05/08 17:43:36 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	iterate_quotes(char *line, int *ctr, char d, int count)
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
	}
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
	int	check;

	check = 0;
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
				iterate_quotes(line, ctr, '"' , 0);
			if (line[ctr[0] - 1] == '\'')
				iterate_quotes(line, ctr, '\'', 0);
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

char	**split_command(char *line)
{
	char	**array;

	array = make_array(line);
	fill_array(line, array);
	while (*array)
		printf("%s\n", *array++);
	//printf("%i\n", len_ctr(line));
	return (NULL);
}

void	expand(char **array, char **env)
{
	int		ctr[2];
	int		len[2];
	char	*ptr;

	len[0] = 0;
	len[1] = 0;
	ctr[0] = 0;
	ctr[1] = 0;
	while (array[ctr[0]])
	{
		while (array[ctr[0]][ctr[1]])
		{
			if (array[ctr[0]][ctr[1]] == '$')
			{
				while (array[ctr[0]][ctr[1]] && array[ctr[0]][ctr[1]] != ' ')
				
			}
		}
	}
}

void	parse_command(char *line, char **env)
{
	char	*array;

	array = split_command(line);
	expand(array, env);
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

int	main(int argc, char **argv, char **env)
{
	t_resrc	*resrc;

	argc = 0;
	(void)argv;
	(void)env;
	/*while (*env)
		printf("%s\n", *env++);*/
	resrc = init_resources();
	minishell(resrc);
	return (0);
}

//hello man