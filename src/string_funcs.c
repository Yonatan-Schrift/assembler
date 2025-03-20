#include "../h/string_funcs.h"
#include <stdlib.h>
#include <string.h>

char *copy_string(const char *origin) {
	char *copy;
	if (!origin) return NULL;

	/* Allocate memory for new string (length + null terminator) */
	copy = malloc(strlen(origin) + 1);
	if (!copy) {
		/* Memory allocation failed */
		return NULL; 
	}
	/* Copy the string contents to new memory */
	strcpy(copy, origin);
	return copy;
}

void remove_quotes(char *string) {
	int len;
	/* Check if string is valid */
	if (!string) return;

	len = strlen(string);
	/* Check if string has at least opening and closing quotes */
	if (len >= 2 && string[0] == '"' && string[len-1] == '"') {
		/* Shift all characters left by one position */
		memmove(string, string + 1, len - 2);
		/* Null terminate the string after removing quotes */
		string[len - 2] = '\0';
	}
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
