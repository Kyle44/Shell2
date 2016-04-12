// Name: Kyle Fritz
// File: shellFunctions.c
// Date Created: 2/20/16
// Description: Functionality for main shell

#include "builtinFuncs.c"

int execute(char** args){
	int numArgs = getNumArgs(args);
	if(args[numArgs - 1][0] == '\n'){
		args[numArgs - 1] = NULL;
	}
//	printArray(args);
    if(args[0] == NULL || args[0][0] == '#'){
		return 1;  // Nothing entered or a comment, so return to command prompt
    }
    char* builtinCommands[] = {"exit", "chdir", "cd", "getenv", "setenv", "echo"}; // all of the possible commands
    int (*builtinFunctions[]) (char **) = {&exitFunc, &chdirFunc, &cdFunc, &getenvFunc, &setenvFunc, &echoFunc}; // array of functions
    int numCommands = sizeof(builtinCommands) / sizeof(char*); // how many commands
    int i = 0;
    while(i < numCommands) {

	    if (strcmp(builtinCommands[i], args[0]) == 0) {
//	    	printf("true\n");
  			return (*builtinFunctions[i])(args);
		}
		else{
//			printf("false\n");
		}
		i++;
	}

    return launch(args);
}

int launch(char** args){
//	printArray(args);
//	printNumArgs(args);
	pid_t pid, wpid;
	int status;

	pid = fork();
	if(pid == 0){   // Child Process
		execvp(args[0], args);
		// If execvp returns, it failed	
		perror("Child Process Error");
		exit(EXIT_FAILURE);
	}
	
	else if(pid<0){
		perror("Error forking");
	}

	else{  // Parent Process
		do{
			wpid = waitpid(pid, &status, WUNTRACED);  // Wait for process's status to change
		} while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}	



char** splitLine(char* line){
	int currentTokenBufferSize = TOKEN_BUFFER_SIZE;
	int position = 0;
	char** tokens = malloc(currentTokenBufferSize * sizeof(char*));  // Allocate for each token
	char* token;
	char* endOfToken; // after function, this is the second argument in the token
	int endPosition; // using first_unquoted_space 
	char firstArg[256];

	if(!tokens){
		fprintf(stderr, "Token Allocation Error\n");
		exit(EXIT_FAILURE);
	}

	token = line;
	//printString(token);
	endPosition = first_unquoted_space(line); // first argument end position.  Can also be -1
	if(endPosition > 256){ // Argument too big to be valid arg
		fprintf(stderr, "Invalid Argument\n");
		exit(EXIT_FAILURE);
	}

	if(endPosition != -1 && token[endPosition + 1] != '\0'){ // if at least 2 args
		memset(firstArg, '\0', sizeof(firstArg)); // nullify firstArg
		strncpy(firstArg, token, endPosition); // copy first argument to firstArg, (dest, src+index, endIndex - beginIndex)
		token = unescape(firstArg, stderr);

		tokens[position] = token;
		position++;

		if(position >= currentTokenBufferSize){
			currentTokenBufferSize += TOKEN_BUFFER_SIZE;
			tokens = realloc(tokens, currentTokenBufferSize * sizeof(char*));
			if(!tokens){
				fprintf(stderr, "Token Reallocation Error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = line;
		
		strcpy(token, &token[endPosition + 1]); // make token the second argument now, endPosition + 1 -> after first space
		token = unescape(token, stderr);
		tokens[position] = token;
		position++;


		if(position >= currentTokenBufferSize){
			currentTokenBufferSize += TOKEN_BUFFER_SIZE;
			tokens = realloc(tokens, currentTokenBufferSize * sizeof(char*));
			if(!tokens){
				fprintf(stderr, "Token Reallocation Error\n");
				exit(EXIT_FAILURE);
			}
		}

		tokens[position] = NULL;
//		printf("tokens = %s, %s\n", tokens[0], tokens[1]);
		return tokens;
	}

	else{ // 1 arg
		token = unescape(token, stderr);
		tokens[position] = token;
		position++;

		if(position >= currentTokenBufferSize){
			currentTokenBufferSize += TOKEN_BUFFER_SIZE;
			tokens = realloc(tokens, currentTokenBufferSize * sizeof(char*));
			if(!tokens){
				fprintf(stderr, "Token Reallocation Error\n");
				exit(EXIT_FAILURE);
			}
		}

		tokens[position] = NULL;
//		printf("tokens = %s\n", tokens[0]);
		return tokens;
	}
}

char* readLine(){
	int currentBufferSize = BUFFER_SIZE;
	int position = 0;
	char* buffer = malloc(sizeof(char) * currentBufferSize);
	int c;

	if(!buffer){
		fprintf(stderr, "Allocation error\n");
		exit(EXIT_FAILURE);
	}

	while(1){
		c = getchar(); // Read a character

		if(c == EOF || c == '\n'){
			buffer[position] = '\0';
			return buffer;
		}
 
		else{
			buffer[position] = c;
		}
		position++;

		if(position >= currentBufferSize){
			currentBufferSize += BUFFER_SIZE;
			buffer = realloc(buffer, currentBufferSize);
			if(!buffer){
				fprintf(stderr, "Reallocation Error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}


char* readLineScript(FILE* file){
	int currentBufferSize = BUFFER_SIZE;
	char* buffer = malloc(sizeof(char) * currentBufferSize);

	if(fgets(buffer, currentBufferSize, file) != NULL) { // while still lines in file
		if(sizeof(buffer) >= currentBufferSize){ // make sure buffer is big enough
			currentBufferSize += BUFFER_SIZE;
			buffer = realloc(buffer, currentBufferSize);
			if(!buffer){
				fprintf(stderr, "Reallocation Error\n");
				exit(EXIT_FAILURE);
			}
		}
		unescape(buffer, stderr);
//		buffer[strlen(buffer) - 1] = '\0';
		printf("%s", buffer); // print the line
		return buffer;
	}

	else{
		exit(0);
	}
}
