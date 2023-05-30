/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:30:16 by ekoljone          #+#    #+#             */
/*   Updated: 2023/05/30 13:09:06 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <unistd.h>
# include <signal.h>
# include "libft/libft.h"
# include <sys/ioctl.h>
# include <sys/stat.h>

typedef struct s_variables
{
	int		ctr[2];
	int		len;
	char	**full_cmd;
	int		fd[2];
}	t_variables;

typedef struct s_command
{
	char	*full_path;
	char	**full_cmd;
	int		output_fd;
	int		input_fd;
}	t_command;

typedef struct s_list
{
	t_command		command;
	struct s_list	*next;
}	t_list;

typedef struct s_resrc
{
	t_list	*list;
	char	**array;
	char	**envp;
}	t_resrc;

void	ft_lstadd_back(t_list **head, t_list *new);
t_list	*ft_lst_last(t_list *head);
/*
** EXEC
*/
void execution(t_resrc *resrc, t_list *list);
/*
** BUILTINS
*/
int execute_builtin_pwd();
int execute_builtin_exit();
int execute_builtin_cd(t_list *list);
int execute_builtin_echo(t_command cmd);
int execute_builtin_env(char **envp);
int execute_builtin_unset(t_list *list, t_resrc *resrc);
int execute_builtin_export(t_list *list, t_resrc *resrc);
/*
** ERRORRR
*/
void error_handling(char *str);
/*
** UTILS
*/
void    free_string_array(char **array);
int linked_list_count(t_list **lst);
void wait_for_child(int command_count);
int is_in_env(char *str, char **envp);
int	get_2d_array_size(char **array);
char **append_2d(char **twod, char *str_to_add);
char **replace_str(char *str, char **envp);
void	free_all_nodes(t_list **head);
int    is_builtin(char *str);

#endif