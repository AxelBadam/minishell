#include "linked_list.h"
#include "minishell.h"

/*
void close_pipes(int *pipefds)
{
	close(pipefds[0]);
	close(pipefds[1]);
}

void setup_pipes(Command *cmd, int *pipefds) 
{

	cmd->output_fd = pipefds[1];
    dup2(cmd->output_fd, STDOUT_FILENO);
	close(pipefds[1]);
	close(cmd->output_fd);

	cmd->input_fd = pipefds[0];
	dup2(cmd->input_fd, STDIN_FILENO);
	close(cmd->input_fd);
	close(pipefds[0]);
}
*/
void wait_for_child(int command_count)
{
	int i;

	i = -1;
	while (++i < command_count)
		wait(NULL); // store the value in an int
}

int setup_redirections(Command *cmd) 
{
    if (cmd->input_file) 
	{
        cmd->input_fd = open(cmd->input_file, O_RDONLY);
        if (cmd->input_fd == -1) {
            perror("open");
            return -1;
        }
        dup2(cmd->input_fd, STDIN_FILENO);
    }

    if (cmd->output_file) 
	{
        cmd->output_fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (cmd->output_fd == -1) {
            perror("open");
            return -1;
        }
        dup2(cmd->output_fd, STDOUT_FILENO);
    }
    return 0;
}