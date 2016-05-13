//Cheng, Gordon: 75368389
//Nina Volkmuth: 19518656
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setHeader(char* heap, int size, int alloc);
void run(char* heap, int *count);
void parseline(char* com, char** arg);
void allocate(char* heap, int size, int *count);
void freeBlock(char* heap, int id);
void writeHeap(char* heap, int id, char* letter, int n);
void blockList(char* heap);
void printHeap(char* heap, int id, int n);

int main()
{
	char* heap = malloc(400);
	char* ptr = heap;
	// First block's header, partitioned:
	int size = 127; //max user size
	int i;
	for(i = 129; i < 400; i+= 129) {
		setHeader(ptr, size, 0);
		ptr += 129;
	}
	setHeader(ptr, (400 - i - 129), 0);

	int count = 1; // keep track of largest id assigned
	while(i) {
		run(heap, &count); //pass count as program executes
	}
	free(heap);
	return 0;
}

void run(char* heap, int *count) {
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
		allocate(heap, atoi(command[1]), &(*count));
	}
	else if(strcmp(command[0], "free") == 0) {
		freeBlock(heap, atoi(command[1]));
	}
	else if(strcmp(command[0], "blocklist") == 0) {
		blockList(heap);
	}
	else if(strcmp(command[0], "writeheap") == 0) {
		writeHeap(heap, atoi(command[1]), command[2], atoi(command[3]));
	}
	else if(strcmp(command[0], "printheap") == 0) {
		printHeap(heap, atoi(command[1]), atoi(command[2]));
	}
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

int ctoi(char* c){ //char* to int
  return ((int)*c - '\0');
}

char itoc(int n){ //int to char
  return (n + '\0');
}

void setHeader(char* heap, int size, int alloc) {
	*heap = itoc(alloc);
	heap++;
	*heap = itoc(size);
	//printf("%d\n", ctoi(heap));
}

void allocate(char* heap, int size, int *count) {
	char* start = heap; // start is equal heap ptr
	while(heap < start + 400) {  //while iterating heap ptr is less than 400 bytes
		char* temp = heap;
		++temp;
		int temp2 = ctoi(temp);
		if(ctoi(heap) == 0) { //if allocate/blockid header is unallocated
			if(ctoi(heap + 1) >= size) { //check size header, if greater than user block size
				*heap = itoc(*count); //change blockid to count
				printf("%d\n", *count);
				(*count)++; //increment for next block id
				/*
			        case 1: if user size is less than header block size + header size
          			case 2: if user size is equal to header block size
         			case 3: if user size is 125 or 126?
				*/
				if(ctoi(heap + 1) > size) { //case 1
          				int old = ctoi(heap + 1);  //get old size header
          				*(heap + 1) = itoc(size);  //change size header to user size
          				int newSize = old - 2 - size; //newsize is old - header size - user size
          				*(heap + 2 + size) =  itoc(0); //set new header alloc/blockid
          				*(heap + 3 + size) = itoc(newSize); //new header size
				} //else case 2
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
		if(ctoi(heap) == id) {
			int tempInt = 0;
			*heap = itoc(tempInt);
			found = 1;
			break;
		}
		heap += ctoi(heap + 1) + 2;
	}
	if(found == 0) {
		printf("ID not found\n");
	}
}

void writeHeap(char* heap, int id, char* letter, int n) {
	char* start = heap;
	int found = 0;
	while(heap < start + 400) {
		if(ctoi(heap) == id) {
			heap += 2;
			int i;
			for(i = 0; i < n; i++) {
				*(heap) = *letter;
				heap++;
			}
			found = 1;
			break;
		}
		heap += ctoi(heap + 1) + 2;
	}
	if(found == 0) {
		printf("ID not found\n");
	}
}

void blockList(char* heap) {
	char* start = heap;
	printf("Size\tAlloc\tStart\t\tEnd\n");
	while(heap < start + 400) {
		int size = ctoi(heap + 1);
		printf("%d\t", size);
		if(ctoi(heap) == 0) {
			printf("No\t");
		}
		else {
			printf("Yes\t");
		}
		printf("0x%p\t0x%p\n", heap, heap + size + 1);
		heap += size + 2;
	}
}

void printHeap(char* heap, int id, int n) {
	char* start = heap;
	int found = 0;
	while(heap < start + 400) {
		if(ctoi(heap) == id) {
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
		heap += ctoi(heap + 1) + 2;
	}
	if(found == 0) {
		printf("ID not found\n");
	}
}
