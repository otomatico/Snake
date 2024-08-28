#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "conio.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct _gfx
{
    int *buffer;
    int width;
    int height;
    int _initialized_;
} gfx;

// REFERENCE
void initGFX(gfx *this, int width, int height);
void clearGFX(gfx *this);
void drawGFX(gfx *this, int x, int y);
void putPixel(int x, int y, int color);
void destroy_gfx(gfx *this);
void color(int bg,int fr){
    printf("\e[%d;%dm", fr+30, bg+40);
}
// Implement
void initGFX(gfx *this, int width, int height)
{
    int size = width * height;
    this->width = width;
    this->height = height;
    this->buffer = (int *)malloc(sizeof(int) * size);
    this->_initialized_ = 1;
    clearGFX(this);
}
void clearGFX(gfx *this)
{
    if (!this->_initialized_)
        return;
    int size = this->width * this->height;
    for (int c = 0; c < size; c++)
    {
        this->buffer[c] = BLACK;
    }
}

void drawGFX(gfx *this, int x, int y)
{
    if (!this->_initialized_)
        return;
    int indexTop,indexBottom, top, bottom, row, col, half = this->height/2;
    textattr(RESETATTR);
    for(row=0; row<half;row++)
    for (col = 0; col < this->width; col++)
    {
        indexTop = (row*2)*this->width +col;
        indexBottom = (row*2+1)*this->width + col;
        gotoxy(col + x, row + y);
        top = this->buffer[indexTop];
        bottom = this->buffer[indexBottom];

        if(top==bottom){
            printf("\e[%d;%dm█", top+30, bottom+40);
        }else{
            printf("\e[%d;%dm▀", top+30, bottom+40);
        }
    }
    textattr(RESETATTR);
}
void putPixelGFX(gfx *this, int x, int y, int color)
{
    if (!this->_initialized_)
        return;
    if (0 <= x && x < this->width && 0 <= y && y < this->height)
    {
        this->buffer[y * this->width + x] = color;
    }
}
void destroy_gfx(gfx *this)
{
    if (!this->_initialized_)
        return;
    free(this->buffer);
}
#endif