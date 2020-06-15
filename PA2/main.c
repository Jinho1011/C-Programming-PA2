#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#define MAP_SIZE 12
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

// 난이도별 맵 생성
void gen_beginner_map();
void gen_intermediate_map();
void gen_advanced_map();
void gen_flag(int temp_map[][MAP_SIZE - 1], int trap_flag, int help_flag, int wall);
void shuffle_map(int temp_map[][MAP_SIZE - 1]);
void set_map_1(int map[][MAP_SIZE]);

// 맵 출력
void display_map(int map[][MAP_SIZE]);

// 메뉴 출력
void display_menu();

// 플레이어 이동 입력
void move_player();

void gotoxy(int x, int y);

int main()
{
	srand(time(NULL));

	int map[MAP_SIZE][MAP_SIZE];
	set_map_1(map);
	int temp_map[MAP_SIZE - 1][MAP_SIZE - 1] = {0};

	gen_flag(temp_map, 3, 3, 20);
	shuffle_map(temp_map);

	// map에 temp_map 넣기
	for (int i = 1; i < MAP_SIZE - 1; i++)
	{
		for (int j = 1; j < MAP_SIZE - 1; j++)
		{
			map[i][j] = temp_map[i - 1][j - 1];
		}
	}

	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			printf("%d ", map[i][j]);
		}
		printf("\n");
	}
}

void display_map()
{
}

void gen_flag(int temp_map[][MAP_SIZE - 1], int trap_flag, int help_flag, int wall)
{
	int *map = temp_map;
	int index = 0;

	// 함정 깃발
	for (int i = 0; i < trap_flag; i++)
		*(map + index++) = rand() % 3 + 2;

	// 힌트 깃발
	for (int i = 0; i < help_flag; i++)
		*(map + index++) = rand() % 3 + 5;

	// 이동 불가능 영역
	for (int i = 0; i < wall; i++)
		*(map + index++) = 1;

	// 보물 깃발
	*(map + index++) = 8;

	// 원점
	*(map + index) = 9;
}

void shuffle_map(int temp_map[][MAP_SIZE - 1])
{
	int *map = temp_map;
	int temp_map_1d[(MAP_SIZE - 1) * (MAP_SIZE - 1)];
	int temp;

	for (int i = 0; i < (MAP_SIZE - 1) * (MAP_SIZE - 1); i++)
	{
		temp_map_1d[i] = *(map + i);
	}

	// shuffle temp_map_1d
	for (int i = 0; i < (MAP_SIZE - 1) * (MAP_SIZE - 1 * 2); i++)
	{
		int randNum = rand() % ((MAP_SIZE - 1) * (MAP_SIZE - 1) - 1) + 1;
		temp = temp_map_1d[0];
		temp_map_1d[0] = temp_map_1d[randNum];
		temp_map_1d[randNum] = temp;
	}

	// temp_map_1d to temp_map
	for (int i = 0; i < (MAP_SIZE - 1) * (MAP_SIZE - 1); i++)
	{
		*(map + i) = temp_map_1d[i];
	}
}

void set_map_1(int map[][MAP_SIZE])
{
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
			map[i][j] = 1;
	}
}