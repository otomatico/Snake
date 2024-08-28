#ifndef _JOYPAD_H
#define _JOYPAD_H
#include "conio.h"

typedef struct _joypad
{
    int (*event)();
    char (*key)();
} Joypad;

void initJoy(Joypad *this)
{
	this->event = kbhit;
	this->key = btn;
}

#define KEY_UP 38
#define KEY_DOWN 39
#define KEY_RIGHT 40
#define KEY_LEFT 41

#define KEY_ESC 27
#define KEY_PAUSE 32
#define KEY_ENTER 10
#define KEY_A 97
#define KEY_B 115
#endif
