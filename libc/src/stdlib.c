#include <stdlib.h>

double atof(const char* str) {
	return strtod(str, (char **) 0);
}

int atoi(const char* str) {
    return (int) atol(str);
}