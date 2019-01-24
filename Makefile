SERVER_PATH_PREFIX = src/server/
CLIENT_PATH_PREFIX = src/client/
CFLAGS = -Wall -I .
LIB = -lpthread

monitord: $(SERVER_PATH_PREFIX)monitord.c $(SERVER_PATH_PREFIX)helper_functions.c src/csapp.c
	gcc -I. -o monitord $(SERVER_PATH_PREFIX)monitord.c $(SERVER_PATH_PREFIX)helper_functions.c src/csapp.c $(LIB)

monitorc: $(CLIENT_PATH_PREFIX)monitorc.c csapp.o
	gcc -o monitorc $(CLIENT_PAHT_PREFIX)monitor.c csapp.o $(LIB)

csapp.o: src/csapp.c
	gcc $(CFLAGS) -c src/csapp.c
