#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // usleep
#include "lib.h"
#include "collection.h"
#include "joypad.h"
#include "graphics.h"

// Constantes
#define delay(a) usleep(a * 1000);

#define _FPS_ 2
#define BOARD_WIDTH  30
#define BOARD_HEIGHT 28
#define BOARD_EDGE_X 1
#define BOARD_EDGE_Y 1

#define BOARD_MARGIN_X 14 + BOARD_EDGE_X
#define BOARD_MARGIN_Y 4 + BOARD_EDGE_Y

#define SCREEN_WIDTH 60
#define SCREEN_HEIGHT 20

typedef struct _Snake
{
	Tail tail;
	char lastKey;
} Player;

typedef struct _game
{
	Player snake;
	Stack fruit;
	EnumGameState state;
	int FPS;
	int score;
	int level;
	gfx buffer;
} Game;
// Initiaze Elements
void initPoint2D(Point2D *this)
{
	this->x = rand() % BOARD_WIDTH;
	this->y = rand() % BOARD_HEIGHT;
}

void initTail(Tail *this)
{
	for (int x = 4; x < 7; x++)
	{
		tailPush(this, (Point2D){x, 4});
	}
}

void initPlayer(Player *this)
{
	initTail(&(this->tail));
	this->lastKey = KEY_RIGHT;
}

int fibonachi(int n)
{
	if (n > 2)
	{
		return fibonachi(n - 1) + fibonachi(n - 2);
	}
	return n > 0 ? n : 0;
}

void initFruit(Stack *this, int level)
{
	int len = fibonachi(level);
	while (len >= 0)
	{
		Point2D newPositionFruit;
		initPoint2D(&newPositionFruit);
		// No esta incluido
		if (!stackInclude(this, newPositionFruit))
		{
			stackPush(this, newPositionFruit);
			len--;
		}
	}
}

// Draw Elements
void drawBar(int len, const char *_string)
{
	for (int col = 0; col < len; col++)
	{
		printf("%s", _string);
	}
}

void drawOpenning()
{
	color(GREEN,RED);
	//textcolor(GREEN);
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 0);	printf("╔══════════════════════════════╗");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 1);	printf("║   CSnake, Snake make in C    ║");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 2);	printf("║                              ║");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 3);	printf("║                              ║");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 4);	printf("║       Press any key          ║");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 5);	printf("╚══════════════════════════════╝");
}

void drawBackground()
{
	gotoxy(1, 1);
	textcolor(BROWN);
	for (int r = 0; r < SCREEN_HEIGHT; r++)
	{
		drawBar(SCREEN_WIDTH, "░");
		printf("\n");
	}
	gotoxy(1, 1);
}

void drawBoard(gfx *this)
{
	//drawGFX(this, 1, 1);
	//return;
	int row;
	textcolor(WHITE);
	gotoxy(BOARD_MARGIN_X - 1, BOARD_MARGIN_Y - 1);
	printf("╓");
	drawBar(BOARD_WIDTH , "─");
	printf("┐");
	for (row = 0; row < BOARD_HEIGHT / 2; row++)
	{
		gotoxy(BOARD_MARGIN_X - 1, BOARD_MARGIN_Y + row);
		printf("║");
		gotoxy(BOARD_MARGIN_X + BOARD_WIDTH , BOARD_MARGIN_Y + row);
		printf("│");
	}
	gotoxy(BOARD_MARGIN_X - 1, BOARD_MARGIN_Y + row);
	printf("╚");
	drawBar(BOARD_WIDTH , "═");
	printf("╛");
	drawGFX(this, BOARD_MARGIN_X, BOARD_MARGIN_Y);
}

void drawTail(Tail *this, gfx *buffer)
{
	Point2D board = {BOARD_MARGIN_X, BOARD_MARGIN_Y};
	Node *pointer = this->first;
	Point2D head;
	textcolor(GREEN);
	while (pointer != NULL)
	{
		head = look(pointer);
		putPixelGFX(buffer,head.x,head.y,GREEN);
		pointer = pointer->next;
	}
}

void drawFruit(Stack *this, gfx *buffer)
{
	Point2D board = {BOARD_MARGIN_X, BOARD_MARGIN_Y};
	Node *pointer = this->first;
	Point2D head;
	textcolor(RED);
	while (pointer != NULL)
	{
		head = look(pointer);
		putPixelGFX(buffer,head.x,head.y,RED);
		pointer = pointer->next;
	}
}

void drawPause()
{
	textattr(RESETATTR);
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 0);	printf("╔══════════════════════════════╗");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 1);	printf("║          Game Pause          ║");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 2);	printf("║                              ║");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 3);	printf("║      Press [ESC]  Exit       ║");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 4);	printf("║    Press [Intro] Reboot      ║");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 5);	printf("║    Press [Space] continue    ║");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 6);	printf("╚══════════════════════════════╝");
}

void drawGameOver()
{
	textcolor(DARKGREY);
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 0);	printf("╔══════════════════════════════╗");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 1);	printf("║          Game Over           ║");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 2);	printf("║                              ║");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 3);	printf("║      Press any continue      ║");
	gotoxy(BOARD_MARGIN_X , BOARD_MARGIN_Y + 4);	printf("╚══════════════════════════════╝");
}

// BordGame
int insideBoard(Point2D point)
{
	return (0 <= point.x && point.x < BOARD_WIDTH) && (0 <= point.y && point.y < BOARD_HEIGHT);
}

// Player Method
int collision(Player *player, Stack *enemies)
{
	Point2D head = tailLookaHead(&(player->tail));
	// Esta dentro del mapa y no se ha chocado consigo mismo
	if (insideBoard(head))// && !tailInclude(&(player->tail), head))
	{
		return stackInclude(enemies, head); // se ha chocado con una fruta
	}
	return -1; // Se ha salido del mapa
}

int isKeyValid(char key)
{
	int keyValid[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};
	int valid = 0;
	for (int index = 0; index < 4 && !valid; index++)
	{
		valid = (key == keyValid[index]);
	}
	return valid;
}

void movePlayer(Player *this, char key)
{
	Point2D head = tailLookaHead(&(this->tail));

	if (!isKeyValid(key))
	{
		key = this->lastKey;
	}

	switch (key)
	{
	case KEY_LEFT:
		head.x--;
		break;
	case KEY_RIGHT:
		head.x++;
		break;
	case KEY_DOWN:
		head.y++;
		break;
	case KEY_UP:
		head.y--;
		break;
	}
	this->lastKey = key;

	tailPush(&(this->tail), head);
}

// Game Method
void initGame(Game *this)
{
	srand(time(NULL));
	this->state = OPEN;
	this->FPS = _FPS_;
	this->score = 0;
	this->level = 0;
	initGFX(&this->buffer,BOARD_WIDTH,BOARD_HEIGHT);
	clear();
}

void drawGameScore(Game *this)
{
	textattr(RESETATTR);
	textbackground(WHITE);
	textcolor(DARKGREY);
	//▀ ▄ █ ▌ ▐ └┘│╜╖║ ┌
	gotoxy(1, 1);printf(" █▀▀ █▌ █  █▀█  █ ▄▀ █▀▀   ┌─────────────────────────────╖  ");
	gotoxy(1, 2);printf(" ▀▀█ █▀▄█ ▐█▄█▌ ██▄  █▀    │ SCORE: %04d      LEVEL: %03d ║  ", this->score, this->level);
	gotoxy(1, 3);printf(" ▀▀▀ ▀  ▀ ▀▀ ▀▀ ▀  ▀ ▀▀▀   └─────────────────────────────╜  ");
}

void drawGame(Game *this)
{
	// en lugar de clear(), esto evita parpadeos
	gotoxy(1, 1);
	drawBackground();
	switch (this->state)
	{
	case OPEN:
		drawOpenning();
		break;
	default:
		drawGameScore(this);
		drawTail(&this->snake.tail,&this->buffer);
		drawFruit(&this->fruit,&this->buffer);
		drawBoard(&this->buffer);
		if (this->state == OVER)
		{
			drawGameOver();
		}
		if (this->state == PAUSE)
		{
			drawPause();
		}
	}
}

void moveGame(Game *this, int hit, char key)
{
	if (!hit)
	{
		tailPop(&(this->snake.tail));
	}else{
		Point2D head = tailLookaHead(&(this->snake.tail));
		stackDelete(&this->fruit, head);
		this->score+=50;	
		if (stackIsEmpty(&this->fruit))
		{
			initFruit(&(this->fruit), this->level++);
			this->FPS = _FPS_ + this->level;
		}
	}
	movePlayer(&(this->snake), key);
}

int updateGame(Game *this, Joypad *stick)
{
	int change = 0;
	switch (this->state)
	{
	case OPEN:

		if (stick->event() && stick->key())
		{
			this->level = 1;
			this->score = 0;
			initPlayer(&(this->snake));
			initFruit(&(this->fruit), this->level);
			this->state = GAME;
			change = 1;
		}
		break;
	case GAME:
		char key = '\0';
		change = 1;
		clearGFX(&this->buffer);
		if (stick->event())
		{
			key = stick->key();
		}
		if (key == KEY_PAUSE)
		{
			this->state = PAUSE;
		}
		else
		{
			int hit = collision(&this->snake, &this->fruit);
			if (hit < 0)
			{
				this->state = OVER;
			}
			else
			{
				moveGame(this, hit, key);
			}
		}
		break;
	case PAUSE:

		if (stick->event())
		{
			switch (stick->key())
			{
			case KEY_ESC:
				this->state = EXIT;
				break;
			case KEY_ENTER:
				this->state = OPEN;
				break;
			case KEY_PAUSE:
				this->state = GAME;
				break;
			}
			change = 1;
		}
		break;
	case OVER:
		if (stick->event())
		{
			destroyTail(&(this->snake.tail));
			destroyStack(&(this->fruit));
			this->state = OPEN;
			stick->key();
			change=1;
		}
		break;
	}
	return change;
}

void setup(Game *game, Joypad *stick)
{
	initJoy(stick);
	initGame(game);
}

int main()
{
	int hasChange = 1;
	Game Snake;
	Joypad Keyboard;
	hidecursor();
	setup(&Snake,&Keyboard);
	while (Snake.state != EXIT)
	{
		if (hasChange)
		{
			drawGame(&Snake);
		}
		hasChange = updateGame(&Snake, &Keyboard);
		delay(1000 / Snake.FPS);
	}
	showcursor();
	return 0;
}
