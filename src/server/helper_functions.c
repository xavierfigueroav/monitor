#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int validate_options(int *hflag, int *cflag, int *port, char **log_file, int argc, char **argv){

    int c;

    while((c = getopt(argc, argv, "::cl:h")) != -1){

        switch(c){

            case 'c':
                *cflag = 1;
                break;
            case 'l':
                *log_file = optarg;
                break;
            case 'h':
                *hflag = 1;
                break;
            case '?':
                if(optopt == 'l')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                return 1;
            default:
                abort();
        }
    }

    if(*cflag + *hflag > 1 || (*cflag + *hflag > 0 && *log_file != NULL)){

        fprintf (stderr, "More than one option was chosen.\n");
        return 1;
    }

    if(argc - optind > 1){

        fprintf (stderr, "More arguments than expected were given.\n");
        return 1;

    } else if(argc - optind == 1){

        char *trash;
        *port = (int) strtol(argv[optind], &trash, 10);
        
        printf("port: %d", *port);

        if(*port == 0 || *trash != '\0'){
            fprintf (stderr, "<port> argument must be an integer value.\n");
            return 1;
        }
    }

    return 0;
}
