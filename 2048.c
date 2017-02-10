#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>

typedef struct $ {
	int map[4][4];
	long long score;
	int status;
	int moveFlag;
	int restTiles;
} status;

status gameStatus;

void init() {
	srand((unsigned)time(NULL));
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			gameStatus.map[i][j] = 0;
		}
	}
	gameStatus.score = 0;
	gameStatus.status = 0;
	gameStatus.restTiles = 16;
}

void map2str(const int number, char* output) {
	if (number == 0) {
		strcpy(output, "      ");
		return;
	}
	int t = 1 << number;

	char bufferOfNumber[4] = "";
	sprintf(bufferOfNumber, "%i", t);

	int prefix = (6 - strlen(bufferOfNumber)) >> 1;
	int postfix = 6 - prefix - strlen(bufferOfNumber);
	sprintf(output, "%*s%s%*s", prefix, " ", bufferOfNumber, postfix, " ");
}

void printLine1(const char* content) {
	printf("%s\n\r", content);
}

void printLine3(const char* begin, const char* content, const char* end) {
	char buffer[98] = "";
	sprintf(buffer, "        %s%s%s ", begin, content, end);

	printLine1(buffer);
}

void printLine4(const char* begin, const char* content, const char* border, const char* end) {
	char bufferPerBlock[19] = "";
	sprintf(bufferPerBlock, "%s%s%s%s%s%s", content, content, content, content, content, content);

	char buffer[82] = "";
	sprintf(buffer, "%s%s%s%s%s%s%s", bufferPerBlock, border, bufferPerBlock, border, bufferPerBlock, border, bufferPerBlock);

	printLine3(begin, buffer, end);
}

void printLine7(const char* begin, const char* content0, const char* content1, const char* content2, const char* content3, const char* border, const char* end) {
	char buffer[56] = "";
	sprintf(buffer, "%s%s%s%s%s%s%s", content0, border, content1, border, content2, border, content3);

	printLine3(begin, buffer, end);
}

void printLine7i(const char* begin, const int content0, const int content1, const int content2, const int content3, const char* border, const char* end) {
	char bufferPerBlock0[7] = "";
	map2str(content0, bufferPerBlock0);

	char bufferPerBlock1[7] = "";
	map2str(content1, bufferPerBlock1);

	char bufferPerBlock2[7] = "";
	map2str(content2, bufferPerBlock2);

	char bufferPerBlock3[7] = "";
	map2str(content3, bufferPerBlock3);

	printLine7(begin, bufferPerBlock0, bufferPerBlock1, bufferPerBlock2, bufferPerBlock3, border, end);
}

void printTableHead() {
	printLine4("╔", "═", "╦", "╗");
}

void printTableBorder() {
	printLine4("╠", "═", "╬", "╣");
}

void printTableFoot() {
	printLine4("╚", "═", "╩", "╝");
}

void printTableContent() {
	for (int i = 0; i < 4; ++i) {
		if (i != 0) printTableBorder();
		printLine4("║", " ", "║", "║");
		printLine7i("║", gameStatus.map[i][0], gameStatus.map[i][1], gameStatus.map[i][2], gameStatus.map[i][3], "║", "║");
		printLine4("║", " ", "║", "║");
	}
}

void printScore() {
	char buffer[100] = "";

	printLine4("╔", "═", "═", "╗");
	printLine3("║", "           SCORE           ", "║");
	if (gameStatus.status == 1)
		sprintf(buffer, "            WIN            ");
	else if (gameStatus.status == -1)
		sprintf(buffer, "       FIALED %5lld        ", gameStatus.score);
	else
		sprintf(buffer, "           %5lld           ", gameStatus.score);
	printLine3("║", buffer, "║");
	printLine4("╚", "═", "═", "╝");
	// score
}

void printHelp() {
	printLine1("");
	printLine1("\033[7m ↑↓←→ \033[0m move the tiles   \033[7m Ctrl + N \033[0m start new game");
}

void operation(int* elmt0, int* elmt1, int* elmt2, int* elmt3) {
	int* map[4] = {elmt0, elmt1, elmt2, elmt3};

	int preBlank = 0;
	for (int i = 0; i < 4; ++i) {
		if (*map[i] != 0) {
			if ((i < 3) && (*map[i] == *map[i + 1])) {
				(*map[i])++;
				*map[i + 1] = 0;
				gameStatus.score += 1 << *(map[i]);
				gameStatus.moveFlag = 1;
				gameStatus.restTiles++;
				if (*map[i] == 2048)
					gameStatus.status = 1;
			}
			if (preBlank != i) gameStatus.moveFlag = 1;
			int t = *(map[i]);
			*map[i] = 0;
			*map[preBlank] = t;
			preBlank++;
		}
	}
}

void turn(char direction) {
	if (gameStatus.status != 0) return;
	switch(direction) {
		case 0:
			for (int j = 0; j < 4; ++j)
				operation(&gameStatus.map[0][j], &gameStatus.map[1][j], &gameStatus.map[2][j], &gameStatus.map[3][j]);
			break;
		case 1:
			for (int j = 0; j < 4; ++j)
				operation(&gameStatus.map[3][j], &gameStatus.map[2][j], &gameStatus.map[1][j], &gameStatus.map[0][j]);
			break;
		case 2:
			for (int i = 0; i < 4; ++i)
				operation(&gameStatus.map[i][3], &gameStatus.map[i][2], &gameStatus.map[i][1], &gameStatus.map[i][0]);
			break;
		case 3:
			for (int i = 0; i < 4; ++i)
				operation(&gameStatus.map[i][0], &gameStatus.map[i][1], &gameStatus.map[i][2], &gameStatus.map[i][3]);
			break;
	}
}

void randomAdd() {
	for (int i = 0; i < 2; ++i) {
		if (gameStatus.restTiles == 0) break;
		do
		{
			int x = (int)(4.0 * rand() / (RAND_MAX + 1.0));
			int y = (int)(4.0 * rand() / (RAND_MAX + 1.0));
			// printf("%d\n", y);
			if (gameStatus.map[x][y] == 0) {
				gameStatus.map[x][y] = (int)(1.0 + 2.0 * rand() / (RAND_MAX + 1.0));
				gameStatus.restTiles--;
				break;
			}
		} while (1);
	}
}

void checkFailed() {
	if (gameStatus.restTiles == 0) {
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				if ((gameStatus.map[i][j] == gameStatus.map[i + 1][j]) ||
					(gameStatus.map[i][j] == gameStatus.map[i][j + 1])) {
					return;
				}
		gameStatus.status = -1;
	}
}

void render(char flush) {
	if (flush == 1) {
		for (int i = 0; i < 23; ++i) {
			printf("\033[A");
		}
		printf("\r");
	} else {
		printLine1("          ___   ____  __ __  ____\n\r         |__ \\ / __ \\/ // / ( __ ) _____\n\r         __/ // / / / // /_/ __  |/ ___/\n\r        / __// /_/ /__  __/ /_/ // /__\n\r       /____/\\____/  /_/  \\____(_)___/\n\r");
	}

	randomAdd();
	checkFailed();

	printScore();

	printTableHead();
	printTableContent();
	printTableFoot();

	printHelp();

	fflush(stdout);
}

int main() {
	SCREEN *s = newterm(NULL, stdin, stdout);
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	raw();
	keypad(stdscr, TRUE);

	init();
	render(0);

	int ch;

	while (1)
	{
		ch = getch();
		if ((ch == 'Q') || (ch == 'q') || (ch == 3) || (ch == 4)) break;
		if (ch == 14) {
			init();
			render(1);
		}
		// printf("%d\n", ch);
		if (ch == '\033') { // if the first value is esc
			getch(); // skip the [
			switch(getch()) { // the real value
				case 'A':
					// code for arrow up
					turn(0);
					break;
				case 'B':
					// code for arrow down
					turn(1);
					break;
				case 'C':
					// code for arrow right
					turn(2);
					break;
				case 'D':
					// code for arrow left
					turn(3);
					break;
			}
			if (gameStatus.moveFlag != 0) {
				gameStatus.moveFlag = 0;
				render(1);
			}
		}
	}

	printLine1("Bye");

	endwin();

	return 0;
}
