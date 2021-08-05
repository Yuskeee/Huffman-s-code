#ifndef HUFFMANS_TREE_H
#define HUFFMANS_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET 256 //Alphabet ASCII
#define BITS_PER_BYTE 8

/*Main structure of the algorithm*/
typedef struct huffman_node{
	char id; //data
	int freq; //priority key
	struct huffman_node *left, *right;
}Node;

/*Priority queue based on the node's freq(uency)*/
typedef struct priority_queue{
	int size; //current size of queue
	Node *V[ALPHABET]; //min-heap
}P_queue;

/*Data Structure*/
void new_pqueue(P_queue *P);//initializes the priority queue
int parent (int i); //returns the parent of a node
void swap (P_queue *P, int a, int b); //swaps the values of two nodes
void print_queue (P_queue *P); //only for debug
void print_node (Node* node); //only for debug
void erase_nodes (Node* node); //free memory from nodes, given the root node
void min_heapify (P_queue *P, int i); //fix the heap
void insert (P_queue *P, Node *element); //inserts an element
Node *extract_min (P_queue *P); //returns the smallest element and removes from the queue

/*Frequency of characters and codes generator*/
void count_chars (char *buffer, int fsize, P_queue *P); //fill frequencies of huffman node
Node *create_huff_tree(P_queue *P); //create huffman tree
void freq_table (Node *root, char store[], char *table[],int top); //fill frequency table

/*Read/Write operations with bits*/
void write_bit (FILE *f, int bit); //write bit/byte to file
void write_char (FILE *f, char c); //write char (1 byte) to file
void write_int (FILE *f, int num); //write int (4 bytes) to file
int read_bit (char *buffer); //read bit/byte from file
char read_char (char *buffer); //read char (1 byte) from file
int read_int (char *buffer); //read int (4 bytes) from file

/*Read/Write the algorithm with files*/
void write_codes (FILE *f, Node *root); //write the codes to file
Node *read_codes (char *buffer); //read the codes
void encode (FILE *input, FILE *output); //encode
void decode (FILE *input, FILE *output); //decode

#endif