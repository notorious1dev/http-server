gcc ./src/server.c ./src/socket_utilities.c ./src/queue.c ./src/http_utilities.c ./src/file_utilities.c -I./header -lm -o ./server
./server