#include <stdio.h>
#include "../../include/server_helper_definitions.h"
#include "../../include/csapp.h"


typedef struct {

    struct sockaddr_in addr;
    int *fd;
    
} fd_and_sockaddr_t;

void console_mode(char *port);
void *console_mode_thread(void *pack);
int main(int argc, char **argv){

    int h_selected = 0;
    int c_selected = 0;
    int l_selected = 0;
    char *port = DEFAULT_PORT;
    char *log_file_path = DEFAULT_LOG_FILE;

    if(!validate_options(&h_selected, &c_selected, &l_selected, &log_file_path, &port, argc, argv))
        return 1;

    if(h_selected){
    
        printf("%s\n", HELP_MESSAGE);

    } else if(c_selected){

        console_mode(port);

    } else if(l_selected){

        // TODO: develop -l option's task.

    } else{

        // TODO: develop default behavior.
    }

    return 0;
}

void console_mode(char *port){
    
    int listenfd;
	unsigned int clientlen;
	struct sockaddr_in clientaddr;

	listenfd = Open_listenfd(port);
	while (1) {
		clientlen = sizeof(clientaddr);
		
		int *connfd = malloc(sizeof(int));
		*connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		
		fd_and_sockaddr_t pack;
		pack.addr = clientaddr;
		pack.fd = connfd;
        
        pthread_t tid;
        pthread_create(&tid, NULL, console_mode_thread, &pack);
        pthread_detach(tid);
        
	}
	exit(0);
    
}

void *console_mode_thread(void *pack){

	    printf("func\n");

        fd_and_sockaddr_t *utils = (fd_and_sockaddr_t *) pack;
        struct sockaddr_in clientaddr = utils->addr;
        int *connfd = utils->fd;
	    struct hostent *hp;
	    char *haddrp;

		hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
					sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		haddrp = inet_ntoa(clientaddr.sin_addr);
		
		printf("Server connected to %s (%s)\n", hp->h_name, haddrp);

		Close(*connfd);
		free(connfd);
}










