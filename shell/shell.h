#ifndef shell_included /*defines mytoc if undefined*/
#define shell_included

/*functions*/
char * getPath(char * command); /*if not included, will look for the path in $PATH and returns the command with path concated into it*/
char includesPath(char *token); /*returns 1 if string contains a path, 0 otherwise*/
char isSameFunction(char *path, char *token); /*parses path and returns 1 if it is the same function and zero otherwise*/
char * concat(char * path, char * token); /*concatenates two strings*/

#endif
