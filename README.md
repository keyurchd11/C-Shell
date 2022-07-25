# C-Shell
A shell in C language which supports most functionalities of a Unix shell like I/O redirection, piping, job control etc.

<p align="center">
    <img src="kshell.gif" width="512"/>
</p>

## Division of Code: <br>
* ``headers.h`` contains function declaration for all functions. 
* The other files have the code for implementing the tasks suggested by the name of the files.
* The code starts executing from main.c from where all other functionalities are performed.

## Assumptions: <br>
* The history will be displayed for maximum 20 latest commands. <br>
* At max 100 commands will be given in one line. <br>
* Each command is less than 100 characters. <br>
* All paths involved are less than 1000 characters. <br>
* History is stored in ``txt`` file in the same folder as where the executable is stored.

## Features: <br>
* Builtin commands - {cd,pwd,echo}. <br>
* ls command - implemented various flags including -a - l. <br>
* System commands with and without arguments. <br>
* Foreground: process waits until the commands get executed. <br>
* Background: process doesn't wait for system commands get executed. <br>
* Upon completion of background, a completion message is prompted. <br>
* Repeat command to repeat a command multiple times. <br>
* pinfo command to display process information. <br>
* jobs: it displays information of the currently running background processes <br>
* sig: it kills a background process
* fg: it brings a background process to foreground and runs it
* bg: it stops a running background process.

## How to run the shell: <br>
* The makefile will compile the code. <br>
* Enter ``make`` command to compile the code. <br>
* The shell can be executed by running the ``kshell`` executable. <br>
* ./kshell to run the program.<br>

## Redirection and piping:
* Redirection and piping functionality has been added in the input.c file
* In this file, the redirections of inputs and outputs have been handled.
* Piping of commands is also implemented in the same file.

## Signal Handling: <br>
* <kbd>ctrl</kbd> + <kbd>c</kbd> : terminates any running foreground process
* <kbd>ctrl</kbd> + <kbd>z</kbd> : sends a foreground process to background and changes its state from running to stopped.
* <kbd>ctrl</kbd> + <kbd>d</kbd> : terminates the shell.