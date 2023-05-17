#include "minishell.h"
#include "linked_list.h"

int execute_builtin_export(Command *cmd)
{
	char *str;
	int i = 0;
	char **new_env;

	LinkedListNode *args = cmd->args->head;
	args = args->next;
	str = args->value;
	//printf("%s", str);
	while(cmd->env.env[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (cmd->env.env[i])
	{
		new_env[i] = ft_strdup(cmd->env.env[i]);
		i++;
	}
	new_env[i++] = str;
	new_env[i] = 0;
	cmd->env.env = new_env;
	write(1, "asd", 1);
	while (*cmd->env.env)
		printf("%s", *cmd->env.env++);
	free(new_env);
	return 0;
}

int execute_builtin_env(Command *cmd)
{
	//write(1, "asd", 1);
	while (*cmd->env.env)
		printf("%s", *cmd->env.env++);
	return 0;
}

int execute_builtin_cd(Command *cmd) 
{
    LinkedListNode *arg_node = cmd->args->head;
    char *path = arg_node->next->value;

    if (chdir(path) != 0) 
	{
        write(2, "cd, error", 8);
        return (1);
    }
    return 0;
}

int execute_builtin_echo(Command *cmd) 
{
    LinkedListNode *arg_node = cmd->args->head;
    int newline = 1;
    
    if (arg_node != NULL && arg_node->next != NULL && ft_strncmp(arg_node->next->value, "-n", 2) == 0)
    {
        arg_node = arg_node->next;
        newline = 0;
    }

    while (arg_node->next != NULL) 
    {
        if (arg_node->value != NULL)
            ft_putstr_fd(arg_node->next->value, STDOUT_FILENO); // using STDOUT_FILENO
        if (arg_node->next->next != NULL)
            write(1, " ", 1); // using STDOUT_FILENO
        arg_node = arg_node->next;
    }
    if (newline)
    {
        ft_putstr_fd("\n", 1); // using STDOUT_FILENO
    }
    return 0;
}

int execute_builtin_exit()
{
	write(1, "exit\n", 5);
	exit (0);
} 

int execute_builtin_pwd() 
{
    char cwd[4096];
    
    if (getcwd(cwd, sizeof(cwd)) != NULL) 
    {
        write(STDOUT_FILENO, cwd, ft_strlen(cwd)); // using STDOUT_FILENO
        write(STDOUT_FILENO, "\n", 1); // using STDOUT_FILENO
    } 
    else 
    {
        write(2, "pwd, error", 8);
        return (0);
    }
    return (1);
}