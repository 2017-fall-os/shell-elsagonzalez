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
  char * exitStr = "exit\n";
  char * cdStr = "cd ";
  char * answer = (char *)malloc(100*sizeof(char));
  char isExiting = 1;
  do{
    printf("%c", '$');
    char * j = fgets(answer, 512, stdin);
    if(answer && *answer){
      //check if it is exit
      isExiting = 1;
      int i;
      for(i = 0; i < 5; i++){
	if(answer[i] != exitStr[i]){
	  isExiting = 0;
	  break;
	}
      }
      if(isExiting == 0){
	//check if it is cd
	char isCd = 1;
	for(i = 0; i < 3; i++){
	  if(answer[i] != cdStr[i]){
	    isCd = 0;
	    break;
	  }
	}
	if(isCd){
	  char * path;
	  //check if it is ..
	  char * backStr = "..\n";
	  char isGoingBack = 1;
	  for(i = 0; i < 3; i++){
	    if(backStr[i] != answer[i+3]){
	      isGoingBack = 0;
	      break;
	    }
	  }
	  if(isGoingBack){
	    char currPath[512];
	    char * cwd = getcwd(currPath, 512);
	    char lastSlash = 0;
	    for(i = 0; currPath[i] && currPath[i] != '\n' && currPath[i] != '\0'; i++){
	      if(currPath[i] == '/'){
		lastSlash = i;
	      }
	    }
	    path = subset(path, 0, lastSlash);
	  }
	  else{
	  for(i; answer[i] && answer[i] != '\n' && answer[i] != '\0'; i++){
	    //count length
	  }
	  path = subset(answer, 3, i);
	  if(includesPath(path) == 0){
	    char currPath[512];
	    char * cwd = getcwd(currPath, 512);
	    path = concat(currPath, path);
	  }
	  }
	  int result = chdir(path);
	}
	else { //not cd
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
	    else {
	      int wc = wait(NULL);
	    }
	  } //end if path found
	  else {
	    printf("Command not found\n");
	  }
	  deleteTokens(tokens);
	}//end if cd
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
