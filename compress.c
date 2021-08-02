#include "huffmans_tree.h"

int main(int argc, char const *argv[]){
	char *file_name;
	int store[];
	if (argc != 2){
		printf("Erro\n");//error
		exit(1);
	}
	strcpy(file_name, argv[1]);
	P_queue P;
	new_pqueue(&P);
	count_chars(file_name, &P);
	codify(create_huff_tree(&P), );
	print(&P);
	return 0;
}