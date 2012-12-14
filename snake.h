
#define MINIMAL_SPEED 0.1
#define INITIAL_SPEED 0.5
#define MAXIMAL_SPEED 1.0

/*
 * 'C' jaws	//you may eat part of snake
 * 's' speed	//you take more speed
 */

enum { UP, RIGHT, DOWN, LEFT };

struct coord {
	float x;
       	float y;

	struct coord * next;
	struct coord * prev;
};

struct food_type {
	char food_type;
	struct coord * pos;
};

struct snake_type {
	int dir;
	int len;

	float speed;

	char head_type;
	char tail_type;

	struct coord * head;
	struct coord * tail;
};

struct snake_type * snake_create(int, float, char, char, int, int);
void snake_destroy(struct snake_type *);
void snake_raise(struct snake_type *);
void snake_chop(struct snake_type *, struct coord *);
void snake_draw(struct snake_type *);

