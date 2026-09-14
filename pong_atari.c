#include <conio.h>
#include <atari.h>
#include <string.h>
#include <peekpoke.h>
#include <unistd.h>

#define CLS clrscr()
#define CONSOLE_WIDTH 40
#define CONSOLE_HEIGHT 24
#define PADDLE_LENGTH 6
#define MAX_POINTS 10
#define PADDLE_MV_DISTANCE 2

#define true 1
#define false 0

#define L_PADDLE_UP 46
#define L_PADDLE_DOWN 62
#define R_PADDLE_UP 8
#define R_PADDLE_DOWN 0

enum direction
{
	LEFT, RIGHT, UP, DOWN
};

struct ball
{
	unsigned int x;
	unsigned int y;
	enum direction x_dir;
	enum direction y_dir;
};

void draw_paddle_segment(unsigned int x, unsigned int y)
{
	gotoxy(x, y);
	cputc('|');
}

void draw_ball(unsigned int x, unsigned int y)
{
	gotoxy(x, y);
	cputc('O');
}

void erase_pixel(unsigned int x, unsigned int y)
{
	gotoxy(x, y);
	cputc(' ');
}

void print_game_start()
{
	int caption_size = strlen("Start the game?");
	int yes_no_size = strlen("Yes  No");
	int margin_caption = (CONSOLE_WIDTH - caption_size) / 2;
	int margin_yes_no = (CONSOLE_WIDTH - yes_no_size) / 2;

	gotox(margin_caption);
	cprintf("Start the game?\r\n");
	gotox(margin_yes_no);
	cprintf("Yes  No\r\n");
}

void render_top_bar(char *n, char *s)
{
	int txt_width = strlen("Players: ") + strlen(n) + strlen(s);
	int l_margin = (CONSOLE_WIDTH - txt_width) / 2;

	CLS;
	chline(CONSOLE_WIDTH);

	gotox(l_margin);
	cprintf("Players: %s %s\r\n", n, s);

	chline(CONSOLE_WIDTH);
}

void erase_paddles(unsigned int l_pad_pos, unsigned int r_pad_pos)
{
	int i = l_pad_pos;
	int j = r_pad_pos;

	while (i < l_pad_pos + PADDLE_LENGTH)
	{
		erase_pixel(0, i);
		++i;
	}

	while (j < r_pad_pos + PADDLE_LENGTH)
	{
		erase_pixel(CONSOLE_WIDTH - 1, j);
		++j;
	}
}

void update_paddle_positions(unsigned int* l_paddle, unsigned int* r_paddle)
{
	unsigned char last_key = OS.ch;

	switch (last_key)
	{
		case L_PADDLE_UP:
			if (*l_paddle > 0) (*l_paddle) -= PADDLE_MV_DISTANCE;
			break;
	    case L_PADDLE_DOWN:
	    	if (*l_paddle < CONSOLE_HEIGHT - PADDLE_LENGTH) (*l_paddle) += PADDLE_MV_DISTANCE;
	    	break;
		case R_PADDLE_UP:
			if (*r_paddle > 0) (*r_paddle) -= PADDLE_MV_DISTANCE;
			break;
		case R_PADDLE_DOWN:
			if (*r_paddle < CONSOLE_HEIGHT - PADDLE_LENGTH) (*r_paddle) += PADDLE_MV_DISTANCE;
			break;
	}

	OS.ch = 255;
}

void draw_paddles(unsigned int l_pad_pos, unsigned int r_pad_pos)
{
	int i = l_pad_pos;
	int j = r_pad_pos;

	// Drawing left paddle
	while (i < l_pad_pos + PADDLE_LENGTH)
	{
		draw_paddle_segment(0, i);
		++i;
	}

	// Drawing right paddle
	while (j < r_pad_pos + PADDLE_LENGTH)
	{
		draw_paddle_segment(CONSOLE_WIDTH - 1, j);
		++j;
	}
}

void wait_vblank(void)
{
    unsigned char t = PEEK(0x14);
    while (PEEK(0x14) == t);
}


void update_ball_position(struct ball *b)
{
	switch (b->x_dir)
	{
	case LEFT:
		(b->x)--;
		break;
	case RIGHT:
		(b->x)++;
		break;
	}

	wait_vblank();

	switch (b->y_dir)
	{
	case UP:
		(b->y)--;
		break;
	case DOWN:
		(b->y)++;
		break;
	}
}

void reset_ball_position(struct ball *b)
{
	b->x = CONSOLE_WIDTH / 2;
	b->y = CONSOLE_HEIGHT / 2;
	b->y_dir = (b->y_dir == UP) ? DOWN : UP;
}

void bounce_ball(struct ball *b)
{
	if (b->y == 0 || b->y == CONSOLE_HEIGHT - 1)
		b->y_dir = (b->y_dir == UP) ? DOWN : UP;
}

void update_score(struct ball *b, unsigned int* p_one_score, unsigned int* p_two_score)
{
	if (b->x == 0)
	{
		(*p_two_score)++;
		reset_ball_position(b);
	}
	else if (b->x == CONSOLE_WIDTH - 1)
	{
		(*p_one_score)++;
		reset_ball_position(b);
	}
}

void announce_winner(char *name)
{
	int txt_width = strlen("Player  is the winner") + strlen(name);
	int l_margin = (CONSOLE_WIDTH - txt_width) / 2;

	chline(CONSOLE_WIDTH);
	gotox(l_margin);
	cprintf("Player %s is the winner!\r\n", name);
	chline(CONSOLE_WIDTH);
}

void check_paddle_collision(struct ball *b, unsigned int l_p_y, unsigned int r_p_y)
{
	if (b->x == 1 && (b->y >= l_p_y && b->y < (l_p_y + PADDLE_LENGTH)) || b->x == CONSOLE_WIDTH - 2 && (b->y >= r_p_y && b->y < (r_p_y + PADDLE_LENGTH)))
		b->x_dir = (b->x_dir == LEFT) ? RIGHT : LEFT;
}

int main(void)
{
	char player_one[25];
	char player_two[25];
	char start_answer[10];
	int game_running = false;

	unsigned int player_one_score = 0;
	unsigned int player_two_score = 0;

	unsigned int left_paddle_y = (CONSOLE_HEIGHT - PADDLE_LENGTH) / 2;
	unsigned int right_paddle_y = (CONSOLE_HEIGHT - PADDLE_LENGTH) / 2;

	struct ball ball;

	unsigned int prev_left_y = left_paddle_y;
	unsigned int prev_right_y = right_paddle_y;
	unsigned int prev_ball_x;
	unsigned int prev_ball_y;

	// Instantiate the ball object
	ball.x = CONSOLE_WIDTH / 2;
	ball.y = CONSOLE_HEIGHT / 2;
	ball.x_dir = LEFT;
	ball.y_dir = DOWN;

	prev_ball_x = ball.x;
	prev_ball_y = ball.y;


	CLS;

    cprintf("Enter player one: "); cgets(player_one, 25);
    cprintf("\rPlayer 1: %s\r\n", player_one);

    cprintf("Enter player two: "); cgets(player_two, 25);
    cprintf("\rPlayer 2: %s\r\n", player_two);

    sleep(3);

    render_top_bar(player_one, player_two);

    cprintf("\r\n\r\n\r\n");
    print_game_start();

    cprintf("\r\n");
    gotox((CONSOLE_WIDTH - (strlen("Yes") + strlen("Answer: "))) / 2);
    cprintf("Answer: ");
    cgets(start_answer, 10);

    if (strcmp(start_answer, "Yes") == 0)
    	game_running = true;
    else
    	__asm__("jmp ($e474)");

    CLS;

    left_paddle_y++;
    right_paddle_y++;

    while (game_running)
    {
    	wait_vblank();

    	if (player_one_score >= MAX_POINTS || player_two_score >= MAX_POINTS)
    		game_running = false;

    	update_paddle_positions(&left_paddle_y, &right_paddle_y);
    	if (left_paddle_y != prev_left_y || right_paddle_y != prev_right_y)
    	{
    	    erase_paddles(prev_left_y, prev_right_y);
    	    draw_paddles(left_paddle_y, right_paddle_y);
    	    prev_left_y = left_paddle_y;
    	    prev_right_y = right_paddle_y;
    	}

    	bounce_ball(&ball);
    	update_ball_position(&ball);
    	check_paddle_collision(&ball, left_paddle_y, right_paddle_y);
    	update_score(&ball, &player_one_score, &player_two_score);

    	if (ball.x != prev_ball_x || ball.y != prev_ball_y)
    	{
    		erase_pixel(prev_ball_x, prev_ball_y);
    		draw_ball(ball.x, ball.y);
    		prev_ball_x = ball.x;
    		prev_ball_y = ball.y;
    	}
    }

    CLS;

    if (player_one_score > player_two_score)
    	announce_winner(player_one);
    else
    	announce_winner(player_two);

    cprintf("\r\nFinal score:\r\n");
    cprintf("%s: %d\r\n", player_one, player_one_score);
    cprintf("%s: %d\r\n", player_two, player_two_score);

    while(1)
    	if (kbhit())
        	break;

    return 0;
}
