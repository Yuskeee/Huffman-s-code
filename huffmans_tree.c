#include "huffmans_tree.h"

void new_pqueue(P_queue *P){
   P->size = 0;
   for (int i = 0; i < ALPHABET; ++i){
      P->V[i] = (Node*)malloc(sizeof(Node));
   }
}

int parent (int i) {
   return (i - 1)/2; 
}

void swap (P_queue *P, int a, int b) {
   if(!P) return;
   Node *aux = P->V[a];
   P->V[a] = P->V[b];
   P->V[b] = aux;
}

void print (P_queue *P) {//apenas para debug
   if(!P) return;
   int i;
   for (i = 0; i < P->size; i++) {
      printf("%d ", P->V[i]->freq);
   }
   printf("\n");
}

void min_heapify (P_queue *P, int i) {
   if(!P) return;
   int left = 2*i + 1, right = left + 1;
   int smallest = i;
   if(left < P->size && P->V[left]->freq < P->V[smallest]->freq)
      smallest = left;
   if(right < P->size && P->V[right]->freq < P->V[smallest]->freq)
      smallest = right;
   if(smallest != i){
      swap(P, smallest, i);
      min_heapify(P, smallest);
   }
}

/*void build_min_heap (P_queue *P) {
   if(!P) return;
   for(int i = P->size/2; i >= 0; i--){
      min_heapify(V, P->size, i);
   }
}*/

void insert (P_queue *P, Node *element) {
   if(!P || !element) return;

   int aux = P->size;
   while(aux > 0 && P->V[parent(aux)]->freq > element->freq){
      P->V[aux] = P->V[parent(aux)];
      aux = parent(aux);
   }
   P->V[aux] = element;

   P->size += 1;
}

Node *extract_min (P_queue *P) {
   if(!P) return NULL;
   if(P->size == 0){
      printf("Erro: Fila está vazia!");
      exit(1);
   }
   Node *aux = P->V[0];
   P->V[0] = P->V[P->size - 1];
   P->size -= 1;
   min_heapify(P, 0);

   return aux;
}

void count_chars(char *file_name, P_queue *P){
   FILE *f = fopen(file_name, "r");
   if (f == NULL)
   {
      printf("Error: couldn't open %s\n", file_name);
      exit(1);
   }    
   // Read the file into a memory buffer
   fseek(f, 0L, SEEK_END);
   int fsize = ftell(f);
   fseek(f, 0L, SEEK_SET);
   unsigned char *buffer = malloc(fsize);
   fread(buffer, fsize, 1, f);
   fclose(f);
   for(int i = 0; i < fsize; i++){
      printf("%c", buffer[i]);
   }
   unsigned char *aux = calloc(ALPHABET, sizeof(char));
   for(int i = 0; i < fsize; i++)
   {
      aux[buffer[i]]++;
      P->V[buffer[i]]->freq = aux[buffer[i]];
   }
   for(int i = 0; i < 256; i++){
      printf("%c: %d\n", i, aux[i]);
   }
   return;
}