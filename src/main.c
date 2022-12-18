#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>  
#include <ctype.h>
#include "parameter.h"
#include "regex_evaluation.h"

#define INT_SIZE 11
#define PRINT_ONLY_PATH true

void print_path(bool color, bool only_path, char *full_path, char *file_name) {
	if (color) {
		if (only_path) {
			printf("\033[0;35m");
			printf("%s\n", full_path);
			printf("\033[0m");
		} else {
			int size = strlen(full_path) - strlen(file_name);

			printf("\033[0;35m");
			printf("%.*s", size, full_path);
			printf("\033[0m");

			printf("\033[0;32m");
			printf("%s\n", file_name);
			printf("\033[0m");
		}
	} else
		printf("%s\n", full_path);
}

void verify_if_can_be_printed(Parameter parameter, char *full_path, char *file_name) {
	// Variable to verify if all the parameters have been used.
	int successful_parameters = 0;

	// Verifies if the name of the current file is the same as defined at the flag.
	if (parameter.name != NULL) {
		verify_name(&successful_parameters, file_name, parameter.name);
	}

	if (parameter.mime != NULL) {
		verify_mime(&successful_parameters, full_path, parameter.mime);
	}

	if (parameter.ctc != NULL) {
		verify_ctc(&successful_parameters, full_path, parameter.ctc);
	}

	// Verifies if the file is in the range of the size flag.
	if (parameter.size != NULL) {
		verify_size(&successful_parameters, full_path, INT_SIZE, parameter.size);
	}

	if (parameter.date != NULL) {
		verify_date(&successful_parameters, full_path, parameter.date);
	}

	if (parameter.quantity == successful_parameters)
		print_path(parameter.color, !PRINT_ONLY_PATH, full_path, file_name);

}

int show_files_in_specific_path(Parameter parameter, char *path) {

	DIR *d;
	struct dirent *dir;
	d = opendir(path);

	if (d) {
		while ((dir = readdir(d)) != NULL) {
			int size = strlen(path) + strlen(dir->d_name) + (path[strlen(path) - 1] == '/' ? 0 : 1) + 1;
			char full_path[size];
			snprintf(full_path, size, "%s%s%s", path, path[strlen(path) - 1] == '/' ? "" : "/",  dir->d_name);
			full_path[strlen(full_path)] = '\0';

			// Print file.
			if (dir->d_type == DT_REG) {
				if (parameter.dir == NULL) 
					verify_if_can_be_printed(parameter, full_path, dir->d_name);
			} else {
				// Print directory.
				if (strcmp("..", dir->d_name) && strcmp(".", dir->d_name)) {
					if (parameter.dir != NULL) {
						if (!strcmp(parameter.dir, "") || evaluate_regex_perfect_match(parameter.dir, dir->d_name))
							print_path(parameter.color, PRINT_ONLY_PATH, full_path, dir->d_name);
					} else if (parameter.quantity == 0) {
						print_path(parameter.color, PRINT_ONLY_PATH, full_path, dir->d_name);
					}

					show_files_in_specific_path(parameter, full_path);
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
	parameter.date = NULL;
	parameter.perm = NULL;
	parameter.threads = NULL;
	parameter.ctc = NULL;
	parameter.test = false;
	parameter.quantity = 0;

	if (argv < 2) {
		fprintf(stderr, "The path needs to be specified!\n");
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
				fprintf(stderr, "Multiple parameters are not allowed, are you trying to use a flag? TIP: start with -\n");
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

				if (parameter.size[0] != '+' && parameter.size[0] != '-' && !isdigit(parameter.size[0])) {
					fprintf(stderr, "Invalid for the flag size. Please try in the format: {+,-, }number{k,G,M}\n");
					return 1;
				}

				char lastDigit = parameter.size[strlen(parameter.size) - 1];
				if (lastDigit != 'c' && lastDigit != 'k' && lastDigit != 'M' && lastDigit != 'G' && !isdigit(lastDigit)) {
					fprintf(stderr, "Invalid for the flag size. Please try in the format: {+,-, }number{k,G,M}\n");
					return 1;
				}

				if (isdigit(argc[i + 1][0])) {
					char number[INT_SIZE] = "";
					strncpy(number, parameter.size + !isdigit(argc[i + 1][0]), strlen(parameter.size) - (1 + !isdigit(argc[i + 1][0])));

					if (!atoi(number)) {
						fprintf(stderr, "Invalid number for the flag -size. Please try in the format: {+,-, }number{k,G,M}\n");
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
			else if (!strcmp("-date", argc[i])) {
				parameter.date = argc[i + 1];

				if (argc[i + 1][0] != '+' && !isdigit(argc[i + 1][0])) {
					fprintf(stderr, "The value of the -date flag must start with '+' or the number.\n");

					return 1;
				}

				char number[INT_SIZE] = "";
				strncpy(number, parameter.date + !isdigit(argc[i + 1][0]), strlen(parameter.date) - 1);

				if (!atoi(number)) {
					fprintf(stderr, "Invalid number for the flag -date. Please try in the format: {+, }number{m,h,j}.\n");
					return 1;
				}

				if (argc[i + 1][strlen(argc[i + 1]) - 1] != 'm' && argc[i + 1][strlen(argc[i + 1]) - 1] != 'h' && argc[i + 1][strlen(argc[i + 1]) - 1] != 'j') {
					fprintf(stderr, "Wrong letter in the -date flag. Expected 'm', 'h' or 'j'.\n");
					return 1;
				}

				parameter.quantity++;
			}
			else if (!strcmp("-perm", argc[i])) {
				parameter.perm = argc[i + 1];
				parameter.quantity++;
			}
			else if (!strcmp("-threads", argc[i])) {
				parameter.threads = argc[i + 1];
				parameter.quantity++;
			}
			else if (!strcmp("-test", argc[i])) {
				i--;
				continue;
			}
			else if (!strcmp("-color", argc[i])) {
				parameter.color = true;
				i--;
				continue;
			}
			else {
				printf("Le flag %s n'est pas correct\n", argc[i]);
				return 1;
			}

			if (parameter.test == true)
				printf("La valeur du flag %s est %s\n", argc[i], argc[i+1]);
		}
	}

	if (parameter.test == true)
		return 0;

	if (parameter.quantity == 0)
		print_path(parameter.color, PRINT_ONLY_PATH, argc[1], NULL);

	show_files_in_specific_path(parameter, argc[1]);

	return 0;
}