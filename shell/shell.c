#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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
	  int commandsRemaining = i;
	  int curr = 0;
	  char firstCommand = 1; //only this one will not read from pipe
	  int * pipeFileDes1 = 0; //for input
	  int * pipeFileDes2 = 0; //for output
	  int stdin_copy = 0;
	  int stdout_copy = 0;
	  int ret;
	  for(commandsRemaining; commandsRemaining != 0; commandsRemaining--){
	    if(!firstCommand){ //will read from pipe
	      stdin_copy = dup(0); //save stdin to recover it later
	    }
	    if(commandsRemaining > 1){ //will write to pipe
	      stdout_copy = dup(1); //save stdout to recover it later
	      //create pipe
	      pipeFileDes2 = (int *) calloc(2, sizeof(int));
	      ret = pipe(pipeFileDes2);
	      assert(ret == 0);
	    }
	    char ** command = mytoc(tokens[curr], ' ');
	    command[0] = getPath(command[0]); 
	    if(command[0] != NULL){
	      //printf("I will fork\n");
	      int rc = fork();
	      if(rc < 0){
		printf("Fork failed");
	      }
	      else if(rc == 0){ //child
		if(!firstCommand){ //will read from pipe
		  printf("I am going to read\n");
		  //dup2(pipeFileDes1[0], 0);
		  close(0);
		  ret = dup(pipeFileDes1[0]);
		  assert(ret > -1);
		  close(pipeFileDes1[0]);
		  close(pipeFileDes1[1]);
		}
		if(commandsRemaining > 1){ //not the last command, will write to pipe
		  printf("I am going to write\n");
		  //dup2(pipeFileDes2[1], 1);
		  close(1);
		  ret = dup(pipeFileDes2[1]);
		  assert(ret > -1);
		  close(pipeFileDes2[0]);
		  close(pipeFileDes2[1]);
		}
		int returnVal = execve(command[0], command, envp);
		exit(0);
	      }
	      else { //parent
		if(!firstCommand){
		  close(pipeFileDes1[0]);
		  close(pipeFileDes1[1]);
		}
		if(commandsRemaining > 1){
		  close(pipeFileDes2[0]);
		  close(pipeFileDes2[0]);
		}
		else { //last child running, wait for it
		  //int wc = wait(NULL);
		  int waitStat;
		  ret = waitpid(rc, &waitStat, 0);
		}
	      } //end fortking
	      if(!firstCommand){ //read from pipe
		free(pipeFileDes1);
		//return input to its original state
		dup2(stdin_copy, 0); 
		close(stdin_copy);
	      }
	      else{ //if it was the first command
		firstCommand = 0;
	      }
	      if(commandsRemaining > 1){
		//swap pipes
		pipeFileDes1 = pipeFileDes2;
		//return output to its original state
		dup2(stdout_copy, 1);
		close(stdout_copy);
	      }
	      curr++;
	    }
	    else {
	      printf("Command not found\n");
	      break;
	    }//end if path found
	  }//end for loop
	  //deleteTokens(tokens);
	}//end if cd
      } //end if exiting
    } //end if answer
    printf("I am here .-. \n");
  } while(isExiting == 0);
}

char * getPath(char * command){
  char pathFound = 0;
  if(includesPath(command) == 0){
    //look for the path in $PATH
    char ** paths = mytoc(getenv("PATH"), ':');
    int k;
    for(k = 0; paths[k] && paths[k] != '\0'; k++){
      char * file = concat(paths[k], command);
      //printf("path is %s, and file is %s\n", paths[k], file);
      struct stat temp;
      if(stat(file, &temp) == 0 && temp.st_mode & S_IXUSR){
	printf("Path was found\n");
	return file;
      }
    }
  }
  else {
    return command;
  }
  return NULL;
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
