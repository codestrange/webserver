port = 10050
fdir = "/media/leynier/Leynier EXT 2TB/Package"

run: compile
	./bin/server ${port} ${fdir}

compile: src/server.c
	gcc -o bin/server src/server.c src/connection.c src/parser.c src/list.c src/files.c src/directory.c src/utils.c

run_memory_check: compile
	valgrind --leak-check=yes ./bin/server ${port} ${fdir}