/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:15:36 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/16 13:52:20 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_full_path(t_resrc *rs, char *cmd, char *path)
{
	char	*full_path;
	int		ctr[2];
	int		len;

	ctr[0] = -1;
	ctr[1] = 0;
	len = 0;
	if (!path)
		return (NULL);
	while (path[++ctr[0]])
	{
		if (path[ctr[0]] == ':')
		{
			full_path = create_full_path(cmd, path, ctr[1], len);
			if (!full_path)
				error_exit("minishell: fatal malloc error\n", rs);
			full_path = check_access(full_path, ctr, &len);
			if (full_path)
				break ;
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
	new_node->command.pid = -2;
	new_node->command.fd[0] = -2;
	new_node->command.fd[1] = -2;
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
	if (array[ctr[1]] && array[ctr[1]][0] == '|')
	{
		if (!array[ctr[1] + 1])
			array = get_new_command(rs);
		else
			array = array_dup(&array[ctr[1] + 1]);
		if (!array)
			return ;
		make_list(rs, array);
		free_string_array(array);
	}
}

void	make_list(t_resrc *rs, char **array)
{
	int		ctr[2];
	int		fd[2];
	int		len;
	char	**full_cmd;

	ctr[1] = 0;
	ctr[0] = 0;
	fd[0] = 0;
	fd[1] = 1;
	len = get_len_without_redirects(rs, array, fd);
	if (!len)
		return ;
	full_cmd = (char **)malloc(sizeof(char *) * (len + 1));
	if (!full_cmd)
		error_exit("minishell: fatal malloc error\n", rs);
	while (array[ctr[1]] && array[ctr[1]][0] != '|')
		create_full_cmd(full_cmd, array, ctr, len);
	full_cmd[ctr[0]] = 0;
	remove_quotes(rs, full_cmd);
	ft_lstadd_back(&rs->list, create_node(full_cmd, fd, rs));
	get_next_node(rs, array, ctr);
}
