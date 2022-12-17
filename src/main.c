#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h> 
#include <stdbool.h>  
#include <dirent.h>
#include <ctype.h>
#include <regex.h>
#include "lib/MegaMimes.h"

#define FULL_SIZE_OF_PATH 500
#define FULL_SIZE_OF_FILE 11
#define FULL_SIZE_OF_NAME 100
#define FULL_SIZE_OF_TYPE 100

typedef struct {
	char* name;
	char* size;
	char* mime;
	bool test;
	int quantity;
} Parameter;

int i;

long int get_file_size(char *path, char *file) {
	char full_name[FULL_SIZE_OF_PATH];
	snprintf(full_name, FULL_SIZE_OF_PATH, "%s%s%s", path, "/",  file);

	FILE *fp = fopen(full_name, "ra");

	if (fp == NULL)
		return -1;

	if (fseek(fp, 0, SEEK_END) < 0) {
		fclose(fp);
		return -1;
	}

	long size = ftell(fp);
	// release the resources when not required
	
	fclose(fp);
	return size;
}

void print_file(char *path, char* file_name, int depth) {
	char name[FULL_SIZE_OF_PATH];
	snprintf(name, FULL_SIZE_OF_PATH, "%s%s%s", path, "/",  file_name);

	// print_spaces(depth);

	// printf("* (%d) %s\n", size, name);
	printf("%s\n", name);
}

void verify_if_can_be_printed(Parameter parameter, char *path, char *file_name, int depth) {
	// Variable to verify if all the parameters have been used.
	int successeful_parameters = 0;

	// Verifies if the name of the current file is the same as defined at the flag.
	if (parameter.name != NULL) {
		// Verifing if the name is contained on the file name.
		if (strcmp(file_name, parameter.name) == 0)
			successeful_parameters++;
		else {
			// Or trying the regex.
			regex_t regex;
			size_t nmatch = 2;
			regmatch_t pmatch[2];

			regcomp(&regex, parameter.name, 0);
 
			// Testing if the regex can be finded in the file name, and it returs 0 if true.
    		if (0 == regexec(&regex, file_name, nmatch, pmatch, 0)) {
				// In case of match, this line verifies if the match matches all the file_name.
				if (pmatch[0].rm_so == 0 && pmatch[0].rm_eo == strlen(file_name))
					successeful_parameters++;
			}
		}
	}

	if (parameter.mime != NULL) {

		char test[FULL_SIZE_OF_PATH];
		snprintf(test, FULL_SIZE_OF_PATH, "%s%s%s", path, "/", file_name);

		if (getMegaMimeType(test) != NULL) {
			char typeSubType[FULL_SIZE_OF_PATH];
			char type[FULL_SIZE_OF_PATH];
			strcpy(typeSubType, getMegaMimeType(test));

			int endType = strchr(typeSubType, '/') - typeSubType;
			strncpy(type, typeSubType, endType);
			type[endType] = '\0';

			if (!strcmp(type, parameter.mime) || !strcmp(typeSubType, parameter.mime))
				successeful_parameters++;
		}

	}

	// Verifies if the file is in the range of the size flag.
	if (parameter.size != NULL) {
		long int size = get_file_size(path, file_name);

		// Gets the multiplier that will be used to calculate the size of the file. For example, in case of 'k',
		// the file needs to be '+' or '-' than 1024.
		char unit = parameter.size[strlen(parameter.size) - 1];
		int multiplier = unit == 'k' ? 1024 : unit == 'M' ? 1024 * 1024 : unit == 'G' ? 1024 * 1024 * 1024 : 1;

		// Gets only the number in the -size flag.
		char threshold_as_string[FULL_SIZE_OF_FILE];
		strncpy(threshold_as_string, parameter.size + !isdigit(parameter.size[0]), strlen(parameter.size) - (1 + !isdigit(parameter.size[0])));

		long int threshold = (long) atoi(threshold_as_string) * multiplier;
		
		// In case the file in the range
		if ((parameter.size[0] == '+'    && size > threshold) || 
			(parameter.size[0] == '-'    && size < threshold) ||
			(isdigit(parameter.size[0])  && size == threshold))
			successeful_parameters++;

	}

	if (parameter.quantity == successeful_parameters)
		print_file(path, file_name, depth);

}

int show_files_in_specific_path(int depth, char *path, Parameter parameter) {

	DIR *d;
	struct dirent *dir;
	d = opendir(path);

	if (d) {
		while ((dir = readdir(d)) != NULL) {
			// Print file.
			if (dir->d_type == DT_REG) {
				verify_if_can_be_printed(parameter, path, dir->d_name, depth);
			} else {
				// Print directory.
				if (strcmp("..", dir->d_name) && strcmp(".", dir->d_name)) {
					char name[FULL_SIZE_OF_PATH];
					snprintf(name, FULL_SIZE_OF_PATH, "%s%s%s", path, "/",  dir->d_name);

					show_files_in_specific_path(depth + 1, name, parameter);
				}
			}
		}

		closedir(d);
	} else {
		printf("Does not exist!");
	}

	return 0;
}

// Verify if the first parameter is a file
int main(int argv, char *argc[]) {
	Parameter parameter;

	if (argv < 3) {
		printf("The path needs to be specified!\n");
		return 1;
	}

	if (argv >= 3) {
		parameter.name = NULL;
		parameter.size = NULL;
		parameter.mime = NULL;
		parameter.test = false;
		parameter.quantity = 0;

		for (int i = 2; i < argv; i += 2) {
			if (!strcmp("-test", argc[i])) {
				parameter.test = true;
			}
		}

		for (int i = 2; i < argv; i += 2) {
			if (argc[i][0] != '-') {
				printf("Multiple parameters are not allowed, are you trying to use a flag? TIP: start with -\n");
				return 1;
			}

			if (!strcmp("-name", argc[i])) {
				parameter.name = argc[i + 1];
				parameter.quantity++;
			}
			else if (!strcmp("-size", argc[i])) {
				// In case the last character is missing, an c needs to be placed.
				if (isdigit(argc[i + 1][strlen(argc[i + 1]) - 1]))
					strcat(argc[i + 1], "c");

				parameter.size = argc[i + 1];

				if (isdigit(argc[i + 1][0])) {
					char number[FULL_SIZE_OF_FILE] = "";
					strncpy(number, parameter.size + !isdigit(argc[i + 1][0]), strlen(parameter.size) - (1 + !isdigit(argc[i + 1][0])));

					if (!atoi(number)) {
						printf("Invalid number for the flag -size. Please try in the format: {+,-, }number{k,G,M}\n");
						return 1;
					}
				}

				parameter.quantity++;
			}
			else if (!strcmp("-mime", argc[i])) {
				parameter.mime = argc[i + 1];
				parameter.quantity++;
			}
			else if (!strcmp("-test", argc[i])) {
				i--;
				continue;
			}
			else {
				printf("Incorrect parameters, try again!\n");
				return 1;
			}

			if (parameter.test == true)
				printf("La valeur du flag %s est %s\n", argc[i], argc[i+1]);
		}
	}

	char cleanPath[FULL_SIZE_OF_PATH];
	bool isSlash = false;
	int pathLen = strlen(argc[1]);
	int iCleanPath = 0;
	for (int i = 0; argc[1][i] != '\0'; i++) {
		if (argc[1][i] == '/' && (!isSlash && pathLen - 1 != i)) {
			isSlash = true;

			cleanPath[iCleanPath] = '/';
			iCleanPath++;
		} else if (argc[1][i] != '/') {
			isSlash = false;

			cleanPath[iCleanPath] = argc[1][i];
			iCleanPath++;
		} 
	}

	show_files_in_specific_path(0, cleanPath, parameter);

	return 0;
}
