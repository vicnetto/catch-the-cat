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
#include "parameter.h"
#include "regex_evaluation.h"
#include "file_size.h"

#define FULL_SIZE_OF_PATH 500
#define FULL_SIZE_OF_FILE 11
#define FULL_SIZE_OF_NAME 100
#define FULL_SIZE_OF_TYPE 100

void verify_if_can_be_printed(Parameter parameter, char *full_path, char *file_name, int depth) {
	// Variable to verify if all the parameters have been used.
	int successful_parameters = 0;

	// Verifies if the name of the current file is the same as defined at the flag.
	if (parameter.name != NULL) {
		verify_name(&successful_parameters, file_name, parameter.name);
	}

	if (parameter.mime != NULL) {
		verify_mime(&successful_parameters, full_path, FULL_SIZE_OF_PATH, parameter.mime);
	}

	if (parameter.ctc != NULL) {
		verify_ctc(&successful_parameters, full_path, parameter.ctc);
	}

	// Verifies if the file is in the range of the size flag.
	if (parameter.size != NULL) {
		verify_size(&successful_parameters, full_path, FULL_SIZE_OF_FILE, parameter.size);
	}

	if (parameter.quantity == successful_parameters)
		printf("%s\n", full_path);

}

int show_files_in_specific_path(int depth, char *path, Parameter parameter) {

	DIR *d;
	struct dirent *dir;
	d = opendir(path);

	if (d) {
		while ((dir = readdir(d)) != NULL) {
			char full_path[FULL_SIZE_OF_PATH];
			snprintf(full_path, FULL_SIZE_OF_PATH, "%s%s%s", path, path[strlen(path) - 1] == '/' ? "" : "/",  dir->d_name);

			// Print file.
			if (dir->d_type == DT_REG) {
				if (parameter.dir == NULL) 
					verify_if_can_be_printed(parameter, full_path, dir->d_name, depth);
			} else {
				// Print directory.
				if (strcmp("..", dir->d_name) && strcmp(".", dir->d_name)) {
					if (parameter.dir != NULL) {
						if (!strcmp(parameter.dir, "")) {
							printf("%s\n", full_path);
						} else if (evaluate_regex_perfect_match(parameter.dir, dir->d_name)) {
							printf("%s\n", full_path);
						}
					} else if (parameter.quantity == 0) {
						printf("%s\n", full_path);
					}

					show_files_in_specific_path(depth + 1, full_path, parameter);
				}
			}
		}

		closedir(d);
	}

	return 0;
}

// Verify if the first parameter is a file
int main(int argv, char *argc[]) {
	Parameter parameter;
	parameter.name = NULL;
	parameter.size = NULL;
	parameter.mime = NULL;
	parameter.dir = NULL;
	parameter.ctc = NULL;
	parameter.test = false;
	parameter.quantity = 0;

	if (argv < 2) {
		printf("The path needs to be specified!\n");
		return 1;
	}

	if (argv >= 3) {
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
			else if (!strcmp("-ctc", argc[i])) {
				parameter.ctc = argc[i + 1];
				parameter.quantity++;
			}
			else if (!strcmp("-dir", argc[i])) {
				if (argv - 2 > 1 && argc[i + 1][0] != '-')
					parameter.dir = argc[i + 1];
				else {
					parameter.dir = "";
					i--;
				}
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
	int iCleanPath = 0;
	for (int i = 0; argc[1][i] != '\0'; i++) {
		if (argc[1][i] == '/' && !isSlash) {
			isSlash = true;

			cleanPath[iCleanPath] = '/';
			iCleanPath++;
		} else if (argc[1][i] != '/') {
			isSlash = false;

			cleanPath[iCleanPath] = argc[1][i];
			iCleanPath++;
		} 
	}

	// Verifies if the path is already finishing with the slash.
	if (cleanPath[iCleanPath - 1] != '/') {
		cleanPath[iCleanPath++] = '/';
	}

	cleanPath[iCleanPath] = '\0';

	if (parameter.quantity == 0)
		printf("%s\n", cleanPath);

	show_files_in_specific_path(0, cleanPath, parameter);

	return 0;
}
