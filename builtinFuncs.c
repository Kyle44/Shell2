// Name: Kyle Fritz
// File: builtinFuncs.c
// Date Created: 3/10/16
// Description: Provides the functionality of certain strings built into the shell

#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#define BUFFER_SIZE 1024
#define TOKEN_BUFFER_SIZE 64
#define TOKEN_DELIM " "


int exitFunc(char** args){
        if(strcmp(args[0], "exit") == 0){ // if 1st arg = "exit"
                if(args[2] != NULL){ // if more than one arg after exit
                        printf("ERROR: Not a valid exit code\n");
                        return 1;
                }

                if(args[1] == NULL || atoi(args[1]) == 0){ // exit is same as exit(0)
                        exit(0);
                }
                else if(atoi(args[1]) == 1){
                        exit(1);
                }
                else{
                        printf("ERROR: Not a valid exit code\n");
                        return 1;
                }
        }
}

int chdirFunc(char** args){
        if (args[1] == NULL) {
                if(getenv("HOME")!=NULL){
                        chdir(getenv("HOME"));       
                }
                else{
                       fprintf(stderr, "ERROR: expected argument for changing directories\n");
               }
        }
        else {
                if (chdir(args[1]) != 0) {
                        perror("ERROR");
                }
        }

        return 1;
}

int cdFunc(char** args){
        return chdirFunc(args);
}

int getenvFunc(char** args){
        return 0;
}

int setenvFunc(char** args){
        return 0;
}

int echoFunc(char** args){
        return 0;
}