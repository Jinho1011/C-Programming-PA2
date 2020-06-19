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
	int gameRes, level = 0, key, totalScore = 0;
	printIntro();
	while (1) {
		scanf_s("%d", &key);
		if (key == 1) {
			level = 0;
			break;
		}
		else if (key == 2) {
			level = 1;
			break;
		}
		else if (key == 3) {
			level = 2;
			break;
		}
		else continue;
	}

	while (1) {
		initMap(node);
		gameRes = manageGame(node, level);

		if (gameRes == -1)
			continue;
		else if (gameRes == 0) {
			system("cls");
			printf("GOOD BYE!");
			return 0;
		}
		else {
			if (level == 2) {
				totalScore += gameRes;
				system("cls");
				gotoxy(30, 10);
				printf("최종 점수 %d로 학교를 탈출했습니다!!\n\n\n\n\n", totalScore);
				return 0;
			}
			level++;
			totalScore += gameRes;
		}
	}
}

void initMap(struct mapNode node[][MAP_SIZE]) {
	int i, j;
	for (i = 0; i < MAP_SIZE; i++) {
		for (j = 0; j < MAP_SIZE; j++) {
			node[i][j].isFlag = FALSE;
			node[i][j].isWall = FALSE;
			node[i][j].isRevealed = FALSE;
		}
	}
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
					if (node[i][j].isRevealed == TRUE)
						printf(REVEALED_FLAG_MARK);
					else
						printf(FLAG_MARK);
				}
				else
					printf(NONE_MARK);
			}
		}
		printf("\n");
	}
}

int manageGame(struct mapNode node[][MAP_SIZE], int level) {
	int x = 2, y = 1, score = 0, count = 0, lCount = 100 - (level * 5), isOverlap = 0;
	clock_t start, now;
	char ch, key;
	double time, lTime = 60 - (level * 5);

	system("cls");
	displayMap(node);
	start = clock();

	while (1) {
		now = clock();
		time = (double)(now - start) / CLK_TCK;

		if (count / 2 == lCount || time >= lTime) {
			system("cls");
			gotoxy(30, 10);
			printf("PRESS ESC TO EXIT OR SPACE TO RESTART");
			while (1) {
				key = _getch();
				if (key == SPACE)
					return -1;
				else if (key == ESC)
					return 0;
				else
					continue;
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
					printf("출구를 찾았습니다!!!");
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
					printf("                                                 ");
					gotoxy(62, 11);
					printf("수업 시간이 %d만큼 줄었습니다..!", 5 * (level + 1));
				}
				break;
			case 3:   // 원점으로
				if (isOverlap != 3 && node[y][x / 2].isRevealed == FALSE) {
					node[y][x / 2].isRevealed = TRUE;
					isOverlap = 3;
					x = 2;
					y = 1;
					gotoxy(62, 11);
					printf("                                                 ");
					gotoxy(62, 11);
					printf("길을 잃어 입구로 돌아갔습니다..!");
					continue;
				}
				break;
			case 4:   // 시간연장
				if (isOverlap != 4 && node[y][x / 2].isRevealed == FALSE) {
					node[y][x / 2].isRevealed = TRUE;
					isOverlap = 4;
					lTime += 5 * (level + 1);
					gotoxy(62, 11);
					printf("                                                 ");
					gotoxy(62, 11);
					printf("수업 시간이 %d만큼 늘었습니다..!", 5 * (level + 1));
				}
				break;
			case 5: // 횟수증가
				if (isOverlap != 5 && node[y][x / 2].isRevealed == FALSE) {
					node[y][x / 2].isRevealed = TRUE;
					isOverlap = 5;
					lCount += 5 * (level + 1);
					gotoxy(62, 11);
					printf("                                                 ");
					gotoxy(62, 11);
					printf("제한 횟수가 %d만큼 늘었습니다..!", 5 * (level + 1));
				}
				break;
			case 6: // 그냥깃발
				node[y][x / 2].isRevealed = TRUE;
				gotoxy(62, 11);
				printf("                                                 ");
				gotoxy(62, 11);
				printf("출구가 아니었습니다...!");
				break;
			}
		}
		printStatus(time, count, lTime, lCount, level);
		if (_kbhit()) {
			gotoxy(0, 0);
			displayMap(node);
			ch = _getch();
			movePlayer(node, &x, &y, ch);
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
	if (level == 0)
		printf("현재 위치는 산학..!");
	else if (level == 1)
		printf("현재 위치는 새천년관..!");
	else if (level == 2)
		printf("현재 위치는 공학관..!");
	gotoxy(62, 5);
	printf("제한 횟수는 %d, ", lCount);
	gotoxy(62, 6);
	printf("수업 시간은 %.0f초 입니다", lTime);
	gotoxy(62, 8);
	printf("횟수 : %d ", count / 2);
	gotoxy(62, 9);
	printf("시간 : %.2f ", time);
}

void printIntro() {
	printf(" /$$   /$$                     /$$                 /$$           /$$ \n");
	printf("| $$  /$$/                    | $$                | $$          | $$ \n");
	printf("| $$ /$$/   /$$$$$$  /$$$$$$$ | $$   /$$ /$$   /$$| $$   /$$    | $$ \n");
	printf("| $$$$$/   /$$__  $$| $$__  $$| $$  /$$/| $$  | $$| $$  /$$/    | $$ \n");
	printf("| $$  $$  | $$  \ $$| $$  \ $$| $$$$$$/ | $$  | $$| $$$$$$/     |__/ \n");
	printf("| $$\  $$ | $$  | $$| $$  | $$| $$_  $$ | $$  | $$| $$_  $$          \n");
	printf("| $$ \  $$|  $$$$$$/| $$  | $$| $$ \  $$|  $$$$$$/| $$ \  $$     /$$ \n");
	printf("|__/  \__/ \______/ |__/  |__/|__/  \__/ \______/ |__/  \__/    |__/ \n");
	printf("\n\n");
	printf("                      PRESS LEVEL : 1 - 3                           \n\n\n");
	printf("                     건국대에 오신걸 환영합니다!!                         \n");
	printf("                      산학, 새천년관, 공대에서                           \n");
	printf("                    수업 시간 안에 탈출해보세요!!!                           \n");
	printf("		           난이도 입력 : ");
}