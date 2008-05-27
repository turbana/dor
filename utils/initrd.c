#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAGIC 0xDEED


/* mirrors definition in include/initrd.h */
struct initrd_file_header {
	unsigned short magic;
	char name[32];
	unsigned int offset;
	unsigned int length;
};


int
main(int argc, char **argv)
{
	int file_count, i;
	char *src, *dest, *buffer;
	struct initrd_file_header headers[32];
	unsigned int offset = sizeof(struct initrd_file_header) * 32 + sizeof(int);
	FILE *input, *output;

	if(argc < 4 || argc % 2 != 0) {
		printf("USAGE: %s initrd.bin source target[ source target]...\n",
			   argv[0]);
		return 1;
	}

	file_count = (argc - 2) / 2;

/*	printf("%d files found:\n", file_count);*/

	/* load headers */
	for(i = 0; i < file_count; i++) {
		src = argv[2 + i * 2];
		dest = argv[3 + i * 2];

/*		printf("\t%s -> /%s\n", src, dest);*/

		strcpy(headers[i].name, dest);		/* set name */
		headers[i].offset = offset;			/* set offset */
		headers[i].magic = MAGIC;			/* set magic */

		/* open input file */
		if((input = fopen(src, "r")) == 0) {
			printf("%s: Could not open file %s", argv[0], src);
			return 2;
		}

		/* find filesize */
		fseek(input, 0, SEEK_END);
		headers[i].length = ftell(input);
		offset += headers[i].length;

		fclose(input);
	}

	/* open output file and write headers */
	output = fopen(argv[1], "w");
	fwrite(&file_count, sizeof(int), 1, output);
	fwrite(headers, sizeof(struct initrd_file_header), 32, output);

	/* copy contents */
	for(i = 0; i < file_count; i++) {
		src = argv[2 + i * 2];

		/* open file and create buffer */
		input = fopen(src, "r");
		buffer = (char *)malloc(headers[i].length);

		/* read from input into buffer and write from buffer into output */
		fread(buffer, 1, headers[i].length, input);
		fwrite(buffer, 1, headers[i].length, output);

		/* close input file */
		free(buffer);
		fclose(input);
	}

	fclose(output);

	return 0;
}
