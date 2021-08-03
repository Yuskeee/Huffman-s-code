#ifndef HUFFMANS_TREE_H
#define HUFFMANS_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void new_pqueue(P_queue *P);//initializes the priority queue
int parent (int i); //returns the parent of a node
void swap (P_queue *P, int a, int b); //swaps the values of two nodes
void print (P_queue *P); //apenas para debug
void min_heapify (P_queue *P, int i); //fix the heap
// void build_min_heap (P_queue *P); //builds the min heap
void insert (P_queue *P, Node *element); //inserts an element
Node *extract_min (P_queue *P); //returns the smallest element and removes from the queue
void count_chars(char *file_name, P_queue *P); //fill frequencies of huffman node
Node *create_huff_tree(P_queue *P); //create huffman tree
void encode(Node *root, int store[], int top); //encode

#endif