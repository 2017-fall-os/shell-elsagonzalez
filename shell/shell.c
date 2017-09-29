#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "mytoc.h"
#include "shell.h"

int main(int argc, char *argv[], char *envp[]){
  char ** tokens;
  char * exitStr = "exit";
  char * answer = (char *)malloc(100*sizeof(char));
  char isExiting = 1;
  do{
    printf("%c", '$');
    char * j = fgets(answer, 100, stdin);
    if(answer && *answer){
      isExiting = 1;
      int i;
      for(i = 0; i < 4; i++){
	if(answer[i] != exitStr[i]){
	  isExiting = 0;
	  break;
	}
      }
      if(isExiting == 0){
	char pathFound = 0;
	tokens = mytoc(answer, ' ');
	if(includesPath(tokens[0]) == 0){
	  //look for the path in $PATH
	  char ** paths = mytoc(getenv("PATH"), ':');
	  for(i = 0; paths[i] && paths[i] != '\0'; i++){
	    char * file = concat(paths[i], tokens[0]);
	    printf("path is %s, and file is %s\n", paths[i], file);
	    struct stat temp;
	    if(stat(file, &temp) == 0 && temp.st_mode & S_IXUSR){
	      printf("Path was found\n");
	      tokens[0] = file;
	      pathFound = 1;
	      break;
	    }
	  }
	}
	else {
	  pathFound = 1;
	}
	if(pathFound){
	  printf("I will fork\n");
          int rc = fork();
	  if(rc < 0){
	    printf("Fork failed");
          }
          else if(rc == 0){
	    int returnVal = execve(tokens[0], tokens, envp); 
          }
          else{
            int wc = wait(NULL);
          }
        } //end if path found
        deleteTokens(tokens);
      } //end if exiting
    } //end if answer
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
  char startIndex = -1;
  int i;
  for(i = 0; path[i] && path[i] != '\n' && path[i] != '\0'; i++){
    if(path[i] == '/'){
      startIndex = i+1;
    }
  }
  char isSameFunction = 1;
  if(startIndex == -1){
    isSameFunction = 0;
  }
  else{
    for(i = startIndex; path[i] && path[i] != '\n' && path[i] != '\0'; i++){
      if(path[i] != token[i]){
	isSameFunction = 0;
	break;
      }
    }
  }
  return isSameFunction;
}

char * concat(char * path, char * token){
  int i;
  int charCount = 0;
  for(i = 0; path[i] && path[i] != '\n' && path[i] != '\0'; i++){
    charCount++;
  }
  for(i = 0; token[i] && token[i] != '\n' && token[i] != '\0'; i++){
    charCount++;
  }
  char * str = (char *)malloc(sizeof(char)*(charCount+2)); //+1 for '/' and +1 for terminator
  charCount = 0;
  for(i = 0; path[i] && path[i] != '\n' && path[i] != '\0'; i++){
    str[i] = path[i];
    charCount++;
  }
  str[i] = '/';
  charCount++;
  for(i = 0; token[i] && token[i] != '\n' && token[i] != '\0'; i++){
    str[i+charCount] = token[i]; 
  }
  str[i+charCount] = '\0';
  return str;
}
