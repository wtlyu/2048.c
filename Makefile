2048: 2048.c
	gcc 2048.c -lncurses -o 2048

install:
	sudo install -m 755 2048 /usr/local/bin

uninstall:
	sudo rm /usr/local/bin/2048
