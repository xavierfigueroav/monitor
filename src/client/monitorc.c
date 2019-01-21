#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/csapp.h"
#include "../include/help_message.h"

int main(int argc, char **argv){

    int clientfd, flagh, c;
    char *port;
    char *host, buf[MAXLINE];
    rio_t rio;

    while(c = getopt(argc, argv, "h")){
	case 'h':
	    flagh = 1;
	    break;
	case '?':
	    if(argc == 3){
		host = argv[1];
		port = argv[2];
	    }else if(argc == 2){
		host = "127.0.0.1";
		port = argv[1];
	    }else{
		fprintf(stderr, "usage: %s [-h] [<ip>] <port>\n", argv[0]);
	    }
	    break;
    }

    if(flagh > 0){
	printf("%s\n",HELP_MESSAGE);
	exit(0);
    }

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    //comunicacion con el servidor
    while((rio_readlineb(&rio,buf,MAXLINE) != -1)){
	Fputs(buf,stdout);
    }

    Closed(clientfd);
    exit(0);
}
