/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 12:50:33 by atuliara          #+#    #+#             */
/*   Updated: 2023/05/25 15:55:05 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

int execute_builtin_cd(t_list *list)
{
	char *path;
	
	path = list->command.full_cmd[1];
    if (chdir(path) != 0)
	{
        error_handling("cd erore");
    }
    g_exit_status = 1337;
	return (1);
}

void execute_builtin_pwd()
{
    char cwd[4096];

    if (getcwd(cwd, sizeof(cwd)) != NULL) 
    {
        write(STDOUT_FILENO, cwd, ft_strlen(cwd)); // using STDOUT_FILENO
        write(STDOUT_FILENO, "\n", 1); // using STDOUT_FILENO
    } 
    else 
		error_handling("pwd error");
	g_exit_status = 1338;
}

int execute_builtin_exit()
{
	write(1, "exit\n", 5);
	exit (0);
}

void execute_builtin_env(char **envp)
{
	char **tmp;

	tmp = envp;
	while (*tmp)
	{
		ft_putstr_fd(*tmp++, 1);
		ft_putstr_fd("\n", 1);
	}
}

int execute_builtin_echo(t_command cmd)
{
    int newline;
	int i;

	newline = 1;
	i = 1;
    if (*cmd.full_cmd != 0 && cmd.full_cmd[i] != 0 && ft_strncmp(cmd.full_cmd[i], "-n", 2) == 0)
    {
		newline = 0;
		i++;
	}
    while (cmd.full_cmd[i] != 0)
    {
    	ft_putstr_fd(cmd.full_cmd[i], STDOUT_FILENO); // using STDOUT_FILENO
        if (cmd.full_cmd[++i] != 0)
            write(1, " ", 1); // using STDOUT_FILENO
    }
    if (newline)
    {
        ft_putstr_fd("\n", 1); // using STDOUT_FILENO
    }
    return 0;
}