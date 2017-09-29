#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "mytoc.h"

int main(int argc, char *argv[], char *envp[]){
  char ** tokens;
  char * exitStr = "exit";
  char * answer = (char *)malloc(100*sizeof(char));
  char isExiting = 1;
  do{
    printf("%c", '$');
    char * j = fgets(answer, 100, stdin);
    char pathFound = 0;
    if(answer && *answer){
      int i;
      for(i = 0; i < 4; i++){
	if(answer[i] != exitStr[i]){
	  isExiting = 0;
	  break;
	}
      }
      if(isExiting == 0){
	tokens = mytoc(answer, ' ');
	printTokens(tokens);
	if(includesPath(tokens[0]) == 0){
	  //look for the path in $PATH
	  char ** paths = mytoc($PATH, ':');
	  for(int i = 0; paths[i] && path[i] != '\n' && path[i] != '\0'; i++){
	    if(isSameFunction(paths[i], tokens[0]){
		tokens[0] = paths[i];
		pathFound = 1;
		break;
	    }
	  }
	}
	else {
          pathFound = 1;
        }
	if(pathFound){
	  int rc = fork();
	  if(rc < 0){
	    printf("Fork failed");
	  }
	  else if(rc == 0){
	    int returnVal = execve(tokens[0], &tokens[1], envp); 
	  }
	  else{
	    int wc = wait(NULL);
	  }
	  deleteTokens(tokens);
	}
      }
   } while(isExiting == 0);
}

char includesPath(char * token){
  if(token[0] == '/'){
    return 1;
  }
  else{
    return 0;
  }
}

 char isSameFunction(char * path, char * token){
   char startIndex;
   for(int i = 0; path[i] && path[i] != '\n' && path[i] != '\0'; i++){
     if(path[i] == '/'){
       startIndex = i+1;
     }
   }
   char isSameFunction = 1;
   for(int i = startIndex; path[i] && path[i] != '\n' && path[i] != '\0'; i++){
     if(path[i] != token[i]){
       isSameFunction = 0;
       break;
     }
   }
   return isSameFunction;
 }
