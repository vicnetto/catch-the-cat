#include "parameter.h"

void verify_name(int *successful_parameters, char *file_name, char *value) {
    // Verifing if the name is contained on the file name.
    if (strcmp(file_name, value) == 0)
        (*successful_parameters)++;
    else {
        if (evaluate_regex_perfect_match(value, file_name))
            (*successful_parameters)++;
    }
}

void verify_mime(int *successful_parameters, char *full_path, int size_type, char *value) {
    if (getMegaMimeType(full_path) != NULL) {
        char typeSubType[size_type];
        char type[size_type];
        strcpy(typeSubType, getMegaMimeType(full_path));

        int endType = strchr(typeSubType, '/') - typeSubType;
        strncpy(type, typeSubType, endType);
        type[endType] = '\0';

        if (!strcmp(type, value) || !strcmp(typeSubType, value))
            (*successful_parameters)++;
    }
}

void verify_ctc(int *successful_parameters, char *full_path, char *value) {

    FILE *fp = fopen(full_path, "r");

    regex_t regex;
    regcomp(&regex, value, 0);

    if (fp != NULL) {
        int test = fseek(fp, 0, SEEK_END);

        if (test >= 0) {
            long size = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            char buffer[size + 1];

            fread(buffer, size, 1, fp);
            buffer[size] = '\0';

            if (0 == regexec(&regex, buffer, 0, NULL, 0))
                (*successful_parameters)++;
        }

        fclose(fp);
    }
}

void verify_size(int *successful_parameters, char *full_path, int file_size, char *value) {
    long int size = get_file_size(full_path);

    if (size < 0)
        return;

    // Gets the multiplier that will be used to calculate the size of the file. For example, in case of 'k',
    // the file needs to be '+' or '-' than 1024.
    char unit = value[strlen(value) - 1];
    int multiplier = unit == 'k' ? 1024 : unit == 'M' ? 1024 * 1024 : unit == 'G' ? 1024 * 1024 * 1024 : 1;

    // Gets only the number in the -size flag.
    char threshold_as_string[file_size];
    strncpy(threshold_as_string, value + !isdigit(value[0]), strlen(value) - (1 + !isdigit(value[0])));

    long int threshold = (long) atoi(threshold_as_string) * multiplier;
    
    // In case the file in the range
    if ((value[0] == '+'    && size > threshold) || 
        (value[0] == '-'    && size < threshold) ||
        (isdigit(value[0])  && size == threshold))
        (*successful_parameters)++;
}

void verify_date(int *successful_parameters, char *full_path, char *value) {
    time_t now = time(0);

    time_t last_access = get_file_last_access(full_path);

    char unit = value[strlen(value) - 1];
    int multiplier = unit == 'm' ? 60 : unit == 'h' ? 3600 : unit == 'j' ? 3600*24 : 60;

    time_t wanted_delay;

    if (value[0] == '+') {
        char number[strlen(value) - 2];
        memcpy(number, value + 1, strlen(value) - 1);
        wanted_delay = multiplier * atoi(number);
    }
    else {
        char number[strlen(value) - 1];
        memcpy(number, value, strlen(value) - 1);
        wanted_delay = multiplier * atoi(number);
    }

    time_t delay = now - last_access;

    if (value[0] == '+') {
        if (delay > wanted_delay) {
            (*successful_parameters)++;
        }
    } else {
        if (delay < wanted_delay) {
            (*successful_parameters)++;
        }
    }
}