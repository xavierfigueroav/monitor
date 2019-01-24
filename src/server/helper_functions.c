#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../include/server_helper_definitions.h"
int validate_options(int *hflag, int *cflag, int *lflag, char **log_file, char **port, int argc, char **argv){

    int c, carg;
    long port_number;

    while((c = getopt(argc, argv, "chl::")) != -1){

        switch(c){

            case 'c':
                *cflag = 1;
                break;
            case 'l':
                *lflag = 1;
                break;
            case 'h':
                *hflag = 1;
                break;
            case '?':
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                return 0;
            default:
                abort();
        }
    }
    
    carg = argc - optind;

    if(*cflag + *hflag + *lflag > 1 ){

        fprintf (stderr, "More than one option was chosen.\n");
        return 0;
    }
    
    if(carg > 2 || (*cflag && carg > 1) || (*hflag && carg != 0)){
    
        fprintf (stderr, "More arguments than expected were given.\n");
        return 0;
        
    }

    if(carg == 1){
        char *trash;
        *port = argv[optind];
        port_number = strtol(argv[optind], &trash, 10);

        if(port_number == 0 || *trash != '\0'){

            if(*lflag){

                *log_file = argv[optind];
                *port = DEFAULT_PORT;

            } else{
                fprintf (stderr, "<port> argument must be an integer value.\n");
                return 0;
            }
        }
    } else if(carg == 2){

        if(!*lflag){
            fprintf (stderr, "More arguments than expected were given.\n");
            return 0;
        }

        *log_file = argv[optind];

        char *trash;
        *port = argv[optind + 1];
        port_number = strtol(argv[optind + 1], &trash, 10);

        if(port_number == 0 || *trash != '\0'){
            fprintf (stderr, "<port> argument must be an integer value.\n");
            return 0;
        }
    }

    //printf("hflag: %d, cflag: %d, lflag: %d, log_file: %s, port: %s\n", *hflag, *cflag, *lflag, *log_file, *port);

    return 1;
}
