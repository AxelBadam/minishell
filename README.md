# Minishell

Welcome to Minishell, a simplified shell program inspired by bash. This project focuses on heavy parsing, expansion, environment variables, pipes, redirections, heredoc, double quotes, and single quotes.

Features:

• Basic command execution

• Parsing and tokenization of user input

• Environment variable expansion

• Handling single and double quotes

• Input and output redirection

• Command pipelines (| symbol)

• Heredoc functionality

• Getting Started:


Clone the repository: git clone https://github.com/AxelBadam/minishelll.git
Build the Minishell executable: make
Run Minishell: ./minishell
Usage:
After compiling the Minishell executable, run it from the command line. Enter shell commands, and Minishell will interpret and execute them.

Examples:

1. Basic command execution: ls, echo Hello, pwd
2. Variable expansion: echo "Hello, $USER"
3. Input and output redirection: cat < input.txt > output.txt
4. Command pipelines: ls | grep ".txt"
5. Heredoc functionality:
cat << EOF
Line 1
Line 2
EOF
