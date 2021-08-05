#include "huffmans_tree.h"

void new_pqueue(P_queue *P){
   P->size = 0;
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

void print_queue (P_queue *P) { //only for debug
   if(!P) return;
   int i;
   for (i = 0; i < P->size; i++) {
      printf("%c:%d  ", P->V[i]->id, P->V[i]->freq);
   }
   printf("\n");
}

void print_node (Node* node) { //only for debug
   if(node){
      if (!node->left && !node->right)
         printf("1%c", node->id);
      else
         printf("0");
      print_node(node->left);
      print_node(node->right);
   } 
}

void erase_nodes (Node* node) {
   if(node){
      erase_nodes(node->left);
      erase_nodes(node->right);
      free(node);
   }
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

void insert_char (P_queue *P, char id, int freq) {
   Node *new_node = (Node*)malloc(sizeof(Node));
   new_node->id = id;
   new_node->freq = freq;
   insert(P, new_node);
}

void count_chars (char *buffer, int fsize, P_queue *P) {
   // for(int i = 0; i < fsize; i++){
   //    printf("%c", buffer[i]);
   // }
   int *aux = calloc(ALPHABET, sizeof(int));
   for(int i = 0; i < fsize; i++){
      aux[buffer[i]]++;
   }
   for (int i = 0; i < ALPHABET; ++i){
      if (aux[i] != 0){
         insert_char(P, i, aux[i]);
      }
   }
}

Node *create_huff_tree (P_queue *P) {
   if(!P) return NULL;
   while(P->size != 1){
      Node *left = extract_min(P), *right = extract_min(P);
      Node *top = (Node*)malloc(sizeof(Node));
      top->freq = left->freq + right->freq;
      top->left = left;
      top->right = right;
      insert(P, top);
   }
   return(extract_min(P));
}

void freq_table (Node *root, char store[], char *table[],int top) {
   if(!root) return;
   if (root->left){
      store[top] = '0';
      freq_table(root->left, store, table, top+1);
   }
   if (root->right){
      store[top] = '1';
      freq_table(root->right, store, table, top+1);
   }
   if (!root->left && !root->right){
      store[top] = '\0';
      table[root->id] = strdup(store);
   }
}

/*Global Variables to write/read bits*/
int currbyte = 0, bitcount = 0, pos = 0;

void write_bit (FILE *f, int bit) { //writes 8 bits onto a file when a byte is completed 
   currbyte = currbyte << 1;
   if(bit == 1) currbyte |= 1;
   bitcount++;
   if (bitcount == BITS_PER_BYTE){
      // printf("%d\n", currbyte);
      fputc(currbyte, f);
      currbyte = 0;
      bitcount = 0;
   }
}

void write_char (FILE *f, char c) { //reads 1 byte
   for(int i = BITS_PER_BYTE - 1; i >= 0; i--){
      write_bit(f, (c & (1 << i)) >> i);
   }
}

void write_int (FILE *f, int num) { //reads 4 bytes
   for(int i = BITS_PER_BYTE*sizeof(int) - 1; i >= 0; i--){
      write_bit(f, (num & (1 << i)) >> i);
   }
}

int read_bit (char *buffer) { //reads a byte when no bit is left and returns a bit
   if (bitcount == 0){
      currbyte = buffer[pos++];
      bitcount = BITS_PER_BYTE;
   }
   bitcount--;
   return ((currbyte & (1 << bitcount)) >> bitcount);
}

char read_char (char *buffer) { //reads 1 byte
   char c = 0;
   for(int i = 0; i < BITS_PER_BYTE; i++){
      c += read_bit(buffer) << (BITS_PER_BYTE - 1 - i);
   }
   return c;
}

int read_int (char *buffer) { //read 4 bytes
   unsigned int num = 0;
   for(int i = 0; i < BITS_PER_BYTE*sizeof(int); i++){
      num += read_bit(buffer) << (BITS_PER_BYTE*sizeof(int) - 1 - i);
   }
   return num;
}

void write_codes (FILE *f, Node *root) {
   if (root && !root->left && !root->right){
      write_bit(f, 1);
      write_char(f, root->id);
      return;
   }
   write_bit(f, 0);
   write_codes(f, root->left);
   write_codes(f, root->right);
}

Node *read_codes (char *buffer) {
   if (read_bit(buffer)){
      char c = read_char(buffer);
      Node *aux = (Node*)malloc(sizeof(Node));
      aux->id = c;
      aux->freq = 0;
      aux->left = aux->right = NULL;
      // printf("%d\n", c);
      return aux;
   }
   Node *aux = (Node*)malloc(sizeof(Node));
   aux->id = '\0';
   aux->freq = 0;
   aux->left = read_codes(buffer);
   aux->right = read_codes(buffer);
   return aux;
}

void encode (FILE *input, FILE *output) {
   char store[ALPHABET];
   char *table[ALPHABET];

   //Read the file into a memory buffer
   fseek(input, 0L, SEEK_END);
   int fsize = ftell(input);
   fseek(input, 0L, SEEK_SET);
   unsigned char *buffer = malloc(fsize);
   fread(buffer, fsize, 1, input);
   printf("%d\n", fsize);

   P_queue P;
   new_pqueue(&P);
   count_chars(buffer, fsize, &P);
   // print_queue(&P);
   Node *node = create_huff_tree(&P);
   freq_table(node, store, table, 0);

   // print_node(node);
   // printf("\n");

   write_codes(output, node);
   write_int(output, fsize); //write the total number of characters

   for(int i = 0; i < fsize; i++){
      for(int j = 0; j < strlen(table[buffer[i]]); j++){
         write_bit(output, ((table[buffer[i]][j] == '1') ? 1:0)); //write every bit onto the output file
         // printf("%c\n", table[buffer[i]][j]);
      }
   }

   while(pos < BITS_PER_BYTE){ //in case some bits do not sum up to a byte
      write_bit(output, 0); //fill the rest of the byte
      pos++;
   }

   free(buffer);
   erase_nodes(node);
}

void decode (FILE *input, FILE *output) {
   int counter = 0;

   //Read the file into a memory buffer
   fseek(input, 0L, SEEK_END);
   int fsize = ftell(input);
   fseek(input, 0L, SEEK_SET);
   unsigned char *buffer = malloc(fsize);
   fread(buffer, fsize, 1, input);

   Node *node = read_codes(buffer);
   int size = read_int(buffer); //reads the total number of characters
   printf("%d\n", size);
   // print_node(node);
   
   int ch = read_bit(buffer);
   Node *aux = node;
   while(counter < size){ //until all characters in the original file are written
      if(!aux) break;
      if(ch == 0)
         aux = aux->left;
      else
         aux = aux->right;

      if (!aux->left && !aux->right){
         char c = aux->id;
         fwrite(&c, 1, 1, output); //write the identified character
         aux = node;
         counter++; // +1 character
      }
      ch = read_bit(buffer);
   }

   free(buffer);
   erase_nodes(node);
}