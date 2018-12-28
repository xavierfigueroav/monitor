SERVER_PATH_PREFIX = src/server/

monitord: $(SERVER_PATH_PREFIX)monitord.c $(SERVER_PATH_PREFIX)helper_functions.c
	gcc -o monitord $(SERVER_PATH_PREFIX)monitord.c $(SERVER_PATH_PREFIX)helper_functions.c -I.
