#ifndef _CONIO_H
#define _CONIO_H
#include <stdio.h>
#include <unistd.h> 
/* Attributes */
#define RESETATTR 0
#define BRIGHT 1
#define DIM 2
#define UNDERSCORE 4
#define BLINK 5 /* May not work on all displays. */
#define REVERSE 7
#define HIDDEN 8

/* Colors for text and background */
#define BLACK 0x0
#define RED 0x1
#define GREEN 0x2
#define BROWN 0x3
#define BLUE 0x4
#define MAGENTA 0x5
#define CYAN 0x6
#define LIGHTGREY 0x7

#define DARKGREY 0x10
#define LIGHTRED 0x11
#define LIGHTGREEN 0x12
#define YELLOW 0x13
#define LIGHTBLUE 0x14
#define LIGHTMAGENTA 0x15
#define LIGHTCYAN 0x16
#define WHITE 0x17

#define hidecursor() puts("\033[?25l")
#define showcursor() puts("\033[?25h")
#define clear() puts("\e[1;1H\e[2J") // Limpiar pantalla
#define gotoxy(x, y) printf("\033[%d;%dH", y, x)
//#define bgcolor(c, s) printf("\033[%dm" s, c ? c + 40 : 0)


#define textattr(attr) __set_gm(attr, 0, 0)
#define textcolor(color) __set_gm(RESETATTR, color, 30)
#define textbackground(color) __set_gm(RESETATTR, color, 40)
void __set_gm(int attr, int color, int val)
{
    if (!color)
        printf("\e[%dm", attr);
    else
        printf("\e[%d;%dm", color & 0x10 ? 1 : 0, (color & 0xF) + val);
}

/*
#include <fcntl.h>
#include <termios.h>

// Captura de teclado
static int _initialized_ = 0;
void init_kbhit()
{
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
    _initialized_ = 1;
}

int kbhit()
{
    if (!_initialized_)
    {
        init_kbhit();
    }
    int ch = getchar();
    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

char btn()
{
    if (!_initialized_)
    {
        init_kbhit();
    }
    char a, b;
    a = getc(stdin);
    if (a == 27 && kbhit() && getc(stdin) == 91)
    {
        b = getc(stdin);
        a = b - 27;
    }
    while (kbhit())
        getc(stdin);
    return a;
}
*/
#endif