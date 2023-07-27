# Minishell

Welcome to Minishell, a simplified shell program inspired by bash. This project focuses on heavy parsing, expansion, environment variables, pipes, redirections, heredoc, double quotes, and single quotes.

Features:

• Basic command execution <br />
• Parsing and tokenization of user input <br />
• Environment variable expansion <br />
• Handling single and double quotes <br />
• Input and output redirection <br />
• Command pipelines (| symbol) <br />
• Heredoc functionality <br />

Getting Started: <br />

Clone the repository: git clone https://github.com/AxelBadam/minishelll.git <br />
Build the Minishell executable: make <br />
Run Minishell: ./minishell <br />

Usage: <br />

After compiling the Minishell executable, run it from the command line. Enter shell commands, and Minishell will interpret and execute them.


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
