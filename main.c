//Cheng, Gordon: 75368389
//Nina Volkmuth: 19518656
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>


typedef struct {
	char* pointer;
	int size;
	int id;
} Block;


void parseline(char* com, char** arg);

int getIndex(int size) {
	return (int) ceil(log2((double) size);
}

void allocate(int size, Block** freeList, Block* allocBlocks, int count, int allocSize) {
	int index = getIndex(size);
	printf("%d", index);
	while(index < 8) {
		for(int i = 0; i < 100; i++) {
			if(freeList[index][i].id == -1) {
				break;
			}
			else if(freeList[index][i].size <= size) {
				allocBlocks[allocSize].pointer = freeList[index][i].pointer;
				allocBlocks[allocSize].id = count;
				allocBlocks[allocSize].size = size;
				if(freeList[index][i].size < size) {
					int newSize = freeList[index][i].size - size;
					int newI = getIndex(newSize);
					for(int j = 0; j < 100; j++) {
						if(freeList[newI][j].id == 0) {
						}
					}
					
				}
				count++;
				freeList[index][i].id = 0;
				index = 100;
				break;
			}
		}
		index++;
		
	}

}

void blockList(Block** freeList, Block* allocBlocks);

void run(char* heap, Block** freeList, Block* allocBlocks, int count, int allocSize) {
	char *command[1024];

	char input[1024];

	printf("> ");
	fgets(input, sizeof input, stdin);
	if(strcmp(input, "\n") == 0) {
		return;
	}
	
	parseline(input, command);
	if(strcmp(command[0], "quit") == 0) {
		exit(0);
	}
	else if(strcmp(command[0], "allocate") == 0) {
		// allocate
		allocate(atoi(command[1]), freeList, allocBlocks, count, allocSize);
	}
	else if(strcmp(command[0], "free") == 0) {
		// free
	}
	else if(strcmp(command[0], "blocklist") == 0) {
		// blocklist
	}
	else if(strcmp(command[0], "writeheap") == 0) {
		// writeheap
	}
}

int main()
{	
	char* heap = malloc(sizeof(char)*256);
	Block freeList[8][100];
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 100; j++) {
			freeList[i][j].id = 0;
		}
	}
	Block allocBlocks[128];
	int allocSize = 0;
	int count = 1; // keep track of largest id assigned
	while(i) {
		run(heap, freeList, allocBlocks, count, allocSize);
	}
	free(heap);
	return 0;
}

void parseline(char* com, char** arg){
	int index = 0;
	char* tok;
        tok  = strtok(com, "\t  \n");
        arg[index] = tok;
        while((tok = strtok(NULL, "\t  \n")) != NULL) {
		index++;
		arg[index] = tok;
	}
	arg[index+1] = NULL;
	
}
