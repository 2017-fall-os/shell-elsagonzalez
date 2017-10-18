# Shell by elsagonzalez

This directory contains:
* a shell program that is able to execute simple commands and cd.
* an implementation for a tokenizer that separates a string into sub-strings and allocates them.

The program will prompt the user with a "$" to enter a command and any relevant parameters together with it just as you would in a shell interface. The command may include a path, otherwise the program will look for it in the enviroment variables in $PATH. When prompted with the "$" to enter a new command and any relevant parameters in the terminal, type "exit" to end the program or type any other string and the program will attempt to run it as a command with the given parameters.  

This demo contains the following files:
* shell.c: implemets a shell like prgram that given commands is able to create child processes and execute them. The commands may include a path, if not, this program will look for it  in enviroment variables $PATH.
* shell.h: header file for shell.
* mytoc.c: implements the tokenizer, given a string and a delimiter, it breaks down the string into sub-strings and allocates them into memory.
* mytoc.h: header file for tokenizer and serves as "public" interface for the functions.

Testing with testShell.sh will indicate all test cases fail although everything in this shell work except for the background commands.

To compile:
~~~
$ make all
~~~

To run:
~~~
$ ./shell
~~~

To delete binaries:
~~~
$ make clean
~~~