# Mini Shell

## Overview
Mini Shell is a basic command-line shell implemented in C. It supports the execution of shell commands, including built-in commands, external commands, process management, file locking, and redirection. It provides a simple interface for interacting with the operating system through various system calls.

## Features
- **Custom Shell**: A simple shell implementation in C with support for user commands.
- **Built-in Commands**: Includes basic shell commands such as `exit`, `cd`, and custom `delep`.
- **Command History**: Maintains a history of commands entered by the user.
- **Wildcard Expansion**: Supports wildcard characters (e.g., `*`) for file matching.
- **Pipeline Support**: Supports pipeline operations to execute multiple commands in sequence.
- **Redirection**: Supports input and output redirection using `>` and `<`.
- **Process Management**: Supports process creation, termination, and handling signals.

## Requirements
- **C Compiler**: GCC or any standard C compiler.
- **Libraries**:
  - `readline`: For reading input with command history support.
  - `unistd.h`: For system calls like `fork()`, `exec()`, `kill()`, etc.
  - `sys/wait.h`: For process management (`waitpid()`).

## Installation
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/RounakMandal3000/Mini-Shell.git
   cd Mini-Shell
2. **Build the Project**:
   Use make to compile the project:
   ```bash
   make -f make_shell.mak
3. **Run the Shell**:
   After building the project, run the Mini Shell:
   ```bash
   ./myshell
