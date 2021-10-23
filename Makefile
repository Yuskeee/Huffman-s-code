all:
	gcc -o hcompress compress.c huffmans_tree.c
	gcc -o hdecompress decompress.c huffmans_tree.c

clean:
	rm hcompress hdecompress
