
#include "utils.h"

void info(const char *str)
{
    fprintf(stdout, "INFO: %s\n", str);
}

void error(const char *str)
{
    fprintf(stderr, "ERROR: %s\n", str);
}
