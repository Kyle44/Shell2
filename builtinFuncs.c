// Name: Kyle Fritz
// File: builtinFuncs.c
// Date Created: 3/10/16
// Description: Provides the functionality of certain strings built into the shell

#include <sys/wait.h>
#include <unistd.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <sys/types.h>
//#include <ctype.h>
#define BUFFER_SIZE 1024
#define TOKEN_BUFFER_SIZE 64
#define TOKEN_DELIM " "
#include "utils.c"


int exitFunc(char** args){

    int i = 1; // start at 1 to count args after exit 
    while(args[i]!=NULL){
        i++; // count total arguments
    }
    i--; // one exta i, so need to subtract

    if(i>1){ // if more than one arg after exit
            printf("ERROR: Too many arguments\n");
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
    if(args[2] != NULL){
        fprintf(stderr, "ERROR: One argument expected\n");
        return 1;
    }

	if(getenv(args[1]) != NULL){
        printf(getenv(args[1]));
	}
    printf("\n");
    return 1;
}

int setenvFunc(char** args){
    int currentBufferSize = BUFFER_SIZE;
    char* str = (char*) malloc(sizeof(char*) * currentBufferSize);
    str = "";
    int i = 1;
    while(args[i] != NULL){
        printf("%s\n", i);
        if(strlen(str) > currentBufferSize/2){ // make sure that there's enough memory for str
            currentBufferSize += BUFFER_SIZE;
            realloc(str, currentBufferSize);
        }
        strcat(str, args[i]);
    }    

    printf("%s\n", str);



    args[1] = unescape(str, stderr);
    printf("%s arg1 \n", args[1]);
    char* key;
    char* value;
    int j = 0;

    while(args[1][j] != '='){ // get key
        key += args[1][j];
        j++;
    }

    free(str);
//    setenv(args[1]);
    return 1;
}

int echoFunc(char** args){
    int i = 1;
    while(args[i]!=NULL){
        printf(args[i]);
        i++;
    }
    printf("\n");
    return 1;
}
