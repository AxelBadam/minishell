/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuliara <atuliara@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:20:59 by ekoljone          #+#    #+#             */
/*   Updated: 2023/05/04 13:31:54 by atuliara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
//#include "input_handling.h" 

int main() 
{
    char *input;
    char *prompt = "minishell> ";

    while (1) 
	{
        input = readline(prompt);
		if (!input) 
		{
            printf("\n");
            break;
        }
        if (input[0] != '\0') {
            add_history(input); // Add input to history
            handle_input(input); // Process the input (parse and execute commands)
        }
        free(input);
    }
    return 0;
}
