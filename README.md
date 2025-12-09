# FIRSTPROJECT_M
MiniShell – A Simple Command Line Interpreter in C
MiniShell is a lightweight Unix-like command interpreter implemented in C.
It supports basic command execution using fork() and execvp(), along with simple built-in commands such as cd, help, and exit.

This project demonstrates fundamental OS and systems programming concepts such as:

Process creation

Command parsing

Tokenization

Directory handling

Built-in command handling

Features
Command Prompt
Displays the current working directory dynamically:

/home/user$
Built-in Commands
Command	Description
cd <path>	Change the current directory
help	Display available built-in commands
exit	Exit the shell
External Program Execution
Runs any valid Linux command via execvp(), e.g.:

ls -l
gcc program.c -o program
echo "Hello"
Robust Input Handling
Reads input using getline()

Splits commands using strtok()

Supports tokenization with spaces, tabs, and newlines

Process Management
Uses fork() to create child processes and waitpid() to wait for execution to finish.

Project Structure
MiniShell/
│── shell.c      # Main source code
│── README.md    # Documentation
│── Makefile     # (Optional) Build automation
How to Build
Compile using GCC:

gcc shell.c -o minishell
Or with warnings enabled:

gcc shell.c -o minishell -Wall -Wextra -g
How to Run
Start the shell:

./minishell
Example session:

/home/user$ ls
Documents Music shell.c
/home/user$ cd Documents
/home/user/Documents$ help
Mini Shell - simple command interpreter in C
Built-in commands:
  cd
  help
  exit
Use the man command for information on other programs.
/home/user/Documents$ exit
 How It Works (High-Level)
shell_loop()

Displays prompt

Reads user input

Tokenizes input

Executes command

split_line()

Converts input string into an array of tokens

execute_command()

Checks if the command is a built-in

If not, forwards it to launch_process()

launch_process()

Creates a child using fork()

Replaces child process with command using execvp()

Parent waits for child to finish

Requirements
GCC compiler

Linux/Unix environment

Basic knowledge of command line operations

Limitations
No support for pipes (|)

No support for I/O redirection (>, <)

No command history

No environment variable expansion ($HOME)

These can be added as future upgrades.

Potential Improvements
If you want to grow this project further:

Implement &&, ||, and ; command chaining

Add color-coded prompts

Add support for piping and redirection

Build command history with ↑ ↓ keys

Add background process execution (&)

Author
ANIMALLA MAHATHI DEVI
A simple shell project written in C for learning OS and system programming concepts.
