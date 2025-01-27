build:
	mkdir bin
	gcc -fdiagnostics-color=always -g ./src/main.c ./src/util.c -o ./bin/themesw -I/usr/local/include -L/usr/local/lib
