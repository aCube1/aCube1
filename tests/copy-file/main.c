#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_BUFFER_SIZE 512
#define HELP_TEXT "Modo de uso:\n" \
"\t./program origem destino\n"

/*
 * Read the contents of the origin and copy the content to destination file.
 * In error case, return 1.
 */
uint8_t copy_content_to(FILE *origin, FILE *dest);

int main(int argc, char *argv[])
{
	FILE *origin;
	FILE *dest;

	if (argc < 3) {
		printf("%s", HELP_TEXT);
		return EXIT_FAILURE;
	}

	origin = fopen(argv[1], "rb");
	dest = fopen(argv[2], "wb");

	if (origin == NULL) {
		printf("Can't open given origin file: %s!\n", argv[1]);
	} else if (dest == NULL) {
		printf("Can't open given destination file: %s!\n", argv[2]);
	} else if (copy_content_to(origin, dest) > 0) {
		printf("Can't copy content from origin to destination file!");
	}

	fclose(origin);
	fclose(dest);
}

uint8_t copy_content_to(FILE *origin, FILE *dest)
{
	uint8_t success = 0; /* 0 = Success | 1 = Failure */
	uint64_t buffer_size = 0;
	char tmp_buffer[MAX_BUFFER_SIZE] = {0}; /* Temporary buffer */
	char *buffer = calloc(MAX_BUFFER_SIZE, sizeof(char));
	if (buffer == NULL) {
		printf("Can't allocate memory to the temporary buffer!");
		success = 1;
	}

	/* Run loop until the end of file */
	while (!feof(origin) && success == 0) {
		size_t bytes_readed = fread(tmp_buffer, sizeof(char), MAX_BUFFER_SIZE, origin);
		if (bytes_readed != MAX_BUFFER_SIZE && ferror(origin)) {
			printf("An error occurried on read of the origin file!");
			success = 1;
		}

		buffer_size += bytes_readed;
		char *new_buffer = realloc(buffer, buffer_size * sizeof(char));
		if (new_buffer == NULL) {
			printf("Can't reallocate buffer memory!");
			success = 1;
		} else {
			buffer = new_buffer;
			strncat(buffer, tmp_buffer, bytes_readed);
		}
	}

	if (success == 0) {
		fwrite(buffer, sizeof(char), buffer_size, dest);
	}

	free(buffer);
	return success;
}
