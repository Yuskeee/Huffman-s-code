#include "huffmans_tree.h"

int main(int argc, char const *argv[]){
	char file_name[128];
	if (argc != 2){
		printf("Erro: insira no formato '%s %s'\n", argv[1], "arquivo.huff"); //error
		exit(1);
	}
	strcpy(file_name, argv[1]);

	FILE *input = fopen(file_name, "r"); //opens the 'huff' file to be decompressed
	if (!input){
      printf("Erro: nao foi possivel abrir %s\n", file_name); //error
      exit(1);
  	}

  	system("mkdir results");
  	char file_name_original[] = "results/";
  	strncat(file_name_original, file_name, strlen(file_name) - 5); //removes the 'huff' extension
	FILE *output = fopen(file_name_original, "w"); //creates/opens the output file
	if (!output){
      printf("Erro: nao foi possivel criar %s\n", file_name_original); //error
      exit(1);
  	}

  	decode(input, output);

  	fclose(input);
  	fclose(output);

  	return 0;
}	