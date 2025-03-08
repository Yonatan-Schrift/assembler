#include "../h/string_funcs.h"
#include <stdlib.h>
#include <string.h>

char *copy_string(const char *origin) {
	if (!origin) return NULL;
	/* Allocate memory for new string (length + null terminator) */
	char *copy = malloc(strlen(origin) + 1);
	if (!copy) {
		/* printf("Failed memory allocation\n"); */
		return NULL; /* Memory allocation failed */
	}
	/* Copy the string contents to new memory */
	strcpy(copy, origin);
	return copy;
}

/**
 *  This method removes everything after a character in a string.
 *  This is used for removing comments while proccessing the code.
 */
void remove_after_delim(char *origin, char delim) {
	char *pos;
	pos = strchr(origin, delim);
	if (pos != NULL) {
		*pos = '\0';
	}
}

int string_array_len(char **args) {
	int count;
	if (args == NULL) return EXIT_FAILURE;

	/* Count strings until NULL terminator is found */
	for(count = 0; args[count] != NULL; count++);

	return count;
}

int handle_malloc(void **var, int size) {
	*var = malloc(size);
	if (!*var) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
