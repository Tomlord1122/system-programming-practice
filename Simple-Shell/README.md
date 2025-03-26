# Simple Shell Program

## Introduction

This simple shell program supports basic command execution, piping, and built-in commands like cd, exit, and history.

Knowledge you should know:

1. How to use `fork()` to create a new process
2. string function usage
3. signal handling
4. pipe and redirection, `dup2()`: write end and read end
5. `execvp()` family of functions

## Compilation

To compile the program, run the following command in the terminal:

```
make
```

## Execution

To run the shell, execute the compiled binary:

```
./cs5374_sh
```

## Features

- Basic command execution
- Piping between commands
- Built-in commands: cd, exit, history
