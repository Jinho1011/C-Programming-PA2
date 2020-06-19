#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#define MAP_SIZE 30
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32
#define ESC 27
#define NONE_MARK "  "
#define PLAYER_MARK "◆"
#define WALL_MARK "■"
#define FLAG_MARK "▶"
#define REVEALED_FLAG_MARK "▷"

struct mapNode {
	int isWall;
	int isFlag;
	int isRevealed;
	int flagNum;
};

void initMap(struct mapNode node[][MAP_SIZE]);
void genFlag(struct mapNode node[][MAP_SIZE], int flagCnt, int flagNum);
void genWall(struct mapNode node[][MAP_SIZE], int wallCnt);
void displayMap(struct mapNode node[][MAP_SIZE]);
int manageGame(struct mapNode node[][MAP_SIZE], int level);
void movePlayer(struct mapNode node[][MAP_SIZE], int* x, int* y, char ch);
void gotoxy(int x, int y);
void CursorView(char show);
void removeBlock();
void printStatus(double time, int count, double lTime, int lCount, int level);
void printIntro();

int main() {
	srand(time(NULL));
	CursorView(0);
	struct mapNode node[MAP_SIZE][MAP_SIZE];
	int gameRes, level = 0;

	// game explanation


	while (1) {
		initMap(node);
		gameRes = manageGame(node, level);

		if (gameRes == -1) {
			continue;
		}
		else if (gameRes == 0) {
			system("cls");
			printf("GOOD BYE!");
			return 0;
		}
		else {
			if (level == 2) {
				system("cls");
				printf("asd!");
				return 0;
			}
			level++;
		}
	}
}

void initMap(struct mapNode node[][MAP_SIZE]) {
	int i, j;

	// init all members to 0
	for (i = 0; i < MAP_SIZE; i++) {
		for (j = 0; j < MAP_SIZE; j++) {
			node[i][j].isFlag = FALSE;
			node[i][j].isWall = FALSE;
			node[i][j].isRevealed = FALSE;
		}
	}

	// init map boundary
	for (i = 0; i < MAP_SIZE; i++) {
		for (j = 0; j < MAP_SIZE; j++) {
			if (i == 0 || i == MAP_SIZE - 1 || j == 0 || j == MAP_SIZE - 1)
				node[i][j].isWall = TRUE;
		}
	}

	genFlag(node, 1, 1);
	genFlag(node, 1, 2);
	genFlag(node, 1, 3);
	genFlag(node, 1, 4);
	genFlag(node, 1, 5);
	genFlag(node, 5, 6);

	genWall(node, 50);
}

void genWall(struct mapNode node[][MAP_SIZE], int wallCnt) {
	int i, j;

	do {
		i = rand() % (MAP_SIZE - 1) + 1;
		j = rand() % (MAP_SIZE - 1) + 1;

		if (node[i][j].isWall == FALSE && node[i][j].isFlag == FALSE && i != 1 && j != 1) {
			node[i][j].isWall = TRUE;
			wallCnt--;
		}
	} while (wallCnt);
}

void genFlag(struct mapNode node[][MAP_SIZE], int flagCnt, int flagNum) {
	int i, j;

	do {
		i = rand() % (MAP_SIZE - 1) + 1;
		j = rand() % (MAP_SIZE - 1) + 1;

		if (node[i][j].isFlag == FALSE) {
			node[i][j].isFlag = TRUE;
			node[i][j].flagNum = flagNum;
			node[i][j].isRevealed = FALSE;
			flagCnt--;
		}
	} while (flagCnt);
}

void displayMap(struct mapNode node[][MAP_SIZE]) {
	int i, j;

	for (i = 0; i < MAP_SIZE; i++) {
		for (j = 0; j < MAP_SIZE; j++) {
			if (node[i][j].isWall == TRUE) {
				printf(WALL_MARK);
			}
			else {
				if (node[i][j].isFlag == TRUE) {
					if (node[i][j].isRevealed == TRUE) {
						printf(REVEALED_FLAG_MARK);
					}
					else {
						printf(FLAG_MARK);
					}
				}
				else {
					printf(NONE_MARK);
				}
			}
		}
		printf("\n");
	}
}

int manageGame(struct mapNode node[][MAP_SIZE], int level) {
	int x = 2, y = 1;
	clock_t start, now;
	int score = 0;
	char ch;
	char key;
	int count = 0;
	int lCount = 100 - (level * 5);
	double time;
	double lTime = 60 - (level * 5);;
	int isOverlap = 0;

	system("cls");
	displayMap(node);
	start = clock();

	while (1) {
		now = clock();
		time = (double)(now - start) / CLK_TCK;

		gotoxy(0, 31);
		printf("%d %d", x, y);

		if (count == lCount || time >= lTime) {
			system("cls");
			gotoxy(30, 10);
			printf("PRESS ESC TO EXIT OR SPACE TO");

			while (1) {
				key = _getch();
				if (key == SPACE) {
					return -1;
				}
				else if (key == ESC) {
					return 0;
				}
				else {
					continue;
				}
			}
		}

		gotoxy(x, y);
		printf(PLAYER_MARK);

		if (node[y][x / 2].isFlag == TRUE) {
			switch (node[y][x / 2].flagNum) {
			case 1: // treasure
				if (isOverlap != 1 && node[y][x / 2].isRevealed == FALSE) {
					node[y][x / 2].isRevealed = TRUE;
					isOverlap = 1;

					score = (level + 1) * 100 - count / 2 - (int)time;

					system("cls");
					gotoxy(30, 10);
					printf("SUCCESS");
					gotoxy(30, 11);
					printf("SCORE : %d", score);
					Sleep(3000);

					return score;
				}
				break;
			case 2:   // 시간단축
				if (isOverlap != 2 && node[y][x / 2].isRevealed == FALSE) {
					node[y][x / 2].isRevealed = TRUE;
					isOverlap = 2;

					lTime -= 5 * (level + 1);

					gotoxy(62, 11);
					printf("제한 시간이 %d만큼 줄었습니다..!", 5 * (level + 1));
				}
				break;
			case 3:   // 원점으로
				if (isOverlap != 3 && node[y][x / 2].isRevealed == FALSE) {
					node[y][x / 2].isRevealed = TRUE;
					isOverlap = 3;

					x = 2;
					y = 1;

					gotoxy(62, 11);
					printf("원점으로 돌아갔습니다..!");

					continue;
				}
				break;
			case 4:   // 시간연장
				if (isOverlap != 4 && node[y][x / 2].isRevealed == FALSE) {
					node[y][x / 2].isRevealed = TRUE;
					isOverlap = 4;

					lTime += 5 * (level + 1);

					gotoxy(62, 11);
					printf("제한 시간이 %d만큼 늘었습니다..!", 5 * (level + 1));
				}
				break;
			case 5: // 횟수증가
				if (isOverlap != 5 && node[y][x / 2].isRevealed == FALSE) {
					node[y][x / 2].isRevealed = TRUE;
					isOverlap = 5;

					lCount += 5 * (level + 1);

					gotoxy(62, 11);
					printf("제한 횟수가 %d만큼 늘었습니다..!", 5 * (level + 1));
				}
				break;
			case 6: // 그냥깃발
				node[y][x / 2].isRevealed = TRUE;
				gotoxy(62, 11);
				printf("아무 일도 일어나지 않았습니다..!");
				break;
			}
		}

		printStatus(time, count, lTime, lCount, level);

		if (_kbhit()) {
			gotoxy(0, 0);
			displayMap(node);
			ch = _getch();
			movePlayer(node, &x, &y, ch);

			/*
				\b * 25
				space * 25
			*/

			count++;
			isOverlap = 0;
		}
	}
}

void movePlayer(struct mapNode node[][MAP_SIZE], int* x, int* y, char ch) {
	removeBlock();
	switch (ch) {
	case LEFT:
		*x -= 2;
		if (node[*y][*x / 2].isWall == TRUE) *x += 2;
		break;
	case RIGHT:
		*x += 2;
		if (node[*y][*x / 2].isWall == TRUE) *x -= 2;
		break;
	case UP:
		*y -= 1;
		if (node[*y][*x / 2].isWall == TRUE) *y += 1;
		break;
	case DOWN:
		*y += 1;
		if (node[*y][*x / 2].isWall == TRUE) *y -= 1;
		break;
	default:
		break;
	}
}

void gotoxy(int x, int y) {
	COORD cd;
	cd.X = x;
	cd.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}

void CursorView(char show) {
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void removeBlock() {
	printf("\b\b");
	printf(NONE_MARK);
}

void printStatus(double time, int count, double lTime, int lCount, int level) {
	gotoxy(62, 3);
	printf("level is %d, ", level);
	gotoxy(62, 5);
	printf("제한 횟수는 %d, ", lCount);
	gotoxy(62, 6);
	printf("제한 시간은 %.0f초 입니다", lTime);

	gotoxy(62, 8);
	printf("횟수 : %d ", count / 2);
	gotoxy(62, 9);
	printf("시간 : %.2f ", time);
}

void printIntro() {
	
}