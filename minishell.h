/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:30:16 by ekoljone          #+#    #+#             */
/*   Updated: 2023/06/15 18:03:46 by ekoljone         ###   ########.fr       */
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
# include <errno.h>

typedef struct s_command
{
	pid_t	pid;
	int		fd[2];
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
void	execution(t_resrc *resrc, t_list *list);
/*
** BUILTINS
*/
void execute_builtin(t_resrc *resrc, t_list *list);
void	execute_builtin_pwd();
void	execute_builtin_exit(char **array, int check);
void	execute_builtin_cd(t_resrc *resrc, t_command command);
void	execute_builtin_echo(t_command cmd);
void	execute_builtin_env(char **envp);
void	execute_builtin_unset(t_list *list, t_resrc *resrc);
void	execute_builtin_export(t_list *list, t_resrc *resrc);
int		check_for_option(char *str);
/*
** BUILTIN_UTIL
*/
char 	**rmv_str_twod(char **env, char *to_rmv);
char 	**append_2d(char **twod, char *str_to_add);
char 	**replace_str(char *str, char **envp);
int 	is_in_env(char *str, char **envp);
int 	update_env(char *var, char *val, t_resrc *resrc);
/*
** ERRORRR
*/
int		print_error(char *str, int exit_status, char *filename);
void	error_handling(char *str);
int		print_error(char *str, int exit_status, char *filename);
/*
** UTILS
*/
void	free_string_array(char **array);
int		linked_list_count(t_list **lst);
void	wait_for_child(int command_count);
int		is_in_env(char *str, char **envp);
int		get_2d_array_size(char **array);
char	**append_2d(char **twod, char *str_to_add);
char	**replace_str(char *str, char **envp);
void	free_all_nodes(t_list **head);
int		is_builtin(char *str);
int		print_error(char *str, int exit_status, char *filename);
int		is_a_directory(char *filename);
char	*str_to_lower(char *tmp);
void	signal_handler(int signal);
void	close_wait(t_list *list);
/*
** SPLIT_COMMAND
*/
char	**split_command(t_resrc *rs, char *line);
char	**make_array(char *line);
void	fill_array(char *line, char **array);
int		iterate_quotes(char *line, int *ctr, char d, int count);
int		count_words(char *line);
/*
** SPLIT_BY_OPERATOR
*/
int		count_strings(char **array);
char	**split_by_operator(char **array);
char	**make_array_with_operators(char **array, int strings);
void	fill_array_with_operators(char **n_arr, char **o_arr);
int		fill(char **n_arr, char **o_arr, int *c, int *i);
/*
** EXPAND
*/
void	expand(char **array, char **env);
void	expand_dollar_sign(char **ar, int *ctr, char **env);
void	add_expansion(char **ar, char *dst, char *src, int rm_len);
void	expand_string(char *src, char *new_str, int *ctr, int rm_len);
char	*create_expanded_string(char *dst, char *src, int rm_len);
/*
** ENV
*/
char	**create_env(char **env);
char	*shlvl(char *sys_shlvl);
char	*get_env(char *d_string, char **env);
void	iterate_env(char **env, char *d_string, int *ctr);
void	set_env(t_resrc *resrc);
/*
** OPEN_REDIRECTS
*/
int		open_file(t_resrc *rs, char *redirect, char *filename, int *fd);
int		open_input_redirect(char *redirect, char *filename, int *fd);
int		open_output_redirect(char *redirect, char *filename, int *fd);
/*
** MAKE_LIST
*/
void	make_list(t_resrc *rs, char **array);
void	create_full_cmd(char **full_cmd, char **array, int *ctr, int len);
t_list	*create_node(char **full_cmd, int *fd, t_resrc *rs);
char	*get_full_path(t_resrc *rs, char *cmd, char *path);
void	get_next_node(t_resrc *rs, char **array, int *ctr);
/*
** UTILS_2
*/
void	ft_lstadd_back(t_list **head, t_list *new);
t_list	*ft_lst_last(t_list *head);
int		get_array_size(char **array);
int		is_a_directory(char *filename);
void	iterate_over_spaces(char *line, int *tmp);
/*
** REMOVE_QUOTES
*/
void	remove_quotes(t_resrc *rs, char **array);
int		str_len_without_quotes(char *str);
char	*make_new_str(char *old_str, int len);
/*
** FREE
*/
void	free_all_nodes(t_list **head);
void	free_string_array(char **array);
void	error_exit(char *str, t_resrc *resource);
int		print_error(char *str, int exit_status, char *filename);
/*
** CHECK
*/
int		is_builtin(char *str);
int		check_syntax(char **array, int *ctr, char d);
int		check_pipe_syntax(char **array, int *ctr);
int		print_syntax_error(char *s, int exit);
int 	check_for_parent_builtin(t_resrc *resrc, t_list *list, int len);
/*
** CHECK 2
*/
int cmd_check(t_list *list);
void check_signal(t_list *list);
int check_input(char **cmd_arr);
int	check_for_option(char *str);
/*
** HEREDOC
*/
void	create_heredoc(int *fd, char *delimitor);
char	**get_new_command(t_resrc *resource);
/*
** UTILS_3
*/
int		get_len_without_redirects(t_resrc *rs, char **ar, int *fd);
int		count(char **array, int *ctr, int strings);
int		len_ctr(char *line);
char	*create_full_path(char *cmd, char *path, int start, int len);
char	**array_dup(char **array);
/*
** PIPES
*/
int 	setup_redir(t_list *list);
void 	setup_pipe(int *fd);
void 	close_pipes(t_list *list, int *fd);

#endif