#include <stdlib.h>
int get_free_memory(FILE *file);
int get_total_memory();
float *get_loadavg(FILE *file);
int get_cpus();
int *get_cpus_usage(FILE *file);
int *get_processes(FILE *file);
char **get_system();
float get_uptime(FILE *file);
