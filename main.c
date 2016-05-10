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

void setHeader(char* heap, int size, int alloc) {
	*heap = alloc + '\0';
	heap++;
	*heap = size + '\0';
	printf("%d\n", (*heap) - '\0');
}


void parseline(char* com, char** arg);

void addBlock(char* heap, int size) {
	
}


void allocate(char* heap, int size, int count) {
	char* start = heap;
	while(heap < start + 400) {
		char* temp = heap;
		++temp;
		int temp2 = (*temp) - '\0';
		if(*heap - '\0' == 0) {
			heap++;
			if(*heap - '\0' >= size) {
				heap--;
				int tempInt = 1;
				*heap = tempInt + '\0';
				printf("%d\n", count);
				count++;
				if(*(heap + 1)- '\0' > size) {
					int old = *(heap + 1) - '\0';
					*(heap + 1) = size + '\0';
					int newSize = old - 2 - size;
					tempInt--;
					printf("%d\n", newSize);
					*(heap + 2 + size) =  tempInt + '\0';
					*(heap + 3 + size) = newSize + '\0';
				}
				break;
			}
		}
		--temp;
		heap = temp;
		heap += temp2 + 2;
	}
}

void blockList(char* heap) {
	char* start = heap;
	printf("Size\tAlloc\tStart\tEnd\n");
	while(heap < start + 400) {
		int size = *(heap + 1) - '\0';
		printf("%d\t", size);
		if(*(heap) - '\0' == 0) {
			printf("no\t");
		}
		else {
			printf("yes\t");
		}
		printf("%p\t%p\n", heap, heap + size + 1);
		heap += size + 2;
	}
}

void run(char* heap, int count) {
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
		allocate(heap, atoi(command[1]), count);
	}
	else if(strcmp(command[0], "free") == 0) {
		// free
	}
	else if(strcmp(command[0], "blocklist") == 0) {
		blockList(heap);
		// blocklist
	}
	else if(strcmp(command[0], "writeheap") == 0) {
		// writeheap
	}
}

int main()
{	
	char* heap = malloc(400);
	printf("%p\n", heap);
	char* ptr = heap;
	// First block's header:
	int size = 127;
	int i;
	for(i = 129; i < 400; i+= 129) {
		setHeader(ptr, size, 0);
		ptr += 129;
	}
	setHeader(ptr, (400 - i - 129), 0);
	
	int count = 1; // keep track of largest id assigned
	while(i) {
		run(heap, count);
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
