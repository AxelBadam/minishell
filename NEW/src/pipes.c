#include "linked_list.h"
#include "minishell.h"


void close_pipes(int command_count, int *pipefds)
{
	int i;

	i = -1;
	while (++i < command_count * 2)
		close(pipefds[i]);
}

void setup_pipes(int pipefds[], int current_command, int total_commands) 
{
    if (current_command > 0) // check if first
	{
        dup2(pipefds[(current_command - 1) * 2], STDIN_FILENO);
		close(pipefds[(current_command - 1) * 2]);
	}
    if (current_command < total_commands - 1) // check if not last
	{
		dup2(pipefds[current_command * 2 + 1], STDOUT_FILENO);
		close(pipefds[current_command * 2 + 1]);
	}
}

void create_pipes(int command_count, int *pipefds)
{
	int i;
	
	i = 0;
	while (i < command_count - 1)
	{
		if (pipe(&pipefds[i * 2]) < 0)
		{
			write(1, "error pipe", 10);
			return ;
		}
		i++;
	}
}

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