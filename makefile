run: compile
	./bin/server

compile: src/server.c
	gcc -o bin/server src/server.c