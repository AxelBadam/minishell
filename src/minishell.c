/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:20:59 by ekoljone          #+#    #+#             */
/*   Updated: 2023/05/03 17:18:34 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell(t_resrc *resrc)
{
	t_command *head;

	head = NULL;
	resrc->line = readline("minishell: ");
	while (resrc->line)
	{
		add_history(resrc->line);
		head = parse_list(resrc->line);
		print_list(&head);
		break ;
		//free(resrc->line);
		//resrc->line = readline("minishell: ");
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

char	**check_for_operator(char **s_line)
{
	int	counter;

	counter = 0;
	while (s_line[counter])
	{
		if (ft_strncmp(s_line[counter], ">", SIZE_MAX) == 0)
			return(&s_line[counter]);
		if (ft_strncmp(s_line[counter], ">>", SIZE_MAX) == 0)
			return(&s_line[counter]);
		if (ft_strncmp(s_line[counter], "<", SIZE_MAX) == 0)
			return(&s_line[counter]);
		if (ft_strncmp(s_line[counter], "<<", SIZE_MAX) == 0)
			return(&s_line[counter]);
		counter++;
	}
	return (NULL);
}

t_command	*create_node(char **s_line, char **operator)
{
	t_command *new_node;

	new_node = (t_command *)malloc(sizeof(t_command));
	if (!new_node)
		return (NULL);
	new_node->command = s_line[0];
	if (operator)
	{
		new_node->operator = *operator;
		if (operator[1])
			new_node->filename = operator[1];
		else
			new_node->filename = NULL;
	}
	else
	{
		new_node->operator = NULL;
		new_node->filename = NULL;
	}
	new_node->next = NULL;
	return (new_node);
}

t_command	*parse_list(char *line)
{
	t_command	*head;
	char		**s_line;
	char		**operator;

	head = NULL;
	s_line = ft_split(line, ' ');
	operator = check_for_operator(s_line);
	if (operator)
	{
		while (operator)
		{
			ft_lstadd_back(&head, create_node(s_line, operator));
			operator = check_for_operator(operator + 1);
		}
	}
	else
		ft_lstadd_back(&head, create_node(s_line, operator));
	return (head);
}

void	print_list(t_command **head)
{
	t_command *tmp;
	int counter = 0;

	tmp = *head;
	while (tmp)
	{
		printf("NODE %i\nCMD: %s\nOPERATOR: %s\nFILENAME: %s\n\n", counter++, tmp->command, tmp->operator, tmp->filename);
		tmp = tmp->next;
	}
}

int	main()
{
	t_resrc	*resrc;

	resrc = init_resources();
	minishell(resrc);
	return (0);
}

//hello man