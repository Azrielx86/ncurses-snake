#include "main.h"

WINDOW* board;
WINDOW* w_game_over;
WINDOW* menu_mwin;
Snake* snake;
Fruit* fruit;
int initMaxSize;
int snakeSize;
int score;
int fruits;
int exit_game = 1;

char* game_over_txt[] = { "  ___    __    __  __  ____ ",
                         " / __)  /__\\  (  \\/  )( ___)",
                         "( (_-. /(__)\\  )    (  )__) ",
                         " \\___/(__)(__)(_/\\/\\_)(____)",
                         "  _____  _  _  ____  ____",
                         " (  _  )( \\/ )( ___)(  _ \\",
                         "  )(_)(  \\  /  )__)  )   /",
                         " (_____)  \\/  (____)(_)\\_)",
                         "",
                         "Press enter to exit..." };

int main(int argc, char const* argv[])
{
    initscr();
    curs_set(0);
    cbreak();
    noecho();
    nodelay(stdscr, true);
    keypad(stdscr, TRUE);
    start_color();
    use_default_colors();

    if (has_colors() == false)
    {
        perror("Terminal doesn't support colors");
        return 1;
    }

    init_pair(1, COLOR_GREEN, COLOR_GREEN);
    init_pair(2, COLOR_GREEN, COLOR_RED);

    int code = snake_game();
    free(snake);
    free(fruit);
    delwin(board);
    delwin(w_game_over);
    delwin(menu_mwin);

    endwin();
    
    return code;
}

int snake_game()
{
    int ch;
    init();
    while (update() == 0);
    if (is_alive() == 1)
    {
        int text_length = 30;
        int text_width = 12;
        w_game_over = newwin(text_width, text_length, (int)(LINES / 2) - (text_width / 2), (int)(COLS / 2) - (text_length / 2));
        box(w_game_over, 0, 0);
        refresh();

        for (int i = 0; i < ARRAY_SIZE(game_over_txt); i++)
        {
            mvwprintw(w_game_over, i + 1, 1, "%s", game_over_txt[i]);
        }

        wrefresh(w_game_over);

        while (ch != 10)
        {
            ch = getch();
        }
    }
    return 0;
}

void init()
{
    srand(time(NULL));
    board = newwin(LINES - 1, COLS - 1, 0, 0);
    box(board, 0, 0);
    mvwprintw(board, 0, (int)(COLS / 2) - 3, "Snake");
    refresh();
    wrefresh(board);

    fruit = malloc(sizeof(Fruit));
    spawn_fruit();

    initMaxSize = 100;
    snake = malloc(initMaxSize * sizeof(Snake));
    snakeSize = 1;

    snake[0].x = (int)COLS / 2;
    snake[0].y = (int)LINES / 2;
    snake[0].dirX = 1;
    snake[0].dirY = 0;

    for (int i = 1; i < 4; i++)
    {
        snake[i].x = snake[i - 1].x - 1;
        snake[i].y = snake[i - 1].y;
        snake[i].dirX = snake[i - 1].dirX;
        snake[i].dirY = snake[i - 1].dirY;
        snakeSize++;
    }

    score = 0;
    fruits = 0;
}

int update()
{
    int ch;

    switch (ch = getch())
    {
    case KEY_UP:
        if (snake[0].dirY == 1)
            break;
        snake[0].dirX = 0;
        snake[0].dirY = -1;
        break;
    case KEY_DOWN:
        if (snake[0].dirY == -1)
            break;
        snake[0].dirX = 0;
        snake[0].dirY = 1;
        break;
    case KEY_LEFT:
        if (snake[0].dirX == 1)
            break;
        snake[0].dirX = -1;
        snake[0].dirY = 0;
        break;
    case KEY_RIGHT:
        if (snake[0].dirX == -1)
            break;
        snake[0].dirX = 1;
        snake[0].dirY = 0;
        break;
    case (int)'q':
        return 1;
    case 27:
        return 1;
    default:
        break;
    }

    move_snake();
    eat_fruit();
    draw_board();
    ms_sleep(60);

    // TODO: Game over screen
    return is_alive();
}

void draw_board()
{
    wclear(board);
    box(board, 0, 0);

    #ifdef DEBUG
    mvwprintw(board, 2, 2, "Board Size: (%d, %d)", COLS - 2, LINES - 2);
    mvwprintw(board, 3, 2, "Fruit: (%d, %d)", fruit->x, fruit->y);
    mvwprintw(board, 4, 2, "Head: (%d, %d)", snake[0].x, snake[0].y);
    mvwprintw(board, 5, 2, "Size: %d", snakeSize);
    mvwprintw(board, 6, 2, "Max Size: %d", initMaxSize);
    wrefresh(board);
    #endif

    for (int i = 0; i < snakeSize; i++)
    {
        wattron(board, COLOR_PAIR(1));
        mvwprintw(board, snake[i].y, snake[i].x, "O");
        wattroff(board, COLOR_PAIR(1));
    }
    mvwprintw(board, 0, (int)(COLS / 2) - 3, "Snake");
    mvwprintw(board, LINES - 2, 2, "[Score: %-5d]", score);
    mvwprintw(board, LINES - 2, COLS - 18, "[Fruits: %-5d]", fruits);

    wattron(board, COLOR_PAIR(2));
    mvwprintw(board, fruit->y, fruit->x, "*");
    wattroff(board, COLOR_PAIR(2));

    wrefresh(board);
}

void grow_snake()
{
    if (snakeSize >= initMaxSize)
        snake = realloc(snake, initMaxSize += 50);

    snake[snakeSize].x = snake[snakeSize - 1].x - snake[snakeSize - 1].dirX;
    snake[snakeSize].dirX = snake[snakeSize - 1].dirX;
    snake[snakeSize].y = snake[snakeSize - 1].y - snake[snakeSize - 1].dirY;
    snake[snakeSize].dirY = snake[snakeSize - 1].dirY;
    snakeSize++;
}

void spawn_fruit()
{
    for (;;)
    {
        fruit->x = rand() % COLS - 2;
        fruit->y = rand() % LINES - 2;

        for (int i = 0; i < snakeSize; i++)
            if (fruit->x == snake[i].x || fruit->y == snake[i].y)
                continue;

        if (fruit->x > 0 && fruit->y > 0 && fruit->x < COLS && fruit->y < LINES)
            return;
    }
}

void eat_fruit()
{
    if (snake[0].x == fruit->x && snake[0].y == fruit->y)
    {
        grow_snake();
        spawn_fruit();
        score += 100;
        fruits += 1;
    }
}

int move_snake()
{

    for (int i = snakeSize; i > 0; i--)
    {
        snake[i].x += snake[i].dirX;
        snake[i].y += snake[i].dirY;
        snake[i].dirX = snake[i - 1].dirX;
        snake[i].dirY = snake[i - 1].dirY;
    }
    snake[0].x += snake[0].dirX;
    snake[0].y += snake[0].dirY;

    return 0;
}

int is_alive()
{
    if (snake[0].x == COLS - 2
        || snake[0].x == 0
        || snake[0].y == LINES - 2
        || snake[0].y == 0)
    {
        return 1;
    }

    for (int i = 1; i < snakeSize; i++)
        if (snake[i].x == snake[0].x && snake[i].y == snake[0].y)
            return 1;

    return 0;
}

int ms_sleep(int miliseconds)
{
    struct timespec time;
    int code;

    if (miliseconds < 0) return -1;

    time.tv_sec = miliseconds / 1000;
    time.tv_nsec = (miliseconds % 1000) * 1000000;

    do
    {
        code = nanosleep(&time, &time);
    } while (code && errno == EINTR);

    return code;
}
