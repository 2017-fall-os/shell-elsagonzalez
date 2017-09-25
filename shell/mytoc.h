#ifndef mytoc_included /*defines mytoc if undefined*/
#define mytoc_included

/*functions*/
char ** mytoc(char *str, char delim); /*returns an array of pointers to allocated strings*/
void deleteTokens(char ** tokens); /*frees data*/
char * subset(char *str, int start, int end); /*returns a subset of the string str given a start (inclusive) and an end index (exclusive)*/
void printTokens(char ** tokens); /*prints tokens*/
#endif
