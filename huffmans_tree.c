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

void print (P_queue *P) {//apenas para debug
   if(!P) return;
   int i;
   for (i = 0; i < P->size; i++) {
      printf("%c:%d  ", P->V[i]->id, P->V[i]->freq);
   }
   printf("\n");
}

void print_node (Node* node) {//apenas para debug
   if(node){
      if (!node->left && !node->right)
         printf("1%c", node->id);
      else
         printf("0");
      print_node(node->left);
      print_node(node->right);
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

void insert_char (P_queue *P, char id, int freq) {
   Node *new_node = (Node*)malloc(sizeof(Node));
   new_node->id = id;
   new_node->freq = freq;
   insert(P, new_node);
}

int count_chars (FILE *f, P_queue *P) {
   // Read the file into a memory buffer
   fseek(f, 0L, SEEK_END);
   int fsize = ftell(f);
   fseek(f, 0L, SEEK_SET);
   unsigned char *buffer = malloc(fsize);
   fread(buffer, fsize, 1, f);
   printf("%d\n", fsize);
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
   return fsize;
}

Node *create_huff_tree (P_queue *P) {
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
      printf("%c:", root->id);
      printf("%s", store);
      table[root->id] = strdup(store);
      printf(" ");
   }
}

/*Global Variables to print bits*/
int currbyte = 0, bitcount = 0;

void write_bit (FILE *f, int bit) {
   currbyte = currbyte << 1;
   if(bit == 1) currbyte |= 1;
   bitcount++;
   if (bitcount == BITS_PER_BYTE){
      printf("%d\n", currbyte);
      fputc(currbyte, f);
      currbyte = 0;
      bitcount = 0;
   }
}

void write_char (FILE *f, char c) {
   for(int i = BITS_PER_BYTE - 1; i >= 0; i--){
      write_bit(f, (c & (1 << i)) >> i);
   }
}

int read_bit (FILE *f) {
   if (bitcount == 0){
      currbyte = fgetc(f);
      bitcount = BITS_PER_BYTE;
   }
   bitcount--;
   return (currbyte & (1 << bitcount)) >> bitcount;
}

char read_char (FILE *f) {
   char c = 0;
   for(int i = 0; i < BITS_PER_BYTE; i++){
      c += read_bit(f);
   }
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

Node *read_codes (FILE *f) {
   if (read_bit(f)){
      char c = read_char(f);
      Node *aux = (Node*)malloc(sizeof(Node));
      aux->id = c;
      aux->freq = 0;
      aux->left = aux->right = NULL;
      return aux;
   }
   Node *aux = (Node*)malloc(sizeof(Node));
   aux->id = '\0';
   aux->freq = 0;
   aux->left = read_codes(f);
   aux->right = read_codes(f);
   return aux;
}

void encode (FILE *input, FILE *output) {
   char store[ALPHABET];
   char *table[ALPHABET];

   P_queue P;
   new_pqueue(&P);
   int n_chars = count_chars(input, &P);
   // print(&P);
   Node *node = create_huff_tree(&P);
   freq_table(node, store, table, 0);

   print_node(node);
   printf("\n");

   write_codes(output, node);

   // write_bit(output, n_chars);

   fseek(input, 0L, SEEK_END);
   fseek(input, 0L, SEEK_SET);
   unsigned char *buffer = malloc(n_chars);
   fread(buffer, n_chars, 1, input);

   for(int i = 0; i < n_chars; i++){
      for(int j = 0; j < strlen(table[buffer[i]]); j++){
         write_bit(output, ((table[buffer[i]][j] == '1') ? 1:0));
      }
   }
}

void decode (FILE *input, FILE *output) {
   // char store[ALPHABET];
   // char *table[ALPHABET];

   fseek(input, 0L, SEEK_END);
   int fsize = ftell(input);
   fseek(input, 0L, SEEK_SET);
   unsigned char *buffer = malloc(fsize);
   fread(buffer, fsize, 1, input);

   // Node *node = read_codes(input);
   // freq_table(node, store, table, 0);

   // print_node(node);

   // int ch = read_bit(input);
   // Node *aux = node;
   // char* text = "";
   // printf("%s\n", table['e']);
   // while(!feof(input)){
   //    if(!aux) break;
   //    if(ch == 0)
   //       aux = aux->left;
   //    else
   //       aux = aux->right;

   //    if (!aux->left && !aux->right){
   //       char c = aux->id;
   //       strcat(text, &(c));
   //       //fwrite(text, strlen(text), 1, output);
   //       printf("%s\n", text);
   //       aux = node;
   //    }
   //    ch = read_bit(input);
   // }

   printf("%d\n", fgetc(input));
   printf("%d\n", read_bit(input));
   printf("%d\n", read_bit(input));
   printf("%d\n", read_bit(input));
   printf("%d\n", read_bit(input));
   printf("%d\n", read_bit(input));
   printf("%d\n", read_bit(input));
   printf("%d\n", read_bit(input));
   printf("%d\n", read_bit(input));
   printf("%d\n", read_bit(input));
   printf("%d\n", read_bit(input));
   printf("%d\n", read_bit(input));
   printf("%d\n", read_bit(input));
   printf("%d\n", read_bit(input));
}

