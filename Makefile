port = 10050
fdir = "/home/deathekid/Documents/C Projects/WebServer/"

run: compile
	./bin/server ${port} ${fdir}

compile: src/server.c
	gcc -o bin/server src/server.c src/connection.c