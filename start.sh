OBJ_DIRECTORY="obj"

if [ ! -d "$OBJ_DIRECTORY" ]; then
	echo "'$OBJ_DIRECTORY' does not exist. Creating..."
	mkdir "$OBJ_DIRECTORY"
else
	echo "'$OBJ_DIRECTORY' already exists"
fi

LIB_DIRECTORY="lib"
if [ ! -d "$LIB_DIRECTORY" ]; then
	echo "'$LIB_DIRECTORY' does not exist. Creating..."
	mkdir "$LIB_DIRECTORY"
else
	echo "'$LIB_DIRECTORY' already exists"
fi

gcc -c ./src/socket_utilities.c -I./header -lm -o ./obj/libsocket_utilities.o
ar rcs ./lib/libsocket_utilities.a ./obj/libsocket_utilities.o

gcc -c ./src/http_utilities.c -I./header -lm -o ./obj/libhttp_utilities.o
ar rcs ./lib/libhttp_utilities.a ./obj/libhttp_utilities.o

gcc -c ./src/file_utilities.c -I./header -lm -o ./obj/libfile_utilities.o
ar rcs ./lib/libfile_utilities.a ./obj/libfile_utilities.o

gcc ./src/server.c -I./header -L./lib -lsocket_utilities -lhttp_utilities -lfile_utilities -lm -o ./server
./server