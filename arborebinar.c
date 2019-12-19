#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node{
	int value;
	struct node *st;
	struct node *dr;
}Node;

Node* new_node(int value){
	Node *node = (Node*) malloc (sizeof(Node));
	node->value = value;
	node->st = NULL;
	node->dr = NULL;
	return node;
}

struct QNode{
	Node* key;
	struct QNode *next;
};

typedef struct{
	struct QNode *front, *rear;
	int size;
}Queue;

Queue* new_Queue(){
	Queue *q = (Queue*)malloc(sizeof(Queue));
	q->front = NULL;
	q->rear = NULL;
	q->size = 0;
	return q;
}

struct QNode* new_QNode(Node *node){
	struct QNode* temp = (struct QNode*) malloc(sizeof(struct QNode*));
	temp->key = node;
	temp->next = NULL;
	return temp;
}



void push(Queue *q, Node *node){
	struct QNode *temp = new_QNode(node);
	q->size += 1;
	if(q->rear == NULL){
		q->front = q->rear = temp;
		return;
	}
	q->rear->next = temp;
	q->rear = temp;
}

int size(Queue *q){
	return q->size;
}

int isEmpty(Queue *q){
	return q->size == 0;
}

struct QNode* pop(Queue *q){
	if(q->front == NULL){ return NULL; }

	struct QNode *temp = q->front;
	q->front = q->front->next;
	q->size -= 1;
	if(q->front == NULL)
		q->rear = NULL;
	
	return temp;
}

void adauga_valori(Node* node, int i, int n){
	if(2*i + 1 < n){
		node->st = new_node(2*i);
		node->dr = new_node(2*i + 1);

		adauga_valori(node->st, 2*i, n);
		adauga_valori(node->dr, 2*i + 1, n);
	}
}

Node* citeste_arbore(int n){
	int i = 1;
	Node *root = new_node(i);
	adauga_valori(root, i, n);
	return root;
}


void afisare_arbore(Node* root){
	if(root == NULL){ return; }

	Queue* queue = new_Queue();
	push(queue, root);
	while(!isEmpty(queue)){
		int node_count = size(queue);
		while(node_count > 0){
			struct QNode *node = pop(queue);
			//printf("%d ", node->key->value);
			if(node->key->st != NULL){
				push(queue, node->key->st);
			}
			if(node->key->dr != NULL){
				push(queue, node->key->dr);
			}
			free(node);
			node_count--;
		}
		//printf("\n");
	}
}

int main(int argc, char**argv){
	if(argc != 2){
		printf("Utilizare: %s nr\n", argv[0]);
		return 1;
	}

	int a;
	char *c;
	a = strtol(argv[1], &c, 10);
	if(strlen(argv[1])== 0 || *c != '\0'){
		printf("%s nu este valid.\n", argv[1]);
		return 1;
	}
	Node *root = citeste_arbore(a);
	afisare_arbore(root);
	printf("\n");
	
	
	return 0;
}
