#include <stdio.h>
#include <stdlib.h>
#include "mytoc.h"

char** mytoc(char *str, char delim){
  int count = 0;
  char **tokens = 0;
  if(str && *str && *str !='\n'){
    //scan to count number of tokens
    int wordSize = 0;
    int i;
    for(i = 0; str[i] && str[i] != '\n' && str[i] != '\0'; i++){
      if(str[i] == delim && wordSize > 0){ //only counts if there is a word before this delimiter
	count++;
	wordSize = 0;
      }
      else if(str[i] != delim){
	wordSize++;
      }
    }
    //in case there was a word at the end with no delimeter after it
    count = (wordSize > 0)? count+1: count;
    tokens = (char **)calloc(count, sizeof(char *));
    
    //allocate tokens
    int currToken = 0;
    wordSize = 0;
    for(i = 0; str[i] && str[i] != '\n' && str[i] != '\0'; i++){
      if(str[i] == delim && wordSize > 0){
	tokens[currToken] = subset(str, i-wordSize, i);
	currToken++;
	wordSize = 0;
      }
      else if(str[i] != delim){
	wordSize++;
      }
    }
    if(wordSize > 0){
      tokens[currToken] = subset(str, i-wordSize, i); 
    }
  }
  printf("number of tokens: %d\n", count);
  return tokens;
}

void deleteTokens(char ** tokens){
  if(tokens){
    int p;
    for(p = 0; tokens[p]; p++){
      free(tokens[p]);
    }
    free(tokens);
  }
}

/*returns a subset of the string str given a start (inclusive) and an end (exclusive) index*/
char * subset(char * str, int start, int end){
  if(str && *str && *str != '\n'){
    int length = end-start;
    char * subStr = (char *)malloc(sizeof(char)*(length+1)); //+1 for the terminator
    int i;
    for(i = 0; i < length; i++){
      subStr[i] = str[i+start];
    }
    subStr[length] = '\0';
    return subStr;
  }
  else{
    return '\0';
  }
}

void printTokens( char ** tokens){
  if(tokens){
    int p;
    for(p = 0; tokens[p]; p++){
      printf(tokens[p]);
      printf("\n");
    }
  }
}
