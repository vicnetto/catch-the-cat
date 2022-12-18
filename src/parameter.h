#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>  
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "lib/MegaMimes.h"
#include "regex_evaluation.h"
#include "file_information.h"
#define TYPE_SIZE 100

typedef struct {
	char* name;
	char* size;
	char* date;
	char* mime;
	char* ctc;
	char* dir;
	char* perm;
	char* threads;
	bool test;
	bool color;
	bool ou;
	int quantity;
} Parameter;

void verify_name(int *successeful_parameters, char *file_name, char *value);
void verify_mime(int *successeful_parameters, char *full_path, char *value);
void verify_ctc(int *successful_parameters, char *full_path, char *value);
void verify_size(int *successful_parameters, char *full_path, int file_size, char *value);
void verify_date(int *successful_parameters, char *full_path, char *value);
void verify_permission(int *successful_parameters, char *full_path, int int_size, char *value);

#endif