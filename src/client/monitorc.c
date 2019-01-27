#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../include/csapp.h"
#include "../../include/help_message.h"
#include "../../include/monitor.pb-c.h"

SystemInfo *static_info(int connfd, void *buffer);
PerformanceInfo *variable_info(int connfd, void *buffer);
void show_info_system(SystemInfo *sys);
void show_info_performance(PerformanceInfo *per);
void show_info_performance_loadavg(PerformanceInfo__LoadAvg *loadavg);

int main(int argc, char **argv){

    int clientfd, flagh, c;
    char *port, *host, buf[MAXLINE];
    rio_t rio;
    short sizesy, sizeper;
    char *bufferSys, *bufferPer;

    if(argc == 2 && argv[1] == "-h"){
	flagh = 1;
    }else if(argc == 3){
	host = argv[1];
	port = argv[2];
    }else if(argc == 1){
	host = "127.0.0.1";
	port = "8000";
    }else{
	printf("%s\n",HELP_MESSAGE);
	exit(0);
    }

    if(flagh > 0){
	printf("%s\n",HELP_MESSAGE);
	exit(0);
    }
    clientfd = Open_clientfd(host, port);

    recv(clientfd, &sizesy, 2, 0);
    bufferSys = (char *) malloc(sizeof(char)*sizesy);

    recv(clientfd, bufferSys, sizesy, 0);
    SystemInfo *sysInfo = system_info__unpack(NULL,sizesy,bufferSys);
    
    show_info_system(sysInfo);
    system_info__free_unpacked(sysInfo, NULL);
    free(bufferSys);
    
    while(1){

	recv(clientfd, &sizeper, 2, 0);
	bufferPer = (char *) malloc(sizeof(char)*sizeper);

	recv(clientfd, bufferPer, sizeper, 0);
	PerformanceInfo *perInfo = performance_info__unpack(NULL,sizeper,bufferPer);
	PerformanceInfo__LoadAvg *loadavg = perInfo->loadavg;
	show_info_performance_loadavg(loadavg);
	show_info_performance(perInfo);
	performance_info__free_unpacked(perInfo, NULL);
    }
    Close(clientfd);
    exit(0);
}

void show_info_system(SystemInfo *sys){
    printf("Nombre del sistema: %s\n",sys->system_name);
    printf("Version: %s\n",sys->version);
    printf("Numero de procesadores: %d\n",sys->num_processors);
    printf("Memoria total: %d Kb\n",sys->mem_total);
}

void show_info_performance(PerformanceInfo *per){
    printf("Uptime: %.2f\n",per->uptime);
    printf("Porcentaje de uso: %ls\n %",per->processor_usage);
    printf("Memoria libre: %d Kb\n",per->mem_free);
    printf("Numero de procesos: %d\n",per->num_process);
    printf("Numero de procesos en ejecucion: %d\n",per->num_process_running);
}

void show_info_performance_loadavg(PerformanceInfo__LoadAvg *loadavg){
    printf("Carga Promedio: 5 min:%.2f 10 min:%.2f 15 min:%.2f\n", loadavg->min5, loadavg->min10, loadavg->min15);
}
