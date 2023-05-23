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
	while(cmd->env[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (cmd->env[i])
	{
		new_env[i] = ft_strdup(cmd->env[i]);
		i++;
	}
	new_env[i++] = str;
	new_env[i] = 0;
	cmd->env = new_env;
	//write(1, "asd", 1);
	/*while (*cmd->env)
		printf("%s\n", *cmd->env++);*/
	free(new_env);
	return 1;
}

int is_in_env(char *str, Command *cmd)
{
	int len;
	int count;
	int i;
	(void)cmd;
	i = 0;
	count = 1;
	len = ft_strlen(str);
	/*while (cmd->env[i])
	{
		if (ft_strncmp(cmd->env[i], str, SIZE_MAX) == 0)
			count++;
		i++;
	}
	printf("%d", count);*/
	return (count);
}

int execute_builtin_unset(Command *cmd, char ***commands_env)
{
	char *str;
	int i = 0;
	int j = 0;
	char **new_env;
	int len;
	//char *ret = NULL;

	LinkedListNode *args = cmd->args->head;
	args = args->next;
	str = args->value;
	//printf("%s", str);
	len = ft_strlen(str);
	cmd->env = *commands_env;

	if (is_in_env(str, cmd))
	{
		while(cmd->env[i])
			i++;
	new_env = (char **)malloc(sizeof(char *) * (i + 2 - is_in_env(str, cmd)));
	i = 0;
	while (cmd->env[i])
	{
		if (ft_strnstr(cmd->env[i], str, len))
			i++;
		//ret = ft_strnstr(cmd->env[i], str, len);
		//printf("%s", ret);

		new_env[j] = ft_strdup(cmd->env[i++]);
		//ft_printf("%s\n", new_env[j]);
		j++;
	}
	new_env[j] = 0;
	*commands_env = new_env;
	}
	printf("--------------------------\n");
	/*while (*cmd->env)
		printf("%s\n", *cmd->env++);*/
	return 1;
}

int execute_builtin_env(Command *cmd, char ***commands_env)
{	
	cmd->env = *commands_env;
	while (*cmd->env)
		printf("%s\n", *cmd->env++);
	return 1;
}

int execute_builtin_cd(Command *cmd) 
{
    LinkedListNode *arg_node = cmd->args->head;
    char *path = arg_node->next->value;

    if (chdir(path) != 0) 
	{
        error_handling("cd erore");
    }
    return 1;
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
	exit (1);
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