#ifndef HUFFMANS_TREE_H
#define HUFFMANS_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET 256 //Alphabet ASCII
#define BITS_PER_BYTE 8

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
void print_node (Node* node); //apenas para debug
void min_heapify (P_queue *P, int i); //fix the heap
// void build_min_heap (P_queue *P); //builds the min heap
void insert (P_queue *P, Node *element); //inserts an element
Node *extract_min (P_queue *P); //returns the smallest element and removes from the queue
int count_chars (FILE *f, P_queue *P); //fill frequencies of huffman node
Node *create_huff_tree(P_queue *P); //create huffman tree
void freq_table (Node *root, char store[], char *table[],int top); //fill frequency table
void write_bit (FILE *f, int bit); //write bit/byte to file
void write_char (FILE *f, char c); //write char to file
int read_bit (FILE *f); //read bit/byte from file
void write_codes (FILE *f, Node *root); //write the codes to file
Node *read_codes (FILE *f); // read the codes
void encode (FILE *input, FILE *output); //encode
void decode (FILE *input, FILE *output); //decode

#endif