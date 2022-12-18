#include <regex.h>
#include <stdbool.h>
#include "regex_evaluation.h"

bool evaluate_regex_perfect_match(char *expression, char *file_name) {
	// Or trying the regex.
	regex_t regex;
	size_t nmatch = 2;
	regmatch_t pmatch[2];

	regcomp(&regex, expression, 0);

	// Testing if the regex can be finded in the file name, and it returs 0 if true.
	if (0 == regexec(&regex, file_name, nmatch, pmatch, 0))
		return pmatch[0].rm_so == 0 && pmatch[0].rm_eo == strlen(file_name);
	else
		return 0;
}