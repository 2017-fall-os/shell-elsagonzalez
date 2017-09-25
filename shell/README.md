# Tokenizer by elsagonzalez

This directory contains:
* an implementation for a tokenizer that separates a string into sub-strings and allocates them.
* and a terminal interface for testing

The program will initially run the test cases provided in the code and then will prompt the user the user to enter additional test cases. For each test case, the program will print each of the strings tokens in a separate line.
After this, the program will then free the used space in memory. When prompted with the "$" to enter a new string for testing in the terminal, type "exit" to end the program or type any other string to tokenize it.

This demo contains the following files:
* mytoc.c: implements the tokenizer, given a string and a delimiter, it breaks down the string into sub-strings and allocates them into memory.
* mytoc.h: header file for tokenizer and serves as "public" interface for the functions.
* test.c: implements the terminal-based user interface for the tokenizer.

To compile:
~~~
$ make test
~~~

To run:
~~~
$ ./test
~~~

To delete binaries:
~~~
$ make clean
~~~