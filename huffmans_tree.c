#include "huffmans_tree.h"

int parent (int i) {
   return (i - 1)/2; 
}

void swap (P_queue *P, int a, int b) {
   if(!P) return;
   int aux = P->V[a];
   P->V[a] = P->V[b];
   P->V[b] = aux;
}

void print (P_queue *P) {//apenas para debug
   if(!P) return;
   int i;
   for (i = 0; i < P->size; i++) {
      printf("%d ", P->V[i]);
   }
   printf("\n");
}

void max_heapify (P_queue *P, int i) {
   if(!P) return;
   int left = 2*i + 1, right = left + 1;
   int largest = i;
   if(left < P->size && V[left]->freq > V[largest]->freq)
      largest = left;
   if(right < P->size && V[right]->freq > V[largest]->freq)
      largest = right;
   if(largest != i){
      swap(V, largest, i);
      max_heapify(V, P->size, largest);
   }
}

void build_max_heap (P_queue *P) {
   if(!P) return;
   for(int i = P->size/2; i >= 0; i--){
      max_heapify(V, P->size, i);
   }
}

void insert (P_queue *P, Node *element){
   if(!P || !element) return;

   int aux = P->size;
   while(aux > 0 && P->V[parent(aux)]->freq > element->freq){
      P->V[aux] = P->V[parent(aux)];
      aux = parent(aux);
   }
   P->V[aux] = element;

   P->size += 1;
}