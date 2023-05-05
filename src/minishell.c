/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:20:59 by ekoljone          #+#    #+#             */
/*   Updated: 2023/05/05 17:29:49 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_ctr(char *line)
{
	int	ctr;
	int	len;

	len = 0;
	ctr = 0;
	while (line[ctr] == ' ')
		ctr++;
	if (line[ctr] == '"')
	{
		ctr++;
		while (line[ctr] && line[ctr] != '"')
		{
			len++;
			ctr++;
		}
		if (line[ctr] == '"')
			len += 2;
		else
			len += 1;
	}
	else
	{
		while (line[ctr] && line[ctr] != ' ')
		{
			ctr++;
			len++;
			if (line[ctr] == '"')
				break ;
		}
	}
	printf("%i\n", len);
	return (len);
}

int	count_words(char *line)
{
	int	ctr;
	int	word;
	ctr = 0;
	word = 0;
	while (line[ctr])
	{
		while (line[ctr] == ' ')
			ctr++;
		while (line[ctr] && line[ctr] != ' ')
		{
			ctr++;
			if (line[ctr - 1] == '"')
			{
				if (ctr > 1)
				{
					if (line[ctr - 2] != ' ' && line[ctr - 2] != '"')
						word++;
				}
				while (line[ctr] && line[ctr] != '"')
				{
					ctr++;
					if (!line[ctr] || line[ctr] == '"')
						word++;
				}
				if (!line[++ctr])
					break ;
			}
			if (line[ctr] == ' ' || !line[ctr])
			{
				if (line[ctr - 1] != '"')
					word++;
			}
		}
	}
	return (word);
}

char	**make_array(char *line)
{
	int		words;
	int		tmp;
	int		ctr;
	int		len;
	char	**array;

	ctr = 0;
	tmp = 0;
	len = 0;
	words = count_words(line);
	array = (char **)malloc(sizeof(char *) * (words + 1));
	if (!array)
		return (NULL);
	while (words > 0)
	{
		len = len_ctr(&line[tmp]);
		tmp += len;
		array[ctr] = (char *)malloc(sizeof(char) * (len + 1));
		if (!array[ctr])
			return (NULL);
		ctr++;
		words--;
	}
	return (array);
}

void	fill_array(char *line, char **array)
{
	int	row;
	int	clm;
	int	words;
	int	len;
	int	tmp;
	int	tmp2;

	tmp = 0;
	len = 0;
	clm = 0;
	words = count_words(line);
	row = 0;
	while (words > row)
	{
		len = len_ctr(&line[tmp]);
		tmp2 = tmp;
		tmp += len;
		while (line[tmp2] == ' ')
			tmp2++;
		while (len > 0)
		{
			array[row][clm++] = line[tmp2++];
			len--;
		}
		array[row][clm] = 0;
		clm = 0;
		row++;
	}
	array[row] = 0;
}

char	**split_command(char *line)
{
	char	**array;

	array = make_array(line);
	fill_array(line, array);
	while (*array)
		printf("%s\n", *array++);
	return (NULL);
}

/*void	separate_operators(t_command *head, char **s_line)
{
	int	ctr;

	ctr = 0;
	while (s_line[ctr])
	{
		
	}
}*/

void	parse_command(char *line)
{
	split_command(line);
}

void	minishell(t_resrc *resrc)
{
	t_command *head;

	head = NULL;
	resrc->line = readline("minishell: ");
	while (resrc->line)
	{
		add_history(resrc->line);
		parse_command(resrc->line);
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

int	main()
{
	t_resrc	*resrc;

	resrc = init_resources();
	minishell(resrc);
	return (0);
}

//hello man