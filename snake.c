
int abs(int n)
{
	if (n < 0)
		return -n;
	else 
		return n;
}

struct snake_type * snake_create(int dir, float speed, char ht, char tt, int posx, int posy)
{
	struct snake_type * snake = (struct snake_type *) malloc(sizeof (struct snake_type));
	assert(snake != NULL);

	snake->dir = dir;
	snake->speed = speed;

	snake->head_type = ht;
	snake->tail_type = tt;

	snake->head = (struct coord *) malloc(sizeof (struct coord));
	assert(snake->head != NULL);

	snake->head->x = posx;
	snake->head->y = posy;
	snake->head->next = snake->head->prev = NULL;

	snake->tail = snake->head;

	return snake;
}

void snake_destroy(struct snake_type * snake)
{
	struct coord * curr = snake->tail;
	while (curr->prev != NULL) {
		curr = curr->prev;
		free((void *) curr->next);
	}
	free((void *) curr);
	free((void *) snake);
}

void snake_raise(struct snake_type * snake)
{
	snake->tail->next = (struct coord *) malloc(sizeof (struct coord));
	snake->tail->next->prev = snake->tail;
	snake->tail = snake->tail->next;
	snake->tail->next = NULL;
}

void snake_chop(struct snake_type * snake, struct coord * begin)
{
	while (snake->tail != begin) {
		snake->tail = snake->tail->prev;
		free((void *) snake->tail->next);
		snake->tail->next = NULL;
	}
	snake->tail = snake->tail->prev;
	free((void *) snake->tail->next);
	snake->tail->next = NULL;
}

void snake_draw(struct snake_type * snake)
{
	move(snake->head->x, snake->head->y);
	addch(snake->head_type);

	struct coord * curr = snake->head->next;
	while (curr != NULL) {
		move(curr->x, curr->y);
		addch(snake->tail_type);

		curr = curr->next;
	}
}

void snake_lives(struct snake_type * snake, int maxx, int maxy)
{
	struct coord * curr = snake->tail;
	while (curr != snake->head) {
		curr->x = curr->prev->x;
		curr->y = curr->prev->y;

		curr = curr->prev;
	}

	switch (snake->dir) {
		case UP :
			snake->head->x -= snake->speed;
			if (snake->head->x < 0)
				snake->head->x = (maxx-1);
			break;
		case DOWN :
			snake->head->x += snake->speed;
			if (snake->head->x >= maxx)
				snake->head->x = 0;
			break;
		case LEFT :
			snake->head->y -= snake->speed;
			if (snake->head->y < 0)
				snake->head->y = (maxy-1);
			break;
		case RIGHT :
			snake->head->y += snake->speed;
			if (snake->head->y >= maxy)
				snake->head->y = 0;
			break;
		default :
			break;
	}
}

struct coord * snake_eat_itself(struct snake_type * snake)
{
	struct coord * curr = snake->head->next;

	while (curr != NULL) {
		if ((abs(snake->head->x - curr->x) < 1) && (abs(snake->head->y - curr->y) < 1))
			break;

		curr = curr->next;
	}

	return curr;
}

