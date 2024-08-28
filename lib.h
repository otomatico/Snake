#ifndef _LIB_BASE_H
#define _LIB_BASE_H
// Tipo de Dato
typedef enum _gamestate
{
	OPEN,
	GAME,
	PAUSE,
	OVER,
	EXIT
} EnumGameState;

typedef struct _point
{
	int x;
	int y;
} Point2D;
#endif