/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:30:16 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/09 16:41:54 by ekoljone         ###   ########.fr       */
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
# include <dirent.h>
# include <sys/wait.h>
# include <termios.h>

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
void execute_builtin_pwd();
void execute_builtin_exit();
void execute_builtin_cd(t_resrc *resrc, t_command command);
void execute_builtin_echo(t_command cmd);
void execute_builtin_env(char **envp);
void execute_builtin_unset(t_list *list, t_resrc *resrc);
void execute_builtin_export(t_list *list, t_resrc *resrc);
/*
** ERRORRR
*/
int	print_error(char *str, int exit_status, char *filename);
void error_handling(char *str);
int	print_error(char *str, int exit_status, char *filename);
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
int	print_error(char *str, int exit_status, char *filename);
int	is_a_directory(char *filename);
char	*get_env(char *d_string, char **env);
char *str_to_lower(char *tmp);
void	signal_handler(int signal);

#endif