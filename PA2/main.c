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
#define GUARD_MARK "▣"
#define FLAG_MARK "▶"
#define TREASURE_MARK "▦"
#define TEXT_COLOR_WHITE 15
#define TEXT_COLOR_RED 12
#define TEXT_COLOR_GREEN 10

struct mapNode
{
	int isWall;
	int isFlag;
	int flagNum;
};

void initMap(struct mapNode node[][MAP_SIZE]);
void genFlag(struct mapNode node[][MAP_SIZE], int flagCnt, int flagNum);
void genWall(struct mapNode node[][MAP_SIZE], int wallCnt);
void displayMap(struct mapNode node[][MAP_SIZE]);
void manageGame(struct mapNode node[][MAP_SIZE]);
void movePlayer(struct mapNode node[][MAP_SIZE], int *x, int *y);
void gotoxy(int x, int y);
void CursorView(char show);
void removeBlock();
void textcolor(int color_number);

int main()
{
	srand(time(NULL));
	CursorView(0);
	struct mapNode node[MAP_SIZE][MAP_SIZE];
	initMap(node);
	manageGame(node);
	displayMap(node);
}

void initMap(struct mapNode node[][MAP_SIZE])
{
	int i, j;

	// init all members to 0
	for (i = 0; i < MAP_SIZE; i++)
	{
		for (j = 0; j < MAP_SIZE; j++)
		{
			node[i][j].isFlag = FALSE;
			node[i][j].isWall = FALSE;
		}
	}

	// init map boundary
	for (i = 0; i < MAP_SIZE; i++)
	{
		for (j = 0; j < MAP_SIZE; j++)
		{
			if (i == 0 || i == MAP_SIZE - 1 || j == 0 || j == MAP_SIZE - 1)
				node[i][j].isWall = TRUE;
		}
	}

	genFlag(node, 1, 1);
	genFlag(node, 1, 2);
	genFlag(node, 1, 3);
	genFlag(node, 1, 4);
	genFlag(node, 1, 5);

	genWall(node, 50);
}

void genWall(struct mapNode node[][MAP_SIZE], int wallCnt)
{
	int i, j;

	do
	{
		i = rand() % (MAP_SIZE - 1) + 1;
		j = rand() % (MAP_SIZE - 1) + 1;

		if (node[i][j].isWall == FALSE && node[i][j].isFlag == FALSE)
		{
			node[i][j].isWall = TRUE;
			wallCnt--;
		}
	} while (wallCnt);
}

void genFlag(struct mapNode node[][MAP_SIZE], int flagCnt, int flagNum)
{
	int i, j;

	do
	{
		i = rand() % (MAP_SIZE - 1) + 1;
		j = rand() % (MAP_SIZE - 1) + 1;

		if (node[i][j].isFlag == FALSE)
		{
			node[i][j].isFlag = TRUE;
			node[i][j].flagNum = flagNum;
			flagCnt--;
		}
	} while (flagCnt);
}

void displayMap(struct mapNode node[][MAP_SIZE])
{
	int i, j;

	for (i = 0; i < MAP_SIZE; i++)
	{
		for (j = 0; j < MAP_SIZE; j++)
		{
			if (node[i][j].isWall == TRUE)
			{
				printf(WALL_MARK);
			}
			else
			{
				if (node[i][j].isFlag == TRUE)
				{
					printf(FLAG_MARK);
				}
				else
				{
					printf(NONE_MARK);
				}
			}
		}
		printf("\n");
	}
}

void manageGame(struct mapNode node[][MAP_SIZE])
{
	int x = 2, y = 1;

	displayMap(node);
	gotoxy(x, y);
	printf(PLAYER_MARK);

	/*
   flagNum 1 = 보물 깃발
   flagNum 2 = 시간 단축 깃발
   flagNum 3 = 원점 재배치 깃발
   flagNUm 4 = 시간 연장 깃발
   flagNum 5 = 장애물 제거 깃발
   */

	while (1)
	{
		movePlayer(node, &x, &y);
		gotoxy(x, y);
		printf(PLAYER_MARK);

		if (node[y][x / 2].isFlag == TRUE)
		{
			switch (node[y][x / 2].flagNum)
			{
			case 1:
				removeBlock();
				textcolor(TEXT_COLOR_GREEN);
				printf(FLAG_MARK);
				// check time
				// get score
				// print result
				// next level or play again
				break;
			case 2:
				removeBlock();
				textcolor(TEXT_COLOR_RED);
				printf(FLAG_MARK);

				break;
			}
		}
	}
}

void movePlayer(struct mapNode node[][MAP_SIZE], int *x, int *y)
{
	char ch;
	s if (_kbhit())
	{
		removeBlock();
		ch = _getch();
		switch (ch)
		{
		case LEFT:
			*x -= 2;
			if (node[*y][*x / 2].isWall == TRUE)
				*x += 2;
			break;
		case RIGHT:
			*x += 2;
			if (node[*y][*x / 2].isWall == TRUE)
				*x -= 2;
			break;
		case UP:
			*y -= 1;
			if (node[*y][*x / 2].isWall == TRUE)
				*y += 1;
			break;
		case DOWN:
			*y += 1;
			if (node[*y][*x / 2].isWall == TRUE)
				*y -= 1;
			break;
		default:
			break;
		}
		gotoxy(0, 30);
		printf("%d %d", *x, *y);
	}
}

void gotoxy(int x, int y)
{
	COORD cd;
	cd.X = x;
	cd.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}

void CursorView(char show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void removeBlock()
{
	printf("\b\b");
	printf(NONE_MARK);
}

void textcolor(int color_number)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}