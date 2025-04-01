#include "../h/string_funcs.h"
#include "../h/globals.h"
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
	if (len >= 2 && string[0] == '"' && string[len - 1] == '"') {
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
	for (count = 0; args[count] != NULL; count++);

	return count;
}

int check_for_commas(char *string) {
	if (!string) return FAIL_CODE;

	/* Check for commas before and after each param */
	if(*(string + 1) == ',') {
		return EXTRA_COMMA_AFTER_PARAM;
	}
	if(*(string - 1) == ',') {
		return EXTRA_COMMA_BEFORE_PARAM;
	}
	return EXIT_SUCCESS;
}

int find_quotes(char *string) {
	int i, ret = -1, count = 0;

	/* Check if string is valid */
	if (!string) return FAIL_CODE;

	/* Search for the first quote character */
	for (i = 0; string[i] != '\0'; i++) {
		if (string[i] == '"') {
			count++;
			/* save the index of the first quote */
			if (ret == -1) ret = i;
		}
	}

	/* If found exactly 2 quotes return the index of the first one */
	if (count == 2) {
		if (string[i - 1] == '"') return ret;
		return EXTRA_TEXT_AFTER_STRING;
	}
	/* If found more than 2, there's (at least) one extra quote */
	else if (count > 2) {
		return EXTRA_QUOTATION_MARK;
	}
	/* If found exactly 1 quote, there's missing a quotation mark */
	else if (count == 1) {
		return (string[i - 1] == '"') ? MISSING_OPENING_QUOTE : MISSING_CLOSING_QUOTE;
	}

	/* No quote found */
	return MISSING_ARGS;
}