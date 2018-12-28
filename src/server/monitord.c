#include <stdio.h>
#include "../../include/server_helper_functions.h"

int main(int argc, char **argv){

    int h_selected = 0;
    int c_selected = 0;
    int port = 8000;
    char *log_file_path = NULL;

    if(validate_options(&h_selected, &c_selected, &port, &log_file_path, argc, argv))
        return 1;

    if(h_selected){

        // TODO: develop -h option's task.

    } else if(c_selected){

        // TODO: develop -c option's task.

    } else if(log_file_path){

        // TODO: develop -l option's task.

    } else{

        // TODO: develop default behavior.
    }

    return 0;
}
