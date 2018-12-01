port = 10050
fdir = "/media/deathekid/Win10/Users/Carlos/Desktop/Mio/YGOPRO/"

run: compile
	./bin/server ${port} ${fdir}

compile: src/server.c
	gcc -o bin/server src/server.c src/connection.c src/parser.c src/list.c src/files.c src/directory.c src/utils.c

run_memory_check: compile
	valgrind --leak-check=yes ./bin/server ${port} ${fdir}