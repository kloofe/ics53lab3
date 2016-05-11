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
				*heap = count + '\0';
				printf("%d\n", count);
				count++;
				if(*(heap + 1)- '\0' > size) {
					int tempInt = 0;
					int old = *(heap + 1) - '\0';
					*(heap + 1) = size + '\0';
					int newSize = old - 2 - size;
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

void freeBlock(char* heap, int id) {
	char* start = heap;
	int found = 0;
	while(heap < start + 400) {
		if(*(heap) - '\0' == id) {
			int tempInt = 0;
			*(heap) = tempInt + '\0';
			found = 1;
			break;
		}
		heap += *(heap + 1) - '\0' + 2;
	}
	if(found == 0) {
		printf("ID not found\n");
	}
}

void writeHeap(char* heap, int id, char* letter, int n) {
	char* start = heap;
	int found = 0;
	while(heap < start + 400) {
		if(*(heap) - '\0' == id) {
			heap += 2;
			int i;
			for(i = 0; i < n; i++) {
				*(heap) = *letter;
				heap++;
			}
			found = 1;
			break;
		}
		heap += *(heap + 1) - '\0' + 2;
	}
	if(found == 0) {
		printf("ID not found\n");
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

void printHeap(char* heap, int id, int n) {
	char* start = heap;
	int found = 0;
	while(heap < start + 400) {
		if(*(heap) - '\0' == id) {
			heap += 2;
			int i;
			for(i = 0; i < n; i++) {
				printf("%c", *(heap));
				heap++;
			}
			printf("\n");
			found = 1;
			break;
		}
		heap += *(heap + 1) - '\0' + 2;
	}
	if(found == 0) {
		printf("ID not found\n");
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
		// allocate DONE
		allocate(heap, atoi(command[1]), count);
	}
	else if(strcmp(command[0], "free") == 0) {
		freeBlock(heap, atoi(command[1]));
		// free DONE
	}
	else if(strcmp(command[0], "blocklist") == 0) {
		blockList(heap);
		// blocklist DONE
	}
	else if(strcmp(command[0], "writeheap") == 0) {
		writeHeap(heap, atoi(command[1]), command[2], atoi(command[3]));
		// writeheap
	}
	else if(strcmp(command[0], "printheap") == 0) {
		printHeap(heap, atoi(command[1]), atoi(command[2]));
		// printheap
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
