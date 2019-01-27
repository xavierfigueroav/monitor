#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <signal.h>
#include <semaphore.h>
#include "../../include/server_helper_definitions.h"
#include "../../include/csapp.h"
#include "../../include/sysinfo.h"
#include "../../include/monitor.pb-c.h"

void sigint_handler(int sig);

void monitor();
void daemonize(const char *id);
void *sending_thread(void *fd);

int send_static_info(int connfd);
int send_variable_info(int connfd);

void set_static_info(SystemInfo *st_info, short *size);
void set_variable_info(PerformanceInfo *vb_info, short *size);

void log_connection(int fd, int status);
void build_log_message(int fd, int status, char *message);

int h_selected = 0;
int c_selected = 0;
int l_selected = 0;
char *port = DEFAULT_PORT;
char *log_file_path = DEFAULT_LOG_FILE;

FILE *uptime_f;
FILE *memory_f;
FILE *processes_f;
FILE *loadavg_f;
FILE *log_file;

sem_t sem;

int main(int argc, char **argv){

    if(!validate_options(&h_selected, &c_selected, &l_selected, &log_file_path, &port, argc, argv))
        return 1;

    signal(SIGINT, sigint_handler);

    if(h_selected)  printf("%s\n", HELP_MESSAGE);
    else if(c_selected)   monitor();
    else if(l_selected){

        log_file = fopen(log_file_path, "a");
        if(!log_file) log_file = fopen(log_file_path, "w");
        if(!log_file){ fprintf(stderr, "%s is not a valid path.\n", log_file_path);  return -1;   }

        setbuf(log_file, NULL);
        daemonize("monitord");

        monitor();

    } else{

        openlog("monitord", LOG_PID, LOG_DAEMON);
        daemonize("monitord");
        monitor();
    }

    return 0;
}

void monitor(){

    int listenfd, *connfd;
	unsigned int clientlen;
	struct sockaddr_in clientaddr;
	sem_init(&sem, 0, 1);

	listenfd = Open_listenfd(port);

    uptime_f = fopen("/proc/uptime", "r");
    memory_f = fopen("/proc/meminfo", "r");
    processes_f = fopen("/proc/stat", "r");
    loadavg_f = fopen("/proc/loadavg", "r");

	while(1){
		clientlen = sizeof(clientaddr);

		connfd = malloc(sizeof(int));
		*connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        pthread_t tid;
        pthread_create(&tid, NULL, sending_thread, (void *) connfd);
        pthread_detach(tid);
	}
}

void daemonize(const char *id){
	int fd0, fd1, fd2;
	pid_t pid;

	umask(0);

	if((pid = fork()) < 0)
	    printf("%s: can't fork", id);
	else if(pid != 0)
	    exit(0);
	setsid();

	for(int i = 0; i < 3; i++)
	    close(i);

	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	if(fd0 != 0 || fd1 != 1 || fd2 != 2){

	    syslog(LOG_ERR, "Unexpected file descriptors %d %d %d.\n", fd0, fd1, fd2);
	    exit(-1);
	}
}

void *sending_thread(void *fd){

    int *connfd = (int *) fd;

    log_connection(*connfd, ON);

    send_static_info(*connfd);
    send_variable_info(*connfd);

	close(*connfd);
    free(connfd);
}

void log_connection(int fd, int status){

    char message[MAXLINE];

    build_log_message(fd, status, message);

	if(c_selected)  printf("%s\n", message);
	else if(l_selected) fprintf(log_file, "%s\n", message);
	else    syslog(LOG_NOTICE, "%s\n", message);
}

void build_log_message(int fd, int status, char *message){

    struct sockaddr_in clientaddr;
    int len = sizeof(clientaddr);

	getsockname(fd, (struct sockaddr *) &clientaddr, &len);

    struct hostent *hp;
    char *haddrp;
	hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
	haddrp = inet_ntoa(clientaddr.sin_addr);

	time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    if(c_selected || l_selected){

        if(status)
            sprintf(message, "[%d/%d/%d %d:%d:%d] monitord[%d]: Server connected to %s (%s)", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, getpid(), hp->h_name, haddrp);
        else
            sprintf(message, "[%d/%d/%d %d:%d:%d] monitord[%d]: Server disconnected from %s (%s)", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, getpid(), hp->h_name, haddrp);

    } else{

        if(status)
            sprintf(message, "Server connected to %s (%s)", hp->h_name, haddrp);
        else
            sprintf(message, "Server disconnected from %s (%s)", hp->h_name, haddrp);
    }
}

int send_static_info(int connfd){
    SystemInfo static_info;
    void *buffer;
    short size;

    set_static_info(&static_info, &size);

    buffer = malloc(size);
    system_info__pack(&static_info, buffer);

    if(send(connfd, &size, 2, MSG_NOSIGNAL) < 0 || send(connfd, buffer, size, MSG_NOSIGNAL) < 0){

        log_connection(connfd, OFF);
        free(buffer);

        return -1;
    }

    free(buffer);
    return 0;
}

void set_static_info(SystemInfo *st_info, short *size){

    system_info__init(st_info);

    sem_wait(&sem);
    char **system_info = get_system();
    sem_post(&sem);

    st_info->system_name = system_info[0];
    st_info->version = system_info[1];

    st_info->has_num_processors = 1;

    sem_wait(&sem);
    st_info->num_processors = get_cpus();
    sem_post(&sem);

    st_info->has_mem_total = 1;

    sem_wait(&sem);
    st_info->mem_total = get_total_memory();
    sem_post(&sem);

    *size = system_info__get_packed_size(st_info);
}

int send_variable_info(int connfd){

    void *buffer;
    short size;

    setbuf(uptime_f, NULL);
    setbuf(memory_f, NULL);
    setbuf(processes_f, NULL);
    setbuf(loadavg_f, NULL);

    while(1){

        sleep(1);

        PerformanceInfo variable_info;
        set_variable_info(&variable_info, &size);

        buffer = malloc(size);

        performance_info__pack(&variable_info, buffer);

        if(send(connfd, &size, 2, MSG_NOSIGNAL) < 0 || send(connfd, buffer, size, MSG_NOSIGNAL) < 0){

            log_connection(connfd, OFF);

            free(variable_info.loadavg);
            free(buffer);

            return -1;
        }

        free(variable_info.loadavg);
        free(buffer);
    }

    return 0;
}

void set_variable_info(PerformanceInfo *vb_info, short *size){

    sem_wait(&sem);
    float *loadavg = get_loadavg(loadavg_f);
    sem_post(&sem);
    
    sem_wait(&sem);
    int *processes = get_processes(processes_f);
    sem_post(&sem);

    sem_wait(&sem);
    rewind(processes_f);
    sem_post(&sem);

    sem_wait(&sem);
    int *usage = get_cpus_usage(processes_f);
    sem_post(&sem);

    performance_info__init(vb_info);

    PerformanceInfo__LoadAvg *loadavg_info = malloc(sizeof(PerformanceInfo__LoadAvg));
    performance_info__load_avg__init(loadavg_info);

    loadavg_info->has_min5 = 1;
    loadavg_info->has_min10 = 1;
    loadavg_info->has_min15 = 1;

    loadavg_info->min5 = loadavg[0];
    loadavg_info->min10 = loadavg[1];
    loadavg_info->min15 = loadavg[2];

    vb_info->has_uptime = 1;
    vb_info->has_mem_free = 1;
    vb_info->has_num_process = 1;
    vb_info->has_num_process_running = 1;

    sem_wait(&sem);
    vb_info->uptime = get_uptime(uptime_f);
    sem_post(&sem);

    sem_wait(&sem);
    vb_info->mem_free = get_free_memory(memory_f);
    sem_post(&sem);

    vb_info->num_process = processes[0];
    vb_info->num_process_running = processes[1];

    vb_info->loadavg = loadavg_info;

    vb_info->n_processor_usage = usage[0];
    vb_info->processor_usage = malloc(sizeof(int) * usage[0]);

    for(int i = 0; i < usage[0]; ++i)
        *(vb_info->processor_usage + i) = usage[i + 1];

    *size = performance_info__get_packed_size(vb_info);

    sem_wait(&sem);
    rewind(uptime_f);
    sem_post(&sem);

    sem_wait(&sem);
    rewind(memory_f);
    sem_post(&sem);

    sem_wait(&sem);
    rewind(processes_f);
    sem_post(&sem);

    sem_wait(&sem);
    rewind(loadavg_f);
    sem_post(&sem);
}

void sigint_handler(int sig){

    if(l_selected)  fclose(log_file);
    else if(!c_selected)  closelog();

	fclose(uptime_f);
    fclose(memory_f);
    fclose(processes_f);
    fclose(loadavg_f);

    signal (sig, SIG_DFL);
    raise(sig);
}

