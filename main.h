
#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <ncurses.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
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