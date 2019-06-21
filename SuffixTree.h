// A C program to implement Ukkonen's Suffix Tree Construction
// And find all locations of a pattern in string
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_CHAR 256

typedef struct SuffixTreeNode {
	struct SuffixTreeNode *children[MAX_CHAR];

	struct SuffixTreeNode *suffixLink;

	int start;
	int *end;

	int suffixIndex;
}Node;

char text[8388700]; //Input string
Node *newNode(int start, int *end);
void extendSuffixTree(int pos);
int edgeLength(Node *n);
int walkDown(Node *currNode);
void extendSuffixTree(int pos);
void setSuffixIndexByDFS(Node *n, int labelHeight);
void freeSuffixTreeByPostOrder(Node *n);
void buildSuffixTree();
int traverseEdge(char *str, int idx, int start, int end);
int doTraversalToCountLeaf(Node *n);
int countLeaf(Node *n);
int doTraversal(Node *n, char* str, int idx);
int checkForSubString(char* str);
int count(char* T, char* P);
int* locate(char* T, char* P);
