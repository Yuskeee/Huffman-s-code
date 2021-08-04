#include "huffmans_tree.h"

int main(int argc, char const *argv[]){
	char file_name[128];
	if (argc != 2){
		printf("Erro: insira no formato '%s %s'\n", argv[1], "arquivo.txt"); //error
		exit(1);
	}
	strcpy(file_name, argv[1]);

	FILE *input = fopen(file_name, "r"); //opens the file to be compressed
	if (!input){
      printf("Erro: nao foi possivel abrir %s\n", file_name); //error
      exit(1);
  	}    

	strcat(file_name, ".huff");
	FILE *output = fopen(file_name, "w"); //creates/opens the output file
	if (!output){
      printf("Erro: nao foi possivel criar %s\n", file_name); //error
      exit(1);
  	}

  	encode(input, output);

  	fclose(input);
  	fclose(output);

	return 0;
}