#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>

//BERK ENGÝN 150117014

typedef struct PrQNode{
	struct PrQNode *sibling;
	struct PrQNode *parent;
	struct PrQNode *child;
	char file_name[40];
	int degree;
	int priority;
}PrQNode;

typedef struct PrQueue {
	PrQNode *front;
}PrQueue;

PrQueue *create_PrQueue(){
	PrQueue *heap = malloc(sizeof(PrQueue));
	heap->front = NULL;
	return heap;
}

PrQNode *create_PrQNode(char *file_name,int priority){
	PrQNode *node = malloc(sizeof(PrQNode));
	node->sibling = NULL;
	node->child = NULL;
	node->parent = NULL;
	node->priority = priority;
	strcpy(node->file_name,file_name);
	node->degree = 0;
	return node;
}

PrQNode *merge_BH(PrQueue *heap_A, PrQueue *heap_B){
	if(heap_A->front == NULL) 
		return heap_B->front;
	if(heap_B->front == NULL) 
		return heap_A->front;

	PrQNode *front;
	PrQNode *heap_A_iter = heap_A->front;
	PrQNode *heap_B_iter = heap_B->front;
	PrQNode *rear;

	if(heap_A->front->degree <= heap_B->front->degree){
		front = heap_A->front;
		heap_A_iter = heap_A_iter->sibling;
	}
	else{
		front = heap_B->front;
		heap_B_iter = heap_B_iter->sibling;
	}
	rear = front;
	while(heap_A_iter != NULL && heap_B_iter != NULL){
		if(heap_A_iter->degree <= heap_B_iter->degree){
			rear->sibling = heap_A_iter;
			heap_A_iter = heap_A_iter->sibling;
		}
		else{
			rear->sibling = heap_B_iter;
			heap_B_iter = heap_B_iter->sibling;
		}

		rear = rear->sibling;
	}

	rear->sibling = (heap_A_iter != NULL) ? heap_A_iter : heap_B_iter;

	return front;
}

PrQNode *union_BH(PrQueue *actual, PrQueue *new_union){
	PrQNode *new_front = merge_BH(actual,new_union);
	actual->front = NULL;
	new_union->front = NULL;

	if(new_front == NULL)
		return NULL;
	PrQNode *prev = NULL;
	PrQNode *current = new_front;
	PrQNode *next = new_front->sibling;

	while(next != NULL){
		if(current->degree != next->degree || (next->sibling != NULL && next->sibling->degree == current->degree)){
			prev = current;
			current = next;
		}
		else{
			if(current->priority <= next->priority){
				current->sibling = next->sibling;
				next->parent = current;
				next->sibling = current->child;
				current->child = next;
				current->degree++;
			}
			else{
				if(prev == NULL)
					new_front = next;
				else
					prev->sibling = next;

				current->parent = next;
				current->sibling = next->child;
				next->child = current;
				next->degree++;

				current = next;
			}
		}

		next = current->sibling;
	}
	return new_front;
}

void insert_BH(PrQueue *heap, char *file_name, int priority){
	PrQNode *node = create_PrQNode(file_name,priority);
	PrQueue *temp = create_PrQueue();
	temp->front = node;
	//printf("	\n\n%s		priority 	%d\n\n",temp->front->file_name,temp->front->priority);
	heap->front = union_BH(heap,temp);
	free(temp);
}


void remove_BH(PrQueue *heap, PrQNode *front, PrQNode *prev){
	if(front == heap->front)
		heap->front = front->sibling;
	else
		prev->sibling = front->sibling;

	PrQNode *new_front = NULL;
	PrQNode *child = front->child;

	while(child != NULL){
		PrQNode *next = child->sibling;
		child->sibling = new_front;
		child->parent = NULL;
		new_front = child;
		child = next;
	}
	PrQueue *temp = create_PrQueue();
	temp->front = new_front;
	heap->front = union_BH(heap,temp);
	free(temp);
}


PrQNode *minHeap(PrQueue *heap){
	if(heap->front == NULL) 
		return NULL;

	PrQNode *min = heap->front;
	PrQNode *minBefore = NULL;
	PrQNode *next = min->sibling;
	PrQNode *nextBefore = min;

	while(next != NULL){
		if(next->priority < min->priority){
			min = next;
			minBefore = nextBefore;
		}
		nextBefore = next;
		next = next->sibling;
	}
	remove_BH(heap,min,minBefore);
	return min;
}

int compareWord(char word[], char input[]){
	int count = 0;
	if(strcmp(word,input) == 0){
		count++;
		return count;
	}
	else{
		return count;
	}
}

void readFiles(char keyword[], int relevance){
	DIR* dir;
    FILE *fp;
    char files[1000];
    int i;
    int j;
    struct dirent *folder;
    char word[50];
    int file_counter = 0;
    dir = opendir("files");

    if(dir == NULL) {
        printf("Directory is empty");
    }
    else{
    	PrQueue *heap = create_PrQueue();
        while((folder = readdir(dir)) != NULL) {
            if (!strcmp (folder->d_name, "."))
                continue;
            if (!strcmp (folder->d_name, ".."))
                continue;
            
			file_counter++;
            //printf("%s :\n", folder->d_name);
            char dir[50] = "files\\\\";
            strcat(dir, folder->d_name);

            fp = fopen(dir, "r");
            char * token;
            const char s[2] = ":";
            const char o[2] = " ";
            const char n[2] = ".";
            int count = 0;
            if (fp != NULL) {
                while(fscanf(fp, "%s", word) != EOF){
                	/*if(word[0] == keyword[0] && word[1] == keyword[1] && word[2] == keyword[2]){
                					printf("%s\n",word);
            					}*/
            					//printf("word: %s\n",word);
					for(i = 0; word[i] != '\0'; ++i){
        				while (!( (word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z') || word[i] == '\0' || word[i] == '\'' || word[i] >= '0' && word[i] <= '9' || word[i] == '$' || word[i] == '.' || word[i] == ':') ){
							for(j = i; word[j] != '\0'; ++j){
                				word[j] = word[j+1];
            				}
            				word[j] = '\0';
						}
						if(word[i] == ':' || word[i] == '.'){
							if(word[i] == ':'){
								token = strtok(word,s);
							}
							else{
								token = strtok(word,n);
							}
								if(token != NULL)
								count = count + compareWord(token,keyword);
								//printf("this is token: %s		this is word: %s\n",token,word);
								//printf("word: %s	count: %d\n", token,count);
							while(token != NULL){
								token = strtok(NULL,o);
								if(token != NULL)
								count = count + compareWord(token,keyword);
								//printf("this is token: %s		this is word: %s\n",token,word);
								//printf("word: %s	count: %d\n", token,count);
							}
							strcpy(word,"");
						}
    				}
                	
                	count = count + compareWord(word,keyword);       
				}
                //printf("\n\nscanned file %s count -> %d\n\n",folder->d_name,count);
                //printf("file: %d\n",count);
                fclose(fp);
                if(folder->d_name != NULL){
                	insert_BH(heap,folder->d_name,count);
				}
            }
            
        }
        
        int new_relevance = relevance_checker(relevance,file_counter);
		PrQNode *node;
		int x;
		int a = 0;
		char filenameArr[new_relevance][30];
		char priorityArr[new_relevance];
		printf("The relevance order is: ");
		for(x = 0; x <= file_counter; x++){
			while(x >= (file_counter-new_relevance) && x < file_counter){
				node = minHeap(heap);
				//printf("\n\nresult file  %s count -> %d\n\n",node->file_name,node->priority);
				printf("%s(%d), ",node->file_name,node->priority);
				strcpy(filenameArr[a],node->file_name);
				priorityArr[a] = node->priority;
				//printf("\n%d",priorityArr[a]);
				//printf("%s",filenameArr[a]);
				x++;
				a++;
			}
			node = minHeap(heap);
		}
		printf("\n");
		char ch;
		FILE *fptr;
		int y;
		for(y = 0; y<new_relevance; y++){
			char dir[50] = "files\\\\";
			printf("\n%s(%d): ",filenameArr[y],priorityArr[y]);
            strcat(dir, filenameArr[y]);
        	
			fptr = fopen(dir,"r");
			if(fptr == NULL){
				perror("Error while opening the file.\n");
			}
			while((ch = fgetc(fptr)) != EOF){
				printf("%c", ch);
			}
			fclose(fptr);
		}
    }

    closedir(dir);
}

int relevance_checker(int relevance,int file_counter){
	while(relevance > file_counter){
		printf("Relevance number can not be greater than the file number\n");
		printf("Please enter the number of relevant documents again: ");
		scanf("%d",&relevance);
	}
	return relevance;
}

int main(){
	char keyword[20];
	int relevance;
	printf("Please enter a keyword: ");
	scanf("%s",&keyword);
	printf("Please enter the number of relevant documents: ");
	scanf("%d",&relevance);
	
    readFiles(keyword,relevance);

    return 0;
}

