#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#define MAXCHAR 100

//NAME : BERK ENGÝN		ID : 150117014

typedef struct node{				//Basic structure of node
	char word[MAXCHAR];
	int freq;
	int depthLevel;
	int accessTime;
	struct node * left;
	struct node * right;
}node;

node * insert(node * root, char word[MAXCHAR],int freq,int depthLevel){		//This function checks whether the tree is empty or not if not the check the current input with the node's value
	if(root == NULL){														//If it is greater than or equal the nodes's value then insert it to the right child of the current node,otherwise insert it to the left child
		node * root = (node*)malloc(sizeof(node));
		strcpy(root->word,word);
		root->freq = freq;
		root->depthLevel = depthLevel;
		root->accessTime = freq * depthLevel;
		root->right = NULL;
		root->left = NULL;
		return root;
	}
	else if(strcasecmp(root->word,word)<=0){
		printf("\n");
		depthLevel++;
		root->right = insert(root->right,word,freq,depthLevel);
		return root;
	}
	else{
		printf("\n");
		depthLevel++;
		root->left = insert(root->left,word,freq,depthLevel);
		return root;
	}	
}

node* newNode(char word[][MAXCHAR],int freq[], int depthLevel,int i){ 	//This function creates new nodes with the given parameters
    node * tree = (node*)malloc(sizeof(node));
    strcpy(tree->word,word[i]);
    tree->freq = freq[i];
	tree->depthLevel = depthLevel; 
	tree->accessTime = freq[i] * depthLevel;
    tree->left = NULL;
	tree->right = NULL; 
    return tree; 
} 

node * insertLevelOrderBT(char arr[][MAXCHAR], node* root, int i, int n,int freq[], int depthLevel) { 	//This function inserts the nodes in the binary tree as to minimize the total access time
    // Base case for recursion 
    if (i < n) 
    { 
    if(i<pow(2,depthLevel)-1){
	
        node* temp = newNode(arr,freq,depthLevel,i);
        root = temp; 
  
        // insert left child 
        root->left = insertLevelOrderBT(arr, root->left, 2 * i + 1, n,freq,depthLevel); 
  
        // insert right child 
        root->right = insertLevelOrderBT(arr, root->right, 2 * i + 2, n,freq,depthLevel); 
    }
    else{
    	depthLevel++;
    	node* temp = newNode(arr,freq,depthLevel,i);
        root = temp; 
  
        // insert left child 
        root->left = insertLevelOrderBT(arr, root->left, 2 * i + 1, n,freq,depthLevel); 
  
        // insert right child 
        root->right = insertLevelOrderBT(arr, root->right, 2 * i + 2, n,freq,depthLevel); 
	}
    
    } 
    return root; 
} 

node * readInputBT(FILE * fp, node * root){			//This function reads input from the input.txt file and copy the words and the frequencies then calls insertLevelOrder function with the given parameters
	char str[MAXCHAR];
	char word[MAXCHAR];
	int freq;
	int depthLevel = 1;
	int x = 0;
    char * filename = "input.txt";
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return;
    }
    int count = 0;
    
	while (fgets(str, MAXCHAR, fp) != NULL){
		count++;
	}
	fclose(fp);
	fp = fopen(filename, "r");
    int freqArr[count];
    char wordArr[count][MAXCHAR];
	while (fgets(str, MAXCHAR, fp) != NULL){
		char * term;
		term = strtok(str,",");
		term = strtok(NULL,",");
		strcpy(word,term);
		strcpy(wordArr[x],word);
		term = strtok(NULL,",");
		freq = atoi(term);
		freqArr[x] = freq;
		x++;
		//printf("%s	%d\n",word,freq);
	}
	/*int y,z = 0;
	printf("%d\n",count);
	while(y<count){
		printf("%s-> ",wordArr+y);
		y++;
	}
	printf("\n");
	while(z<count){
		printf("%d -> ",freqArr[z]);
		z++;
	}*/
	root = insertLevelOrderBT(wordArr, root, 0, count,freqArr, depthLevel);
    fclose(fp);
    printf("c)Construct a BT so as to minimize the total access time (L-N-R): \n\n");
	printf("\n\nd)Total Access Time in the BT %d\n",traverseBT(root));
}
   
node * readInput(FILE * fp, node * root){		//This function reads input from the input.txt file and copy the words and the frequencies then calls insert function with the given parameters
	char str[MAXCHAR];
	char word[MAXCHAR];
	int freq;
	int depthLevel = 1;
    char* filename = "input.txt";
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return;
    }
    while (fgets(str, MAXCHAR, fp) != NULL){
    	
		char * term;
		term = strtok(str,",");
		term = strtok(NULL,",");
		strcpy(word,term);
		term = strtok(NULL,",");
		freq = atoi(term);
		root = insert(root,word,freq,depthLevel);
	}
    fclose(fp);
    printf("a)Build a BST with the key Word (L-N-R):\n\n");
	printf("\n\nb)Total Access Time in the BST %d\n",traverse(root));
	printf("\n");
}



int traverse(node * tree){		//This function prints out the BST in left mid right notation
	static int totalAccessTime;
	if(tree==NULL)
		return;
	traverse(tree->left);
	printf("%s -> ",tree->word);
	totalAccessTime += tree->accessTime;
	traverse(tree->right);
	return totalAccessTime;
}

int traverseBT(node * tree){		//This function prints out the BT in left mid right notation
	static int totalAccessTime;
	if(tree==NULL)
		return;
	traverseBT(tree->left);
	printf("%s -> ",tree->word);
	totalAccessTime += tree->accessTime;
	traverseBT(tree->right);
	return totalAccessTime;
}

int main() {
	setlocale(LC_ALL,"TURKISH");
	
	node * root = (node*)malloc(sizeof(node));
	root = NULL;
	FILE *fp;
	readInput(fp,root);
	
	FILE *fptr;
	node * tree = (node*)malloc(sizeof(node));
	tree = NULL;
	readInputBT(fptr,tree);
	
	return 0;
}
