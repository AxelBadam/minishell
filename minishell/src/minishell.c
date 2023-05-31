/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:20:59 by ekoljone          #+#    #+#             */
/*   Updated: 2023/05/31 17:15:57 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status;

void	error_exit(char *str, t_resrc *resource)
{
	ft_putstr_fd(str, 2);
	if (resource)
	{
		if (resource->list)
			free_all_nodes(&resource->list);
		if (resource->array)
			free_string_array(resource->array);
		free(resource);
	}
	exit(1);
}

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
	print_error("syntax error unclosed quotes\n", 69, NULL);
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
				iterate_quotes(line, ctr, line[ctr[0] - 1], 1);
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
				if (!iterate_quotes(line, ctr, line[ctr[0] - 1], 0))
					return (0);
			if (line[ctr[0]] == ' ' || !line[ctr[0]])
				ctr[1]++;
		}
	}
	return (ctr[1]);
}

char	**make_array(char *line)
{
	int		tmp[3];
	int		ctr;
	char	**array;

	ctr = 0;
	tmp[0] = 0;
	tmp[1] = 0;
	tmp[2] = count_words(line);
	if (!tmp[2])
		return (NULL);
	array = (char **)malloc(sizeof(char *) * (tmp[2] + 1));
	if (!array)
		return (NULL);
	while (tmp[2] > 0)
	{
		tmp[1] = len_ctr(&line[tmp[0]]);
		tmp[0] += tmp[1];
		array[ctr] = (char *)malloc(sizeof(char) * (tmp[1] + 1));
		if (!array[ctr++])
			return (NULL);
		while (line[tmp[0]] == ' ')
			tmp[0]++;
		tmp[2]--;
	}
	return (array);
}

void	iterate_over_spaces(char *line, int *tmp)
{
	while (line[tmp[0]] == ' ')
		tmp[0]++;
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
		iterate_over_spaces(line, tmp);
		len = len_ctr(&line[tmp[0]]);
		tmp[1] = tmp[0];
		tmp[0] += len;
		while (--len > -1)
			array[index[0]][index[1]++] = line[tmp[1]++];
		array[index[0]][index[1]] = 0;
		index[1] = 0;
		index[0]++;
		iterate_over_spaces(line, tmp);
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
			if (dst[c[1]] == '$' || (!dst[c[1] - 1] && dst[c[1]] == '~'
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
			expand_dollar_sign(array, ctr, env);
			if (ctr[1] == 0 && array[ctr[0]][ctr[1]] == '~'
				&& (array[ctr[0]][ctr[1] + 1] == '/'
					|| !array[ctr[0]][ctr[1] + 1]))
				add_expansion(array, array[ctr[0]], get_env("HOME", env), 1);
			if (array[ctr[0]][ctr[1] - 1] == '\'')
				if (ft_strchr(&array[ctr[0]][ctr[1]], '\''))
					iterate_quotes(array[ctr[0]], &ctr[1], '\'', 0);
		}
		ctr[1] = -1;
		ctr[0]++;
	}
}

int	count(char **array, int *ctr, int strings)
{
	int	start;

	start = 0;
	if (array[ctr[0]][ctr[1]] == '>' || array[ctr[0]][ctr[1]] == '<'
		|| array[ctr[0]][ctr[1]] == '|')
	{
		start = ctr[1];
		if (ctr[1] > 0)
		strings++;
		while (array[ctr[0]][ctr[1]] && (array[ctr[0]][ctr[1]] == '>'
			|| array[ctr[0]][ctr[1]] == '<' || array[ctr[0]][ctr[1]] == '|'))
			ctr[1]++;
		if (array[ctr[0]][ctr[1]])
			strings++;
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
				iterate_quotes(array[ctr[0]],
					&ctr[1], array[ctr[0]][ctr[1] - 1], 0);
			}
			strings = count(array, ctr, strings);
		}
		ctr[1] = -1;
		ctr[0]++;
	}
	printf("strings == %i\n", strings);
	return (strings);
}

//potential problems

int	fill(char **n_arr, char **o_arr, int *c, int *i)
{
	if (o_arr[c[0]][c[1]] == '"' || o_arr[c[0]][c[1]] == '\'')
	{
		c[1]++;
		iterate_quotes(o_arr[c[0]], &c[1], o_arr[c[0]][c[1] - 1], 0);
	}
	if (o_arr[c[0]][c[1]] == '<' || o_arr[c[0]][c[1]] == '>'
		|| o_arr[c[0]][c[1]] == '|')
	{
		if (c[1])
		{
			n_arr[i[0]++] = ft_substr(o_arr[c[0]], i[1], c[1] - i[1]);
			i[1] = c[1];
		}
		while (o_arr[c[0]][c[1]] && (o_arr[c[0]][c[1]] == '<'
			|| o_arr[c[0]][c[1]] == '>' || o_arr[c[0]][c[1]] == '|'))
			c[1]++;
		n_arr[i[0]++] = ft_substr(o_arr[c[0]], i[1], c[1] - i[1]);
		i[1] = c[1];
		if (!o_arr[c[0]][c[1]])
			return (1);
	}
	if (!o_arr[c[0]][c[1] + 1])
		n_arr[i[0]++] = ft_substr(o_arr[c[0]], i[1], c[1] + 1 - i[1]);
	return (0);
}

void	fill_array_with_operators(char **n_arr, char **o_arr)
{
	int	ctr[2];
	int	index[2];

	ctr[0] = 0;
	ctr[1] = -1;
	index[0] = 0;
	index[1] = 0;
	while (o_arr[ctr[0]])
	{
		while (o_arr[ctr[0]][++ctr[1]])
		{
			if (fill(n_arr, o_arr, ctr, index))
				break ;
		}
		index[1] = 0;
		ctr[1] = -1;
		ctr[0]++;
	}
	printf("INDEX %i\n", index[0]);
	n_arr[index[0]] = 0;
}

char	**make_array_with_operators(char **array, int strings)
{
	int		ctr;
	char	**n_arr;

	ctr = 0;
	while (array[ctr])
		ctr++;
	printf("strings AAA == %i\n", ctr + strings + 1);
	n_arr = (char **)malloc(sizeof(char *) * (ctr + strings + 1));
	if (!n_arr)
		return (NULL);
	fill_array_with_operators(n_arr, array);
	return (n_arr);
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
	char	**n_arr;

	int ctr = 0;
	while (array[ctr])
		printf("%s\n", array[ctr++]);
	strings = count_strings(array);
	if (!strings)
		return (array);
	n_arr = make_array_with_operators(array, strings);
	free_string_array(array);
	ctr = 0;
	while (n_arr[ctr])
		printf("%s\n", n_arr[ctr++]);
	return (n_arr);
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
		if (old_str[ctr[0]] && (old_str[ctr[0]] != '"'
				|| old_str[ctr[0]] != '\''))
			new_str[ctr[1]++] = old_str[ctr[0]++];
	}
	new_str[ctr[1]] = 0;
	free(old_str);
	return (new_str);
}

void	remove_quotes(t_resrc *rs, char **array)
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
		{
			array[ctr[0]] = make_new_str(array[ctr[0]], len);
			if (!array[ctr[0]++])
				error_exit("minishell: fatal malloc error\n", rs);
		}
	}
}

char	**split_command(t_resrc *rs, char *line, char **env)
{
	char	**array;

	array = make_array(line);
	if (!array)
		return (NULL);
	fill_array(line, array);
	expand(array, env);
	array = split_by_operator(array);
	if (!array)
		error_exit("minishell: fatal malloc error\n", rs);
	return (array);
}

void	create_heredoc(int *fd, char *delimitor)
{
	char	*line;

	g_exit_status = 0;
	while (g_exit_status != 1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimitor, SIZE_MAX) == 0)
			break ;
		ft_putendl_fd(line, fd[1]);
		free(line);
	}
	close(fd[1]);
	if (g_exit_status == 1)
		close(fd[0]);
	fd[1] = 1;
	if (line)
		free(line);
}

int	print_error(char *str, int exit_status, char *filename)
{
	ft_putstr_fd("minishell: ", 2);
	if (filename)
		ft_putstr_fd(filename, 2);
	ft_putstr_fd(str, 2);
	g_exit_status = exit_status;
	return (0);
}

int	open_output_redirect(char *redirect, char *filename, int *fd)
{
	if (access(filename, F_OK) == 0 && access(filename, W_OK == -1)
		&& (ft_strncmp(redirect, ">", SIZE_MAX) == 0
			|| ft_strncmp(redirect, ">>", SIZE_MAX) == 0))
		if (!print_error(": Permission denied\n", 69, filename))
			return (-1);
	if (ft_strncmp(redirect, ">", SIZE_MAX) == 0)
		fd[1] = open(filename, O_CREAT | O_WRONLY, 0644);
	else if (ft_strncmp(redirect, ">>", SIZE_MAX) == 0)
		fd[1] = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd[1] == -1 || fd[0] == -1)
		if (!print_error("fatal: open fail\n", 69, NULL))
			return (-1);
	return (0);
}

int	open_input_redirect(char *redirect, char *filename, int *fd)
{
	if (ft_strncmp(redirect, "<<", SIZE_MAX) == 0)
	{
		if (pipe(fd) == -1)
			if (!print_error("FATAL: Pipe fail\n", 69, NULL))
				return (-1);
		create_heredoc(fd, filename);
	}
	else if (ft_strncmp(redirect, "<", SIZE_MAX) == 0)
	{
		if (access(filename, F_OK) != 0
			|| (access(filename, F_OK) == 0 && access(filename, R_OK) != 0))
		{
			if (access(filename, F_OK) != 0)
			{
				if (!print_error(": No such file or directory\n", 69, filename))
					return (-1);
			}
			else if (!print_error(": Permission denied\n", 69, filename))
				return (-1);
		}
		fd[0] = open(filename, O_RDONLY);
	}
	return (0);
}

int	is_a_directory(char *filename)
{
	struct stat	statbuf;
	int			dir;

	if (stat(filename, &statbuf) != 0)
		return (0);
	dir = S_ISDIR(statbuf.st_mode);
	if (dir)
		print_error(": Is a directory\n", 258, filename);
	return (dir);
}

int	open_file(t_resrc *rs, char *redirect, char *filename, int *fd)
{
	int	len;

	if (!filename)
	{
		print_error("syntax error near unexpected token `newline'\n",
			69, filename);
		return (-1);
	}
	len = str_len_without_quotes(filename);
	if (len != ft_strlen(filename))
		filename = make_new_str(filename, len);
	if (!filename)
		error_exit("minishell: fatal malloc error\n", rs);
	if (is_a_directory(filename))
		return (-1);
	if (open_input_redirect(redirect, filename, fd) == -1)
		return (-1);
	if (open_output_redirect(redirect, filename, fd) == -1)
		return (-1);
	return (0);
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
	char	**n_arr;
	int		ctr[2];

	ctr[0] = 0;
	ctr[1] = 0;
	n_arr = (char **)malloc(sizeof(char *)
			* (get_array_size(array) + get_array_size(pipe_command) + 1));
	if (!n_arr)
		error_exit("minishell: fatal malloc error\n", resource);
	while (array[ctr[0]])
		n_arr[ctr[0]++] = ft_strdup(array[ctr[0]]);
	while (pipe_command[ctr[1]])
		n_arr[ctr[0]++] = ft_strdup(pipe_command[ctr[1]++]);
	n_arr[ctr[0]] = 0;
	free_string_array(array);
	free_string_array(pipe_command);
	resource->array = n_arr;
}

int	get_new_command(t_resrc *resource, char **array)
{
	char	*line;
	char	**pipe_command;

	g_exit_status = 0;
	line = readline("> ");
	if (!line)
		print_error("syntax error: unexpected end of file\n", 258, NULL);
	if (line && g_exit_status != 1)
	{
		while (!*line)
			line = readline("> ");
		pipe_command = split_command(resource, line, resource->envp);
		add_array_to_array(resource, array, pipe_command);
		free(line);
		return (1);
	}
	free_all_nodes(&resource->list);
	return (0);
}

int	check_pipe_syntax(char **array, int *ctr)
{
	int	error;

	error = 0;
	if (array[ctr[0]][1])
		error = 258;
	if (array[ctr[0] + 1])
		if (array[ctr[0] + 1][0] == '>' || array[ctr[0] + 1][0] == '<'
			|| array[ctr[0] + 1][0] == '|')
			error = 258;
	if (error)
	{
		print_error("syntax error near unexpected token `|'\n", 258, NULL);
		return (0);
	}
	return (1);
}

int	check_syntax(char **array, int *ctr, char d)
{
	int	error;

	error = 0;
	if (d == '|')
		if (!check_pipe_syntax(array, ctr))
			return (0);
	if ((array[ctr[0]][1] && array[ctr[0]][1] != d) || array[ctr[0]][2])
		return (0);
	if (array[ctr[0] + 1])
	{
		if (array[ctr[0] + 1][0] == '>' || array[ctr[0] + 1][0] == '<'
			|| array[ctr[0] + 1][0] == '|')
		{
			print_error("syntax error near unexpected token `>'\n", 258, NULL);
			return (0);
		}
	}
	return (1);
}

int	get_len_without_redirects(t_resrc *rs, char **ar, int *fd)
{
	int	c[2];

	c[0] = 0;
	c[1] = 0;
	while (ar[c[0]])
	{
		if (ar[c[0]][0] == '>' || ar[c[0]][0] == '<' || ar[c[0]][0] == '|')
		{
			if (!check_syntax(ar, c, ar[c[0]][0]))
				return (0);
			if (ar[c[0]][0] == '|')
				break ;
			if (fd[1] != 1)
				close(fd[1]);
			if (open_file(rs, ar[c[0]], ar[c[0] + 1], fd) == -1)
				return (0);
			if (ar[c[0] + 1])
				c[1] -= 2;
			else
				c[1]--;
		}
		c[0]++;
		c[1]++;
	}
	return (c[1]);
}

char	*create_full_path(char *cmd, char *path, int start, int len)
{
	char	*full_path;

	full_path = (char *)malloc(sizeof(char) * (len));
	full_path = ft_substr(path, start, len - 1);
	full_path = ft_strjoin(full_path, "/");
	full_path = ft_strjoin(full_path, cmd);
	if (!full_path)
		return (NULL);
	return (full_path);
}

char	*get_full_path(t_resrc *rs, char *cmd, char *path)
{
	char	*full_path;
	int		ctr[2];
	int		len;

	ctr[0] = 0;
	ctr[1] = 0;
	len = 0;
	while (path[ctr[0]])
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
		ctr[0]++;
	}
	free(path);
	return (full_path);
}

int	is_builtin(char *str)
{
	char	*tmp;
	int		ctr;
	int		re;

	tmp = ft_strdup(str);
	ctr = -1;
	re = 0;
	while (tmp[++ctr])
		tmp[ctr] = ft_tolower(tmp[ctr]);
	if (!ft_strncmp(tmp, "pwd", ctr))
		re = 1;
	else if (!ft_strncmp(tmp, "env", ctr))
		re = 1;
	else if (!ft_strncmp(tmp, "cd", ctr))
		re = 1;
	else if (!ft_strncmp(tmp, "export", ctr))
		re = 1;
	else if (!ft_strncmp(tmp, "unset", ctr))
		re = 1;
	else if (!ft_strncmp(tmp, "echo", ctr))
		re = 1;
	else if (!ft_strncmp(tmp, "exit", ctr))
		re = 1;
	free(tmp);
    return (re);
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
	v.ctr[0] = 0;
	remove_quotes(rs, v.full_cmd);
	ft_lstadd_back(&rs->list, create_node(v.full_cmd, v.fd, rs));
	if (array[v.ctr[1]])
	{
		if (!array[v.ctr[1] + 1])
			if (!get_new_command(rs, array))
				return ;
		make_list(rs, &rs->array[v.ctr[1] + 1]);
	}
}

void	print_list(t_list **head)
{
	t_list	*tmp;
	int		ctr;

	ctr = 0;
	tmp = *head;
	while (tmp)
	{
		printf("FULL CMD = ");
		while (tmp->command.full_cmd[ctr])
			printf("%s ", tmp->command.full_cmd[ctr++]);
		printf("\nFULL PATH = %s\n", tmp->command.full_path);
		printf("OUTPUT_FD = %i\nINPUT_FD = %i\n",
			tmp->command.output_fd, tmp->command.input_fd);
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

void set_env(t_resrc *resrc)
{
	char *prefix;
	char *last_arg;
	int size;
	char *str;
	
	prefix = "_=";
	size = get_2d_array_size(resrc->list->command.full_cmd);
	last_arg = resrc->list->command.full_cmd[size - 1];
	str = ft_strjoin(prefix, last_arg);
	if (!str)
		error_exit("minishell: fatal malloc error\n", resrc);
	if (is_in_env(str, resrc->envp))
		resrc->envp = replace_str(str, resrc->envp);
	else if (ft_strchr(str, '=') != NULL)
		resrc->envp = append_2d(resrc->envp, str);
}

void	minishell(t_resrc *resrc)
{
	char	*line;

	line = readline("minishell-1.0$ ");
	while (line)
	{
		if (*line)
		{
			resrc->array = split_command(resrc, line, resrc->envp);
			add_history(line);
			if (resrc->array)
			{
				make_list(resrc, resrc->array);
				print_list(&resrc->list);
				if (resrc->list)
				{
					execution(resrc, resrc->list);
					set_env(resrc);
				}
				free_string_array(resrc->array);
				free_all_nodes(&resrc->list);
			}
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
		error_exit("minishell: fatal malloc error\n", NULL);
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
		error_exit("minishell: fatal malloc error\n", NULL);
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
		error_exit("minishell: fatal malloc error\n", NULL);
	while (env[ctr[0]])
	{
		if (ft_strncmp("SHLVL", env[ctr[0]], 5) == 0)
			envp[ctr[0]] = shlvl(env[ctr[0]]);
		else
			envp[ctr[0]] = ft_strdup(env[ctr[0]]);
		if (!envp[ctr[0]++])
			error_exit("minishell: fatal malloc error\n", NULL);
	}
	envp[ctr[0]] = 0;
	return (envp);
}

void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		g_exit_status = 1;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

int	main(int argc, char **argv, char **env)
{
	t_resrc	*resrc;

	(void)argc;
	(void)argv;
	g_exit_status = 0;
	signal(SIGINT, signal_handler);
	resrc = init_resources(create_env(env));
	minishell(resrc);
	free_string_array(resrc->envp);
	free(resrc);
	return (0);
}
