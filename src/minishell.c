/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:20:59 by ekoljone          #+#    #+#             */
/*   Updated: 2023/05/24 15:00:26 by ekoljone         ###   ########.fr       */
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
			if (line[ctr[0] - 1] == '"' || line[ctr[0] - 1] == '\'')
				iterate_quotes(line, ctr, line[ctr[0] - 1] , 1);
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
			if (line[ctr[0] - 1] == '"' || line[ctr[0] - 1] == '\'')
				if (!iterate_quotes(line, ctr, line[ctr[0] - 1] , 0))
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

void	iterate_env(char **env, char *d_string, int *ctr)
{
	char	*str;

	ctr[1] = 0;
	while (env[ctr[0]])
	{
		while (env[ctr[0]][ctr[1]] != '=')
			ctr[1]++;
		str = ft_substr(env[ctr[0]], 0, ctr[1]);
		if (ft_strncmp(str, d_string, SIZE_MAX) == 0)
		{
			free(str);
			break ;
		}
		free(str);
		ctr[0]++;
		ctr[1] = 0;
	}
}

char	*get_env(char *d_string, char **env)
{
	int		ctr[3];
	int		len;
	char	*str;

	ctr[0] = 0;
	ctr[2] = 0;
	len = 0;
	str = NULL;
	iterate_env(env, d_string, ctr);
	ctr[1] = 0;
	if (env[ctr[0]])
	{
		while (env[ctr[0]][ctr[1]] != '=')
			ctr[1]++;
		len = ft_strlen(&env[ctr[0]][++ctr[1]]);
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
			if (dst[ctr[1]] == '$' || (!dst[ctr[1] - 1] && dst[ctr[1]] == '~' && (array[ctr[0]][ctr[1] + 1] == '/' || !array[ctr[0]][ctr[1] + 1])))
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
	free(array[ctr[0]]);
	free(src);
	array[ctr[0]] = new_str;
}

void	expand(char **array, char **env)
{
	int		ctr[2];
	int		len;
	char	*ptr;

	len = 0;
	ctr[0] = 0;
	ctr[1] = -1;
	while (array[ctr[0]])
	{
		while (array[ctr[0]][++ctr[1]])
		{
			if (array[ctr[0]][ctr[1] - 1] == '$')
			{
				len++;
				while (ft_isalpha(array[ctr[0]][ctr[1]]) || array[ctr[0]][ctr[1]] == '_')
				{
					ctr[1]++;
					len++;
				}
				ptr = ft_substr(array[ctr[0]], ctr[1] - len, len);
				add_expansion(array, array[ctr[0]], get_env(ptr + 1, env), len);
				ctr[1] = 0;
				if (ptr)
					free(ptr);
			}
			if (!array[ctr[0]][ctr[1] -  1] && array[ctr[0]][ctr[1]] == '~' && (array[ctr[0]][ctr[1] + 1] == '/' || !array[ctr[0]][ctr[1] + 1]))
				add_expansion(array, array[ctr[0]], get_env("HOME", env), 1);
			if (array[ctr[0]][ctr[1] - 1] == '\'')
				if (ft_strchr(&array[ctr[0]][ctr[1]], '\''))
					iterate_quotes(array[ctr[0]], &ctr[1], '\'', 0);
			len = 0;
		}
		ctr[1] = -1;
		ctr[0]++;
	}
}

int	count(char **array, int *ctr, int strings)
{
	int	start;

	start = 0;
	if (array[ctr[0]][ctr[1]] == '>' || array[ctr[0]][ctr[1]] == '<' || array[ctr[0]][ctr[1]] == '|')
	{
		start = ctr[1];
		strings++;
		while (array[ctr[0]][ctr[1]] && (array[ctr[0]][ctr[1]] == '>' || array[ctr[0]][ctr[1]] == '<' || array[ctr[0]][ctr[1]] == '|'))
			ctr[1]++;
		if (array[ctr[0]][ctr[1]])
			strings++;
		else if (!start && !array[ctr[0]][ctr[1]])
			strings--;
	}
	return (strings);
}

int	count_strings(char **array)
{
	int	ctr[2];
	int	strings;

	strings = 0;
	ctr[0] = 0;
	ctr[1] = -1;
	while (array[ctr[0]])
	{
		while (array[ctr[0]][++ctr[1]])
		{
			if (array[ctr[0]][ctr[1]] == '"' || array[ctr[0]][ctr[1]] == '\'')
			{
				ctr[1]++;
				iterate_quotes(array[ctr[0]], &ctr[1], array[ctr[0]][ctr[1] - 1], 0);
			}
			strings = count(array, ctr, strings);
		}
		ctr[1] = -1;
		ctr[0]++;
	}
	printf("strings == %i\n", strings);
	return (strings);
}

int	fill(char **new_array, char **old_array, int *ctr, int *index)
{
	if (old_array[ctr[0]][ctr[1]] == '"' || old_array[ctr[0]][ctr[1]] == '\'')
	{
		ctr[1]++;
		iterate_quotes(old_array[ctr[0]], &ctr[1], old_array[ctr[0]][ctr[1] - 1], 0);
	}
	if (old_array[ctr[0]][ctr[1]] == '<' || old_array[ctr[0]][ctr[1]] == '>' || old_array[ctr[0]][ctr[1]] == '|')
	{
		if (ctr[1])
		{
			new_array[index[0]++] = ft_substr(old_array[ctr[0]], index[1], ctr[1] - index[1]);
			index[1] = ctr[1];
		}
		while (old_array[ctr[0]][ctr[1]] && (old_array[ctr[0]][ctr[1]] == '<' || old_array[ctr[0]][ctr[1]] == '>' || old_array[ctr[0]][ctr[1]] == '|'))
			ctr[1]++;
		new_array[index[0]++] = ft_substr(old_array[ctr[0]], index[1], ctr[1] - index[1]);
		index[1] = ctr[1];
		if (!old_array[ctr[0]][ctr[1]])
			return (1);
	}
	if (!old_array[ctr[0]][ctr[1] + 1])
		if (!new_array[index[0]])
			new_array[index[0]++] = ft_substr(old_array[ctr[0]], index[1], ctr[1] + 1 - index[1]);
	return (0);
}

void	fill_array_with_operators(char **new_array, char **old_array)
{
	int	ctr[2];
	int	index[2];

	ctr[0] = 0;
	ctr[1] = -1;
	index[0] = 0;
	index[1] = 0;
	while (old_array[ctr[0]])
	{
		while (old_array[ctr[0]][++ctr[1]])
		{
			if (fill(new_array, old_array, ctr, index))
				break ;
		}
		index[1] = 0;
		ctr[1] = -1;
		ctr[0]++;
	}
	printf("INDEX %i\n", index[0]);
	new_array[index[0]] = 0;
}

char	**make_array_with_operators(char **array, int strings)
{
	int		ctr;
	char	**new_array;

	ctr = 0;
	while (array[ctr])
		ctr++;
	printf("strings == %i\n", ctr + strings + 1);
	new_array = (char **)malloc(sizeof(char *) * (ctr + strings + 1));
	if (!new_array)
		return (NULL);
	fill_array_with_operators(new_array, array);
	return (new_array);
}

void	free_string_array(char **array)
{
	int	ctr;

	ctr = 0;
	if (array)
	{
		while (array[ctr])
			free(array[ctr++]);
		free(array);
	}
}

char	**split_by_operator(char **array)
{
	int		strings;
	char	**new_array;

	strings = count_strings(array);
	if (!strings)
		return (array);
	new_array = make_array_with_operators(array, strings);
	free_string_array(array);
	return (new_array);
}

int	str_len_without_quotes(char *str)
{
	int		ctr[2];
	char	d;

	ctr[0] = -1;
	ctr[1] = 0;
	while (str[++ctr[0]])
	{
		if (str[ctr[0]] == '\'' || str[ctr[0]] == '"')
		{
			ctr[1] -= 2;
			d = str[ctr[0]++];
			while (str[ctr[0]] && str[ctr[0]] != d)
				ctr[0]++;
			ctr[0]++;
		}
	}
	return (ctr[0] - ctr[1]);
}

char	*make_new_str(char *old_str, int len)
{
	char	*new_str;
	int		ctr[2];
	char	d;

	ctr[0] = 0;
	ctr[1] = 0;
	new_str = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (NULL);
	while (old_str[ctr[0]])
	{
		if (old_str[ctr[0]] == '"' || old_str[ctr[0]] == '\'')
		{
			d = old_str[ctr[0]++];
			while (old_str[ctr[0]] && old_str[ctr[0]] != d)
				new_str[ctr[1]++] = old_str[ctr[0]++];
			ctr[0]++;
		}
		if (old_str[ctr[0]] && (old_str[ctr[0]] != '"' || old_str[ctr[0]] != '\''))
			new_str[ctr[1]++] = old_str[ctr[0]++];
	}
	new_str[ctr[1]] = 0;
	free(old_str);
	return (new_str);
}

void	remove_quotes(char **array)
{
	int		ctr[2];
	int		len;

	ctr[0] = 0;
	ctr[1] = 0;
	len = 0;
	while (array[ctr[0]])
	{
		len = str_len_without_quotes(array[ctr[0]]);
		if (len == ft_strlen(array[ctr[0]]))
			ctr[0]++;
		else
			array[ctr[0]++] = make_new_str(array[ctr[0]], len);
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
	array = split_by_operator(array);
	return (array);
}

void	create_heredoc(int *fd, char *delimitor)
{
	char	*line;

	line = readline("> ");
	while (ft_strncmp(line, delimitor, SIZE_MAX) != 0)
	{
		ft_putendl_fd(line, fd[1]);
		printf("%s", line);
		free(line);
		line = readline("> ");
	}
	close(fd[1]);
	fd[1] = 1;
	free(line);
}

int	*open_file(char *redirect, char *filename, int *fd)
{
	int	len;

	len = str_len_without_quotes(filename);
	if (len != ft_strlen(filename))
		filename = make_new_str(filename, len);
	if (filename)
	{
		if (ft_strncmp(redirect, ">", SIZE_MAX) == 0)
			fd[1] = open(filename, O_CREAT | O_WRONLY , 0644);
		else if (ft_strncmp(redirect, ">>", SIZE_MAX) == 0)
			fd[1] = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else if	(ft_strncmp(redirect, "<<", SIZE_MAX) == 0)
		{
			pipe(fd);
			create_heredoc(fd, filename);
		}
		else if	(ft_strncmp(redirect, "<", SIZE_MAX) == 0)
			fd[0] = open(filename, O_RDONLY);
	}
	return (fd);
}

int	get_array_size(char **array)
{
	int	ctr;

	ctr = 0;
	if (!array)
		return (0);
	while (array[ctr])
		ctr++;
	return (ctr);
}

void	add_array_to_array(t_resrc *resource, char **array, char **pipe_command)
{
	char	**new_array;
	int		ctr[2];

	ctr[0] = 0;
	ctr[1] = 0;
	new_array = (char **)malloc(sizeof(char *) * (get_array_size(array) + get_array_size(pipe_command) + 1));
	while (array[ctr[0]])
		new_array[ctr[0]++] = ft_strdup(array[ctr[0]]);
	while (pipe_command[ctr[1]])
		new_array[ctr[0]++] = ft_strdup(array[ctr[1]++]);
	new_array[ctr[0]] = 0;
	free_string_array(array);
	free_string_array(pipe_command);
	resource->array = new_array;
}

void	get_new_command(t_resrc *resource, char **array)
{
	char	*line;
	char	**pipe_command;

	line = readline("> ");
	pipe_command = split_command(line, resource->envp);
	add_array_to_array(resource, array, pipe_command);
	free(line);
}

int	check_syntax(char **array, int *ctr, char d)
{
	if (d == '|')
	{
		if (array[ctr[0]][1])
			return (0);
		return (1);
	}
	if ((array[ctr[0]][1] && array[ctr[0]][1] != d) || array[ctr[0]][2])
		return (0);
	if (!array[ctr[0] + 1]|| (array[ctr[0] + 1] && (array[ctr[0] + 1][0] == '>' ||  array[ctr[0] + 1][0] == '<' ||  array[ctr[0] + 1][0] == '|')))
		return (0);
	return (1);
}

int	get_file_descriptor(char **array, int *fd)
{
	int	ctr[2];

	ctr[0] = 0;
	ctr[1] = 0;
	while (array[ctr[0]])
	{
		if (array[ctr[0]][0] == '>' || array[ctr[0]][0] == '<' || array[ctr[0]][0] == '|')
		{
			if (!check_syntax(array, ctr, array[ctr[0]][0]))
				return (-1);
			if (array[ctr[0]][0] == '|')
				break ;
			if (fd[1] != 1)
				close(fd[1]);
			open_file(array[ctr[0]], array[ctr[0] + 1], fd);
			if (array[ctr[0] + 1])
				ctr[1] -= 2;
			else
				ctr[1]--;
		}
		ctr[0]++;
		ctr[1]++;
	}
	printf("LEN == %i\n", ctr[1]);
	return (ctr[1]);
}

t_list *create_node(char **full_cmd, int *fd)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->command.full_path = NULL;
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
	while (array[ctr[1]] && (array[ctr[1]][0] == '>' || array[ctr[1]][0] == '<'))
	{
		if (array[ctr[1] + 1])
			ctr[1] += 2;
		else
			ctr[1]++;
	}
}

void	make_list(t_resrc *resource, char **array)
{
	t_variables variable;

	variable.ctr[1] = 0;
	variable.ctr[0] = 0;
	variable.fd[0] = 0;
	variable.fd[1] = 1;
	variable.len = get_file_descriptor(array, variable.fd);
	if (!variable.len)
		return ;
	variable.full_cmd = (char **)malloc(sizeof(char *) * (variable.len + 1));
	if (!variable.full_cmd)
		return ;
	while (array[variable.ctr[1]] && array[variable.ctr[1]][0] != '|')
		create_full_cmd(variable.full_cmd, array, variable.ctr, variable.len);
	variable.full_cmd[variable.ctr[0]] = 0;
	variable.ctr[0] = 0;
	while (variable.full_cmd[variable.ctr[0]])
		printf("%s\n", variable.full_cmd[variable.ctr[0]++]);
	remove_quotes(variable.full_cmd);
	ft_lstadd_back(&resource->list, create_node(variable.full_cmd, variable.fd));
	if (array[variable.ctr[1]])
	{
		if (!array[variable.ctr[1] + 1])
			get_new_command(resource, array);
		make_list(resource, &resource->array[variable.ctr[1] + 1]);
	}
}

void	print_list(t_list **head)
{
	t_list *tmp = *head;
	int ctr = 0;

	while (tmp)
	{
		printf("FULL CMD = ");
		while (tmp->command.full_cmd[ctr])
			printf("%s ", tmp->command.full_cmd[ctr++]);
		printf("\nOUTPUT_FD = %i\nINPUT_FD = %i\n", tmp->command.output_fd, tmp->command.input_fd);
		tmp = tmp->next;
		ctr = 0;
	}
}

void	free_all_nodes(t_list **head)
{
	t_list	*tmp;

	tmp = *head;
	while (*head)
	{
		if ((*head)->command.full_cmd)
			free_string_array((*head)->command.full_cmd);
		*head = (*head)->next;
		free(tmp);
		tmp = *head;
	}
	free(*head);
}

void	minishell(t_resrc *resrc)
{
	char	*line;

	line = readline("minishell-1.0$ ");
	while (line)
	{
		if (*line)
		{
			resrc->array = split_command(line, resrc->envp);
			add_history(line);
			make_list(resrc, resrc->array);
			print_list(&resrc->list);
			free_string_array(resrc->array);
			free_all_nodes(&resrc->list);
		}
		free(line);
		line = readline("minishell-1.0$ ");
	}
}

void	*init_resources(char **envp)
{
	t_resrc	*resrc;

	resrc = (t_resrc *)malloc(sizeof(t_resrc));
	if (!resrc)
		return NULL;
	resrc->envp = envp;
	resrc->list = NULL;
	return (resrc);
}

t_list	*ft_lst_last(t_list *head)
{
	while (head->next != NULL)
		head = head->next;
	return (head);
}

void	ft_lstadd_back(t_list **head, t_list *new)
{
	t_list	*tmp;

	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	tmp = ft_lst_last(*head);
	tmp->next = new;
	new->next = NULL;
}

char	*shlvl(char *sys_shlvl)
{
	char	shlvl[7];
	int		lvl;
	char	*level;
	char	*program_shlvl;
	
	lvl = ft_atoi(&sys_shlvl[6]);
	lvl++;
	ft_strlcpy(shlvl, sys_shlvl, 7);
	level = ft_itoa(lvl);
	program_shlvl = ft_strjoin(shlvl, level);
	free(level);
	if (!program_shlvl)
		return (NULL);
	return (program_shlvl);
}

char	**create_env(char **env)
{
	int		ctr[2];
	char	**envp;

	ctr[0] = 0;
	ctr[1] = 0;
	envp = (char **)malloc(sizeof(char *) * (get_array_size(env) + 1));
	if (!envp)
		return (NULL);
	while (env[ctr[0]])
	{
		if (ft_strncmp("SHLVL", env[ctr[0]], 5) == 0)
			envp[ctr[0]++] = shlvl(env[ctr[0]]);
		else
			envp[ctr[0]++] = ft_strdup(env[ctr[0]]);
	}
	envp[ctr[0]] = 0;
	return (envp);
}

int	main(int argc, char **argv, char **env)
{
	t_resrc	*resrc;

	(void)argc;
	(void)argv;
	resrc = init_resources(create_env(env));
	minishell(resrc);
	free_string_array(resrc->envp);
	free(resrc);
	return (0);
}

//hello man