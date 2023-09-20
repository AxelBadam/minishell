# minishell

Welcome to minishell, a simplified shell program inspired by bash. This project focuses on heavy parsing, expansion, environment variables, pipes, redirections, heredoc, double quotes, and single quotes.
![minishell](https://github.com/AxelBadam/minishell/assets/110740560/c946ab91-be47-487a-8106-9fd843a2e10c)

## features:

• Basic command execution <br />
• Parsing and tokenization of user input <br />
• Environment variable expansion <br />
• Handling single and double quotes <br />
• Input and output redirection <br />
• Command pipelines (| symbol) <br />
• Heredoc functionality <br />

## getting started: <br />

Clone the repository: ```git clone https://github.com/AxelBadam/minishelll.git``` <br />
Build the Minishell executable: ```make``` <br />
Run Minishell: ```./minishell``` <br />

## usage: <br />

After compiling the minishell executable, run it from the command line. Enter shell commands, and minishell will interpret and execute them.


Examples:

1. Basic command execution: ls, echo Hello, pwd
2. Variable expansion: echo "Hello, $USER"
3. Input and output redirection: cat < input.txt > output.txt
4. Command pipelines: ls | grep ".txt"
5. Heredoc functionality: <br />
cat << EOF <br />
Line 1 <br />
Line 2 <br />
EOF <br />
