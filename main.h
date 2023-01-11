
#ifndef __SNAKE_H__
#define __SNAKE_H__

#if defined(_WIN32) || defined(__CYGWIN__)
#include <curses.h>
#include <windows.h>
#define true 1
#define false 0
#else
#include <ncurses.h>
#endif

#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

typedef struct Snake
{
    int x;
    int y;
    short dirX;
    short dirY;
} Snake;

typedef struct Fruit
{
    int x;
    int y;
} Fruit;

int snake_menu();
int snake_game();
void init();
int update();
void draw_board();
void grow_snake();
void spawn_fruit();
void eat_fruit();
int move_snake();
int is_alive();
int ms_sleep(int miliseconds);

#endif