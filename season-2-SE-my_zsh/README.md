My Zsh README
Problem Statement
The challenge lies in creating a simple shell program that can execute external commands, handle built-in commands, and manage environment variables. Additionally, error handling and a user-friendly prompt are implemented.

Solution
The project provides a basic shell implementation in C. It uses fork() to create a child process for command execution and handles built-in commands such as cd, echo, env, setenv, unsetenv, exit, and quit. The shell supports error messages for common scenarios and displays a simple prompt.

Installation
To compile the code, use the following command:
```
make
```

Usage
After compiling, run the shell with the following command:
```
./my_zsh
```
Commands
Executing Commands:

Enter external commands like in a regular shell.
Example: ls, gcc, etc.
Built-in Commands:

cd [directory]: Change the current working directory.
echo [message or $variable]: Display messages or environment variables.
env: Print the environment variables.
setenv [variable] [value]: Set a new environment variable.
unsetenv [variable]: Unset an environment variable.
exit or quit: Terminate the shell.
Example:

Change directory: cd /path/to/directory
Set environment variable: setenv MY_VAR my_value
Unset environment variable: unsetenv MY_VAR
Display environment variables: env
Execute external command: ls -l
Quit the shell: exit or quit