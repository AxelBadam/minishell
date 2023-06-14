/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:15:36 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/14 15:01:12 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

char	*get_full_path(t_resrc *rs, char *cmd, char *path)
{
	char	*full_path;
	int		ctr[2];
	int		len;

	ctr[0] = -1;
	ctr[1] = 0;
	len = 0;
	while (path[++ctr[0]])
	{
		if (path[ctr[0]] == ':')
		{
			full_path = create_full_path(cmd, path, ctr[1], len);
			if (!full_path)
				error_exit("minishell: fatal malloc error\n", rs);
			if (access(full_path, F_OK) == 0)
				break ;
			free(full_path);
			full_path = NULL;
			ctr[1] = ctr[0] + 1;
			len = 0;
		}
		len++;
	}
	free(path);
	return (full_path);
}

t_list	*create_node(char **full_cmd, int *fd, t_resrc *rs)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->command.full_path = NULL;
	if (!is_builtin(*full_cmd))
		new_node->command.full_path = get_full_path(rs, *full_cmd,
				get_env("PATH", rs->envp));
	new_node->command.full_cmd = full_cmd;
	new_node->command.output_fd = fd[1];
	new_node->command.input_fd = fd[0];
	new_node->next = NULL;
	return (new_node);
}

void	create_full_cmd(char **full_cmd, char **array, int *ctr, int len)
{
	if (ctr[0] < len)
		full_cmd[ctr[0]++] = ft_strdup(array[ctr[1]++]);
	while (array[ctr[1]]
		&& (array[ctr[1]][0] == '>' || array[ctr[1]][0] == '<'))
	{
		if (array[ctr[1] + 1])
			ctr[1] += 2;
		else
			ctr[1]++;
	}
}

void	get_next_node(t_resrc *rs, char **array, int *ctr)
{
	if (!array[ctr[1] + 1])
	{
		free_string_array(rs->array);
		rs->array = get_new_command(rs, array);
		array = rs->array;
	}
	if (!array)
		return ;
	make_list(rs, &array[ctr[1] + 1]);
}

void	make_list(t_resrc *rs, char **array)
{
	t_variables	v;

	v.ctr[1] = 0;
	v.ctr[0] = 0;
	v.fd[0] = 0;
	v.fd[1] = 1;
	v.len = get_len_without_redirects(rs, array, v.fd);
	if (!v.len)
		return ;
	v.full_cmd = (char **)malloc(sizeof(char *) * (v.len + 1));
	if (!v.full_cmd)
		error_exit("minishell: fatal malloc error\n", rs);
	while (array[v.ctr[1]] && array[v.ctr[1]][0] != '|')
		create_full_cmd(v.full_cmd, array, v.ctr, v.len);
	v.full_cmd[v.ctr[0]] = 0;
	remove_quotes(rs, v.full_cmd);
	ft_lstadd_back(&rs->list, create_node(v.full_cmd, v.fd, rs));
	if (array[v.ctr[1]] && array[v.ctr[1]][0] == '|')
		get_next_node(rs, array, v.ctr);
}
