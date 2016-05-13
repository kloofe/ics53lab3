//Cheng, Gordon: 75368389
//Nina Volkmuth: 19518656
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setHeader(char* heap, int size, int alloc);
int getSize(char* heap);
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
	/*
	// First block's header, partitioned:
	int size = 127; //max user size
	int i;
	for(i = 129; i < 400; i+= 129) {
		setHeader(ptr, size, 0);
		ptr += 129;
	}
	setHeader(ptr, (400 - i - 129), 0);
	*/
	setHeader(ptr, 400, 0);
	int count = 1; // keep track of largest id assigned
	int i = 1;
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
  return (int)*c;
}

char itoc(int n){ //int to char
  return (n + '\0');
}

int getSize(char* heap) {
	int first = ctoi(heap + 1);
	if(first < 0) {
		first = 128 + 128 + first;
	}
	int second = ctoi((heap + 2));
	if(second < 0) {
		second = 128 + 128 + second;
	}
	return first + second;
}

void setHeader(char* heap, int size, int alloc) {
	*heap = itoc(alloc);
	heap++;
	
	if(size <= 255)  {
		*heap = itoc(size);
		*(heap + 1) = itoc(0);
	}
	else {
		int overflow = size - 255;
		*heap = itoc(255); 
		*(heap + 1) = itoc(overflow);
	}
	//printf("%d\n", ctoi(heap));
}

void allocate(char* heap, int size, int *count) {
	char* start = heap; // start is equal heap ptr
		while(heap < start + 400) {  //while iterating heap ptr is less than 400 bytes
		char* temp = heap;
		int temp2 = getSize(heap);
		if(ctoi(heap) == 0) { //if allocate/blockid header is unallocated
			if(getSize(heap) >= size) { //check size header, if greater than user block size
				*heap = itoc(*count); //change blockid to count
				printf("%d\n", *count);
				(*count)++; //increment for next block id
				/*
			        case 1: if user size is less than header block size + header size
          			case 2: if user size is equal to header block size
         			case 3: if user size is 125 or 126?
				*/
				if(getSize(heap) > size) { //case 1
          				int old = getSize(heap);  //get old size header
					setHeader(heap, size, (*count) - 1); // change size header to user size
          				int newSize = old - size; //newsize is old - header size - user size
					// create new block:
					heap += size + 3;
					setHeader(heap, newSize, 0);
				} //else case 2
				break;
			}
		}
		heap = temp;
		heap += temp2 + 3;
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
		heap += getSize(heap) + 3;
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
			heap += 3;
			int i;
			for(i = 0; i < n; i++) {
				*(heap) = *letter;
				heap++;
			}
			found = 1;
			break;
		}
		heap += getSize(heap) + 3;
	}
	if(found == 0) {
		printf("ID not found\n");
	}
}

void blockList(char* heap) {
	char* start = heap;
	printf("Size\tAlloc\tStart\t\tEnd\n");
	while(heap < start + 400) {
		int size = getSize(heap);
		printf("%d\t", size);
		if(ctoi(heap) == 0) {
			printf("No\t");
		}
		else {
			printf("Yes\t");
		}
		printf("0x%p\t0x%p\n", heap, heap + size + 2);
		heap += size + 3;
	}
}

void printHeap(char* heap, int id, int n) {
	char* start = heap;
	int found = 0;
	while(heap < start + 400) {
		if(ctoi(heap) == id) {
			heap += 3;
			int i;
			for(i = 0; i < n; i++) {
				printf("%c", *(heap));
				heap++;
			}
			printf("\n");
			found = 1;
			break;
		}
		heap += getSize(heap) + 3;
	}
	if(found == 0) {
		printf("ID not found\n");
	}
}
