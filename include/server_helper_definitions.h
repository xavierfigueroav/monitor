#define DEFAULT_PORT "8000"
#define DEFAULT_LOG_FILE "log.txt"
#define HELP_MESSAGE "Options:\n -h Help, show this screen.\n -c Console mode.\n -l <log_file> Log file to use [default: log.txt].\nUsage:\n monitord [-c] [<port>]\n monitord [-l <log_file>] [<port>]\n monitord -h"
#define ON 1
#define OFF 0
int validate_options(int *hflag, int *cflag, int *lflag, char **log_file, char **port, int argc, char **argv);
