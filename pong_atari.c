#include <conio.h>
#include <atari.h>
#include <string.h>
#include <peekpoke.h>

#define CLS clrscr()
#define CONSOLE_WIDTH 40
#define CONSOLE_HEIGHT 24
#define PADDLE_LENGTH 6

#define true 1
#define false 0

#define L_PADDLE_UP 46
#define L_PADDLE_DOWN 62
#define R_PADDLE_UP 8
#define R_PADDLE_DOWN 0

void draw_pixel(unsigned int x, unsigned int y)
{
	gotoxy(x, y);
	cprintf("X");
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
	int txt_width = strlen("Player: ") + strlen(n) + strlen(s);
	int l_margin = (CONSOLE_WIDTH - txt_width) / 2;

	CLS;
	chline(CONSOLE_WIDTH);

	gotox(l_margin);
	cprintf("Player: %s %s\r\n", n, s);

	chline(CONSOLE_WIDTH);
}

void update_paddle_positions(unsigned int* l_paddle, unsigned int* r_paddle)
{
	unsigned char last_key = OS.ch;

	switch (last_key)
	{
		case L_PADDLE_UP:
			if (*l_paddle > 0) (*l_paddle)--;
			break;
	    case L_PADDLE_DOWN:
	    	if (*l_paddle < CONSOLE_HEIGHT - PADDLE_LENGTH) (*l_paddle)++;
	    	break;
	    case R_PADDLE_UP:
	    	if (*r_paddle > 0) (*r_paddle)--;
	    	break;
	    case R_PADDLE_DOWN:
	    	if (*r_paddle < CONSOLE_HEIGHT - PADDLE_LENGTH) (*r_paddle)++;
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
		draw_pixel(0, i);
		++i;
	}

	// Drawing right paddle
	while (j < r_pad_pos + PADDLE_LENGTH)
	{
		draw_pixel(CONSOLE_WIDTH - 1, j);
		++j;
	}
}

int main(void)
{
	char name[25];
	char surname[25];
	char start_answer[10];
	int game_running = false;

	unsigned int left_paddle_y = 0;
	unsigned int right_paddle_y = 0;

	CLS;

    cprintf("Enter your name: "); cgets(name, 25);
    cprintf("\rYour name is: %s\r\n", name);

    cprintf("Enter your surname: "); cgets(surname, 25);
    cprintf("\rYour surname is: %s\r\n", surname);

    render_top_bar(name, surname);

    cprintf("\r\n\r\n\r\n");
    print_game_start();

    cprintf("\r\n");
    gotox((CONSOLE_WIDTH - (strlen("Yes") + strlen("Answer: "))) / 2);
    cprintf("Answer: ");
    cgets(start_answer, 10);

    if (strcmp(start_answer, "Yes") == 0)
    	game_running = true;

    while (game_running)
    {
    	update_paddle_positions(&left_paddle_y, &right_paddle_y);

    	// Drawing

    	CLS;
    	draw_paddles(left_paddle_y, right_paddle_y);

    	// End Drawing

    }

    return 0;
}
