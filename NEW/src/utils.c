/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:45:06 by atuliara          #+#    #+#             */
/*   Updated: 2023/05/17 15:40:58 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "linked_list.h"

/*
#include "minishell.h"

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
}*/

void error_handling(char *str)
{
	printf("%s", str);
	exit (1);
}

char    **create_env(char **env)
{
    int        ctr[2];
    char    **envp;
    //int        lvl;

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
        envp[ctr[0]++] = ft_strdup(env[ctr[0]]);
    }
    envp[ctr[0]] = 0;
    return (envp);
}