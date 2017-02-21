2048: 2048.c getch.c
	clang 2048.c getch.c -o 2048

install:
	sudo install -m 755 2048 /usr/local/bin

uninstall:
	sudo rm /usr/local/bin/2048
