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

int main(int argc, char **argv){

    int clientfd, flagh, c;
    char *port, *host, buf[MAXLINE];
    rio_t rio;
    void *bufferSys, *bufferPer;
    SystemInfo *sysInfo ;
    PerformanceInfo *perInfo ;

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

    sysInfo = static_info(clientfd,bufferSys);
    show_info_system(sysInfo);
    system_info__free_unpacked(sysInfo, bufferSys);
    
    setbuf(stdin, NULL);
    while(Fgets(buf,MAXLINE,stdin) != NULL){
	if(buf == "q"){
	    exit(0);
	}
	perInfo = variable_info(clientfd,bufferPer);
	show_info_performance(perInfo);
	performance_info__free_unpacked(perInfo, bufferPer);	
    }
    Close(clientfd);
    exit(0);
}

SystemInfo *static_info(int connfd, void *buffer){
    short size;
    void *valloc;

    recv(connfd, &size, 2, MSG_NOSIGNAL);
    buffer = malloc(size);
    recv(connfd, &buffer, size, MSG_NOSIGNAL);
    SystemInfo *sistema = system_info__unpack(buffer, size, valloc);
    return sistema;
}

PerformanceInfo *variable_info(int connfd, void *buffer ){
    short size;
    void *valloc;

    recv(connfd, &size, 2, MSG_NOSIGNAL);
    buffer = malloc(size);
    recv(connfd, &buffer, size, MSG_NOSIGNAL);
    PerformanceInfo *performance = performance_info__unpack(buffer, size, valloc);
    return performance;
}

void show_info_system(SystemInfo *sys){
    printf("Nombre del sistema: %s\n",sys->system_name);
    printf("Version: %s\n",sys->version);
    printf("Numero de procesadores: %d\n",sys->num_processors);
    printf("Memoria total: %d\n",sys->mem_total);
}

void show_info_performance(PerformanceInfo *per){
    printf("Uptime: %f\n",per->uptime);
    printf("Porcentaje de uso: %ls\n",per->processor_usage );
    printf("Memoria libre: %d\n",per->mem_free);
    printf("Numero de procesos: %d\n",per->num_process);
    printf("Numero de procesos en ejecucion: %d\n",per->num_process_running);
}
