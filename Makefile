all: hcompress hdecompress

hcompress: compress.c huffmans_tree.c
	gcc -o hcompress compress.c huffmans_tree.c -std=gnu99

hdecompress: decompress.c huffmans_tree.c
	gcc -o hdecompress decompress.c huffmans_tree.c -std=gnu99

clean:
	rm hcompress hdecompress
