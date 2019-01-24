#include <stdlib.h>
#define MAXLINE 10000
int get_free_memory(FILE *file);
int get_total_memory();
float *get_loadavg(FILE *file);
int get_cpus();
int *get_proccesses(FILE *file);
char **get_system();
