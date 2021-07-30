#ifndef HUFFMANS_TREE_H
#define HUFFMANS_TREE_H

#include <stdio.h>
#include <stdlib.h>

#define ALPHABET 256 //Alphabet ASCII

typedef struct huffman_node{
	char id;
	int freq;
	struct huffman_node *left, *right;

}Node;

/*Priority queue based on the node's freq(uency)*/
typedef struct priority_queue{
	int size;
	Node *V[ALPHABET];
}P_queue;
P_queue* new_pqueue(P_queue *P){P->size = 0}; //initializes the priority queue
int parent (int i); //returns the parent of a node
void swap (P_queue *P, int a, int b); //swaps the values of two nodes
void print (P_queue *P); //apenas para debug
void max_heapify (P_queue *P, int i); //fix the heap
void build_max_heap (P_queue *P); //builds the heap
void insert (P_queue *P, Node *element); //inserts an element


#endif