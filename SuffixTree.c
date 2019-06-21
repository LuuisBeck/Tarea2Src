#include "SuffixTree.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

char text[8388700]; //Input string
Node *root = NULL; //Pointer to root node
Node *lastNewNode = NULL;
Node *activeNode = NULL;

int activeEdge = -1;
int activeLength = 0;

int remainingSuffixCount = 0; //cuantos sufijos faltan
int leafEnd = -1;
int *rootEnd = NULL;
int *splitEnd = NULL;
int size = -1; //Largo string

int num_pos;
int position_list[100];
int index_list = 0;

Node *newNode(int start, int *end){
	Node *node =(Node*) malloc(sizeof(Node));
	int i;
	for (i = 0; i < MAX_CHAR; i++)
		node->children[i] = NULL;

	node->suffixLink = root;
	node->start = start;
	node->end = end;

	node->suffixIndex = -1;
	return node;
}

int edgeLength(Node *n) {
	if(n == root)
		return 0;
	return *(n->end) - (n->start) + 1;
}

int walkDown(Node *currNode){
	if (activeLength >= edgeLength(currNode)){
		activeEdge += edgeLength(currNode);
		activeLength -= edgeLength(currNode);
		activeNode = currNode;
		return 1;
	}
	return 0;
}

void extendSuffixTree(int pos){
	leafEnd = pos;

	remainingSuffixCount++;

	lastNewNode = NULL;

	while(remainingSuffixCount > 0) {

		if (activeLength == 0)
			activeEdge = pos;


		if (activeNode->children[text[activeEdge]] == NULL){
			activeNode->children[text[activeEdge]] =
										newNode(pos, &leafEnd);

			if (lastNewNode != NULL){
				lastNewNode->suffixLink = activeNode;
				lastNewNode = NULL;
			}
		}
		else{
			Node *next = activeNode->children[text[activeEdge]];
			if (walkDown(next)){
				continue;
			}
			if (text[next->start + activeLength] == text[pos]){
				if(lastNewNode != NULL && activeNode != root){
					lastNewNode->suffixLink = activeNode;
					lastNewNode = NULL;
				}

				activeLength++;
				break;
			}

			splitEnd = (int*) malloc(sizeof(int));
			*splitEnd = next->start + activeLength - 1;

			Node *split = newNode(next->start, splitEnd);
			activeNode->children[text[activeEdge]] = split;

			split->children[text[pos]] = newNode(pos, &leafEnd);
			next->start += activeLength;
			split->children[text[next->start]] = next;


			if (lastNewNode != NULL)
			{

				lastNewNode->suffixLink = split;
			}

			lastNewNode = split;
		}

		remainingSuffixCount--;
		if (activeNode == root && activeLength > 0){
			activeLength--;
			activeEdge = pos - remainingSuffixCount + 1;
		}
		else if (activeNode != root){
			activeNode = activeNode->suffixLink;
		}
	}
}

void print(int i, int j){
	int k;
	for (k=i; k<=j; k++)
		printf("%c", text[k]);
}

void setSuffixIndexByDFS(Node *n, int labelHeight){
	if (n == NULL) return;

	int leaf = 1;
	int i;
	for (i = 0; i < MAX_CHAR; i++)
	{
		if (n->children[i] != NULL){
			leaf = 0;
			setSuffixIndexByDFS(n->children[i], labelHeight +	edgeLength(n->children[i]));
		}
	}
	if (leaf == 1){
		n->suffixIndex = size - labelHeight;
	}
}

void freeSuffixTreeByPostOrder(Node *n){
	if (n == NULL)
		return;
	int i;
	for (i = 0; i < MAX_CHAR; i++){
		if (n->children[i] != NULL){
			freeSuffixTreeByPostOrder(n->children[i]);
		}
	}
	if (n->suffixIndex == -1)
		free(n->end);
	free(n);
	for (int i = 0; i < index_list; i++) {
		position_list[i] = 0;
	}
	index_list = 0;
}

void buildSuffixTree()
{
	size = strlen(text);
	int i;
	rootEnd = (int*) malloc(sizeof(int));
	*rootEnd = - 1;

	root = newNode(-1, rootEnd);

	activeNode = root;
	for (i=0; i<size; i++)
		extendSuffixTree(i);
	int labelHeight = 0;
	setSuffixIndexByDFS(root, labelHeight);
}

int traverseEdge(char *str, int idx, int start, int end){
	int k = 0;

	for(k=start; k<=end && str[idx] != '\0'; k++, idx++){
		if(text[k] != str[idx])
			return -1;
	}
	if(str[idx] == '\0')
		return 1;
	return 0;
}

int doTraversalToCountLeaf(Node *n){
	if(n == NULL)
		return 0;
	if(n->suffixIndex > -1){
		position_list[index_list] = n->suffixIndex;
		index_list++;
		return 1;
	}
	int count = 0;
	int i = 0;
	for (i = 0; i < MAX_CHAR; i++)
	{
		if(n->children[i] != NULL)
		{
			count += doTraversalToCountLeaf(n->children[i]);
		}
	}
	return count;
}

int countLeaf(Node *n){
	if(n == NULL)
		return 0;

	return doTraversalToCountLeaf(n);

}

int doTraversal(Node *n, char* str, int idx){
	num_pos=0;
	if(n == NULL)
	{
		return -1;
	}
	int res = -1;

	if(n->start != -1){
		res = traverseEdge(str, idx, n->start, *(n->end));
		if(res == -1)
			return -1;
		if(res == 1){
			if(n->suffixIndex > -1){
				num_pos=1;
				position_list[index_list] = n->suffixIndex;
				index_list++;
			}
			else{
				num_pos=countLeaf(n);
			}
			return 1;
		}
	}

	idx = idx + edgeLength(n);

	if(n->children[str[idx]] != NULL)
		return doTraversal(n->children[str[idx]], str, idx);
	else
		return -1; // no match
}

int checkForSubString(char* str){
	int res = doTraversal(root, str, 0);
	if(res == 1){
		return 1;
	}
	else{
		return 0;
	}
}

int count(char* T, char* P){
	//freeSuffixTreeByPostOrder(root);
	strcpy(text, T);
	strcat(text,"$");
	checkForSubString(P);

	return num_pos;
}

int* locate(char* T, char* P) {
	//freeSuffixTreeByPostOrder(root);
	strcpy(text, T);
	strcat(text,"$");
	checkForSubString(P);

	return position_list;
}

void locateTest() {
	char* T = "holoholaholaho";
	char* P = "ho";
	int* array_results = locate(T,P);
	if (index_list != 0) {
		for (int i = 0; i < index_list; i++) {
			printf("POSICIONES: %d\n", array_results[i]);
		}
	}
}

void countTest(){
	char* T = "holoholaholaho";
	char* P = "z";
	printf("CONTEO: %d\n", count(T,P));
}

typedef struct key_value{
	int key;
	char* value;
}kv;

kv create_kv(int k, char* v){
	kv n;
	n.key=k;
	n.value = v;
	return n;
}

int notinArray(char** array, char* str, int ocupados){
	int i;
	for(i=0; i<ocupados; i++){

		if (array[i]==str){
			return 0;
		}
	}
	return 1;
}

char** getAllvalues(kv* array, int k){
	char** resultados;
	for(int i=0; i<k; i++){
		resultados[i]= array[i].value;
		printf("%s\n", resultados[i] );
	}
	return resultados;
}
/*
char** top_k_q(char* T, int k, int q){
	int fin_palabra=q;
	char** revisadas;
	kv resultado[k];
	int libre = 0;
	int num_revisadas = 0;

	char res_final[k][q+1];

	for (int i=0; i<strlen(T) && fin_palabra<strlen(T); i++){//

		fin_palabra = i + q;
		char substr[q]; //darle el valor de T[i,fin_palabra]
		strncpy(substr, T +i , q);
		substr[q]='\0';

		int apariciones;
		if(notinArray(revisadas, substr, num_revisadas)){ //si no esta ya en la lista de strings revisados
			apariciones = count(T,substr);
			if(libre<k){ //queda espacio, agrego al tiro
				resultado[libre].key= apariciones;
				resultado[libre].value = substr;
			}
			else{ //no queda espacio, hay que verificar
				for (int q=0; q<k; q++){
					if (resultado[q].key < apariciones){ //reemplazo
						resultado[q].key = apariciones;
						resultado[q].value = substr;
					}
				}

			}
			revisadas[libre]= substr;
			num_revisadas++;
			libre++;
		}
	}
	res_final[0]="ha";
	printf("jojo\n" );
	printf("j: %s\n",res_final[0]);

	for (int num=0; num<k; num++){
		printf("ENTRO!\n" );
		res_final[0]="hola";
		printf("jojo\n" );
		printf("j: %s\n",res_final[0]);

		res_final[num]= resultado[num].value;
		printf("AQ!\n");
	}
	printf("AQUIII!\n");
	return res_final;

}
*/

/*
// driver program to test above functions
int main(int argc, char *argv[])
{


	printf("----------------------\n");



	strcpy(text, "GEEKSFORGEEKS$");
	buildSuffixTree();
	printf("Text: GEEKSFORGEEKS, Pattern to search: GEEKS");
	checkForSubString("GEEKS");
	printf("\n\nText: GEEKSFORGEEKS, Pattern to search: GEEK1");
	checkForSubString("GEEK1");
	printf("\n\nText: GEEKSFORGEEKS, Pattern to search: FOR");
	checkForSubString("FOR");
	//Free the dynamically allocated memory
	freeSuffixTreeByPostOrder(root);

	strcpy(text, "AABAACAADAABAAABAA$");
	buildSuffixTree();
	printf("\n\nText: AABAACAADAABAAABAA, Pattern to search: AABA");
	checkForSubString("AABA");
	printf("\n\nText: AABAACAADAABAAABAA, Pattern to search: AA");
	checkForSubString("AA");
	printf("\n\nText: AABAACAADAABAAABAA, Pattern to search: AAE");
	checkForSubString("AAE");
	//Free the dynamically allocated memory
	freeSuffixTreeByPostOrder(root);

	strcpy(text, "AAAAAAAAA$");
	buildSuffixTree();
	printf("\n\nText: AAAAAAAAA, Pattern to search: AAAA");
	checkForSubString("AAAA");
	printf("\n\nText: AAAAAAAAA, Pattern to search: AA");
	checkForSubString("AA");
	printf("\n\nText: AAAAAAAAA, Pattern to search: A");
	checkForSubString("A");
	printf("\n\nText: AAAAAAAAA, Pattern to search: AB");
	checkForSubString("AB");
	//Free the dynamically allocated memory
	freeSuffixTreeByPostOrder(root);

	printf("-------------------------------\n" );


	char* T = "holoholaholaho";
	char* P = "ho";


	printf("HOLAAAA!\n");
	//top_k_q(T,3,2);
	return 0;
}
*/
