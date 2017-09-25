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
