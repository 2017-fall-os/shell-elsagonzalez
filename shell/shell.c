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
  char * answer = (char *)malloc(256*sizeof(char));
  char isExiting = 1;
  do{
    //printf("%c", '$');
    char * j = fgets(answer, 256, stdin);
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
	  for(i; answer[i] && answer[i] != '\n' && answer[i] != '\0'; i++){
	    //count length
	  }
	  path = subset(answer, 3, i);
	  if(includesPath(path) == 0){
	    char currPath[512];
	    char * cwd = getcwd(currPath, 512);
	    path = concat(currPath, path);
	  }
	  int result = chdir(path);
	}
	else { //not cd
	  tokens = mytoc(answer, '|');
	  for(i = 0; tokens[i] && tokens[i] != NULL; i++){
	    //count commands
	  }
	  printf("num of commands is %d\n", i);
	  int curr = 0;
	  char readingFromPipe = 0;
	  int * pipeFileDes1 = 0; //for input
	  int * pipeFileDes2 = 0; //for output
	  int stdin_copy = 0;
	  int stdout_copy = 0;
	  int ret;
	  for(i; i != 0; i--){
	    if(readingFromPipe || i > 1){
	      stdin_copy = dup(0);
	      stdout_copy = dup(1);
	    }
	    if(i > 1){
	      //pipe
	      pipeFileDes2 = (int *) calloc(2, sizeof(int));
	      ret = pipe(pipeFileDes2);
	    }
	    char pathFound = 0;
	    char ** command = mytoc(tokens[curr], ' ');
	    if(includesPath(command[0]) == 0){
	      //look for the path in $PATH
	      char ** paths = mytoc(getenv("PATH"), ':');
	      int k;
	      for(k = 0; paths[k] && paths[k] != '\0'; k++){
		char * file = concat(paths[k], command[0]);
		//printf("path is %s, and file is %s\n", paths[k], file);
		struct stat temp;
		if(stat(file, &temp) == 0 && temp.st_mode & S_IXUSR){
		  printf("Path was found\n");
		  command[0] = file;
		  pathFound = 1;
		  break;
		}
	      }
	    }
	    else {
	      pathFound = 1;
	    }
	    if(pathFound){
	      //printf("I will fork\n");
	      int rc = fork();
	      if(rc < 0){
		printf("Fork failed");
	      }
	      else if(rc == 0){ //child
		if(readingFromPipe){
		  printf("I am going to read\n");
		  close(0);
		  ret = dup(pipeFileDes1[0]);
		}
		if(i > 1){
		  printf("I am going to write\n"); 
		  close(1);
		  ret = dup(pipeFileDes2[1]);
		}
		int returnVal = execve(command[0], command, envp);
		exit(0);
	      }
	      else { //parent
		int wc = wait(NULL);
	      }
	      if(readingFromPipe || i > 1){
		free(pipeFileDes1);
		//return input and output to their original state
		dup2(stdin_copy, 0);
		dup2(stdout_copy, 1); 
		close(stdin_copy);
		close(stdout_copy);
	      }
	      if(i > 1){
		//swap pipes
		pipeFileDes1 = pipeFileDes2;
		readingFromPipe = 1;
	      }
	      curr++;
	    }
	    else {
	      printf("Command not found\n");
	      break;
	    }//end if path found
	  }//end for loop
	  deleteTokens(tokens);
	}//end if cd
      } //end if exiting
    } //end if answer
    printf("I am here .-. \n");
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
