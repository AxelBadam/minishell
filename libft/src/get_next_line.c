/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 11:28:38 by ekoljone          #+#    #+#             */
/*   Updated: 2023/02/27 12:55:38 by ekoljone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*copyline(char *string)
{
	int		ctr;
	char	*line;

	ctr = 0;
	if (!string[ctr])
		return (NULL);
	while (string[ctr] != '\n' && string[ctr] != '\0')
		ctr++;
	line = (char *)malloc(sizeof(char) * (ctr + 2));
	if (!line)
		return (NULL);
	ctr = 0;
	while (string[ctr] != '\n' && string[ctr] != '\0')
	{
		line[ctr] = string[ctr];
		ctr++;
	}
	if (string[ctr] == '\n')
	{
		line[ctr] = string[ctr];
		ctr++;
	}
	line[ctr] = '\0';
	return (line);
}

char	*readtostring(char *string, int fd)
{
	char	*buf;
	int		a;

	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	a = 1;
	while (!ft_strchr_gnl(string, '\n') && a != 0)
	{
		a = read(fd, buf, BUFFER_SIZE);
		if (a == -1)
		{
			free(buf);
			return (NULL);
		}
		buf[a] = '\0';
		string = ft_strjoin_gnl(string, buf);
	}
	free(buf);
	return (string);
}

char	*newstring(char *string)
{
	int		a;
	int		ctr;
	char	*ptr;

	a = 0;
	while (string[a] && string[a] != '\n')
		a++;
	if (!string[a])
	{
		free(string);
		return (NULL);
	}
	ptr = (char *)malloc(sizeof(char) * (ft_strlen_gnl(string) - a + 2));
	if (!ptr)
		return (NULL);
	a++;
	ctr = 0;
	while (string[a] != '\0')
		ptr[ctr++] = string[a++];
	ptr[ctr] = '\0';
	free(string);
	return (ptr);
}

char	*get_next_line(int fd)
{
	static char	*string;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	string = readtostring(string, fd);
	if (!string)
		return (NULL);
	line = copyline(string);
	string = newstring(string);
	return (line);
}
