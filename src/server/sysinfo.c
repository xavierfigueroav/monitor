#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 10000
float get_uptime(FILE *file){
    // file should point to '/proc/uptime'
    float uptime = 0;
    fscanf(file, "%f", &uptime);

    return uptime;
}

float *get_loadavg(FILE *file){
    // file should point to '/proc/loadavg'

    float *loadavg = (float *) malloc(sizeof(float) * 3);
    fscanf(file, "%f %f %f", loadavg, (loadavg + 1), (loadavg + 2));

    return loadavg;
}

int get_cpus(){
    int n = 0;
    size_t s;
    char *line = (char *) malloc(sizeof(char) * MAXLINE);

    FILE *file = fopen("/proc/stat", "r");
    getline(&line, &s, file); // skip the first line starting with 'cpu'

    while(getline(&line, &s, file) != -1)
        if(strstr(line, "cpu"))
            n++;
    fclose(file);

    return n;
}

int *get_processes(FILE *file){
    // file should point to '/proc/stat'
    int *proc = (int *) malloc(sizeof(int) * 2);
    size_t s;
    char *line = (char *) malloc(sizeof(char)* MAXLINE);
    int i = 0;

    while(getline(&line, &s, file) != -1 && i < 2){
        if(strstr(line, "proc")){
            strtok(line, " "); // skip the word starting with 'proc'
            proc[i++] = atoi(strtok(NULL, " "));
        }
    }
    return proc;
}

int get_free_memory(FILE *file){
    // file should point to '/proc/meminfo'
    int *mem = malloc(sizeof(int) * 2);
    size_t n;
    char *line = (char *) malloc(sizeof(char) * MAXLINE);
    char trash[100];
    int memory = 0;

    getline(&line, &n, file); // skip MemTotal line
    fscanf(file, "%s %d", trash, &memory);

    return memory;
}

int get_total_memory(){
    size_t n;
    char *line = (char *) malloc(sizeof(char) * MAXLINE);
    int memory = 0;
    char trash[100];

    FILE * file = fopen("/proc/meminfo", "r");
    fscanf(file, "%s %d", trash, &memory);
    fclose(file);

    return memory;
}

char **get_system(){
    char **sysinfo = (char **) malloc(sizeof(char *) * 2);

    FILE *file1 = fopen("/proc/version", "r");
    char *linux_version = (char *) malloc(sizeof(char) * MAXLINE);
    size_t n;
    getdelim(&linux_version, &n, '(', file1);
    fclose(file1);

    linux_version[strlen(linux_version) - 2] = '\0';

    FILE *file2 = fopen("/proc/version_signature", "r");
    char *os = (char *) malloc(sizeof(char) * MAXLINE);

    fscanf(file2, "%s", os);
    fclose(file2);

    sysinfo[0] = os;
    sysinfo[1] = linux_version;

    return sysinfo;
}
