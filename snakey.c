#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>

#define DISTANCE 10
#define BURGER 'B'

#include "snake.h"
#include "snake.c"

struct Game {
	int sleep;			// game delay in loop
	int screenx;			// number of lines
	int screeny;			// number of columns
	int snake_num;			// number of snakes
	int key_pressed;		// pressed key in game
	int burger;			// the number of eaten scores

	struct coord * food;
	struct snake_type ** snake;
};

int rand_num(int max)
{
	return 0 + (int)(max * (rand() / (RAND_MAX + 1.0)));
}

struct Game * game_init()
{
	initscr();
	noecho();
	nodelay(stdscr, true);
	keypad(stdscr, true);

	struct Game * game = (struct Game *) malloc(sizeof (struct Game));

	game->sleep = 5;
	game->snake_num = 1;
	getmaxyx(stdscr, game->screenx, game->screeny);

	game->snake = (struct snake_type **) \
	malloc(sizeof (struct snake_type *) * (game->snake_num));

	assert(game->snake != NULL);

	game->snake[ 0 ] = \
	snake_create(RIGHT, 1, 'O', 'o', (game->screenx / 2), (game->screeny/2 - DISTANCE));

	game->food = (struct coord *) malloc(sizeof (struct coord));
	game->food->x = rand_num(game->screenx);
	game->food->y = rand_num(game->screeny);
	game->burger = 0;

	return game;
}

void game_draw(struct Game * game)
{
	clear();
	int i;
	for (i = 0; i < game->snake_num; i++) {
		snake_draw(game->snake[ i ]);
	}

	move(game->food->x, game->food->y);
	addch(BURGER);

	move(0, 0);
	printw("You have eat %d burgers waaa ha, ha, ha! ", game->burger);
	printw("If you couldn't find burger then it may be here under this words.");

	refresh();
}

void game_sleep(struct Game * game)
{
	int i, j, s = game->sleep;
	while (s--)
		for (i = 0; i < 1000; i++)
			for (j = 0; j < 5000; j++);
}

void game_end(struct Game * game)
{
	int i;
	for (i = 0; i < game->snake_num; i++) {
		snake_destroy(game->snake[ i ]);
	}

	free((void *) game);

	endwin();
}

int main()
{
	int tos;
	srand((int)&tos);
	struct Game * game = game_init();

	do {
		int i;

		while ((tos = getch()) != -1)		// the clearing the buffer of keyboard
			game->key_pressed = tos;	// if there will be many chars then we'll get only last

		switch (game->key_pressed) {
			case KEY_UP :
				if (game->snake[ 0 ]->dir != DOWN)
					game->snake[ 0 ]->dir = UP;
				break;
			case KEY_DOWN :
				if (game->snake[ 0 ]->dir != UP)
					game->snake[ 0 ]->dir = DOWN;
				break;
			case KEY_LEFT :
				if (game->snake[ 0 ]->dir != RIGHT)
					game->snake[ 0 ]->dir = LEFT;
				break;
			case KEY_RIGHT :
				if (game->snake[ 0 ]->dir != LEFT)
					game->snake[ 0 ]->dir = RIGHT;
				break;
			default :
				break;
		}

		for (i = 0; i < game->snake_num; i++) {
			snake_lives(game->snake[ i ], game->screenx, game->screeny);
		}

		struct coord * snakep;
		getmaxyx(stdscr, game->screenx, game->screeny);

		for (i = 0; i < game->snake_num; i++) {

			snakep = snake_eat_itself(game->snake[ i ]);

			if (snakep)
				snake_chop(game->snake[ i ], snakep);

			if ((abs(game->snake[ i ]->head->x - game->food->x) < 1) && \
				(abs(game->snake[ i ]->head->y - game->food->y) < 1)) {
				snake_raise(game->snake[ i ]);
				game->burger++;

				game->food->x = rand_num(game->screenx);
				game->food->y = rand_num(game->screeny);
			}
		}

		game_draw(game);
		game_sleep(game);
	} while (game->key_pressed != 'Q');

	game_end(game);

	return 0;
}

