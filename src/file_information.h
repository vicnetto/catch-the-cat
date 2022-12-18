#ifndef FILE_INFORMATION_H_
#define FILE_INFORMATION_H_

#include<time.h>

long int get_file_size(char *full_path);

time_t get_file_last_access(char *full_path);

int get_octal_chmod(char *full_path);

#endif