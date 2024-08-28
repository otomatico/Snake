#ifndef _TDA_H
#define _TDA_H
#include <stdlib.h>
#include "lib.h"
/**
 * TAD (Tipo Abstrato de Dato) Pilas, ~Listas~ y Colas
 * Esta es la forma correcta de definir un NODO,
 *  no re-invente que luego el compilador se queja que estas queriendo meter Macintosh en Apple
 **/
typedef struct _link Node;
struct _link
{
	Point2D value;
	Node *next;
};
// Lo mismo dicho en Nodo ahora para Pilas
typedef struct _stack
{
	Node *first;
} Stack;

typedef struct _tail
{
	Node *first;
	Node *last;
} Tail;
typedef struct _list
{
	Node *first;
	Node *current;
	int Length;
} List;
// Node Method
int isEmpty(Node *this)
{
	return this == NULL;
}
Point2D look(Node *this)
{
	return this->value;
}
int any(Node *this, Point2D point)
{
	int exist = 0;
	Point2D head;
	while (this != NULL && !exist)
	{
		head = look(this);
		exist = (head.x == point.x && head.y == point.y);
		this = this->next;
	}
	return exist;
}
Node *createNode(Point2D value)
{
	Node *newbie = (Node *)malloc(sizeof(Node));
	newbie->value = value;
	newbie->next = NULL;
	return newbie;
}
// Stack Method
int stackIsEmpty(Stack *this)
{
	return isEmpty(this->first);
}
Point2D stackLookaHead(Stack *this)
{
	return look(this->first);
}

void stackPush(Stack *this, Point2D value)
{
	Node *newbie = createNode(value);
	newbie->next = this->first;
	this->first = newbie;
}

Point2D stackPop(Stack *this)
{
	Point2D value;
	if (!isEmpty(this->first))
	{
		value = stackLookaHead(this);
		Node *erase = this->first;
		this->first = this->first->next;
		free(erase);
	}
	return value;
}

int stackInclude(Stack *this, Point2D point)
{
	Node *pointer = this->first;
	return any(pointer, point);
}
#define printPoint(A) printf("{x:%02d,y:%02d}\r\n",A.x,A.y);
void stackDelete(Stack *this, Point2D point)
{
	if(!stackIsEmpty(this)){
		Point2D head = stackPop(this);
		if(!(head.x == point.x && head.y ==point.y)){
			stackDelete(this,point);
			stackPush(this,head);
		}
	}
}
void destroyStack(Stack *this){
	while(!stackIsEmpty(this)){
		stackPop(this);
	}
}
// Tail Method
int tailIsEmpty(Tail *this)
{
	return isEmpty(this->first);
}
Point2D tailLookaHead(Tail *this)
{
	return look(this->last);
}

void tailPush(Tail *this, Point2D value)
{
	Node *newbie = createNode(value);
	if (this->first == NULL)
	{
		this->first = newbie;
	}
	if (this->last != NULL)
	{
		this->last->next = newbie;
	}
	this->last = newbie;
}

Point2D tailPop(Tail *this)
{
	Point2D head;
	if (!isEmpty(this->first))
	{
		head = tailLookaHead(this);
		Node *erase = this->first;
		this->first = this->first->next;
		free(erase);
	}

	return head;
}
int tailInclude(Tail *this, Point2D value)
{
	Node *pointer = this->first;
	return any(pointer, value);
}
void destroyTail(Tail *this){
	while(!tailIsEmpty(this)){
		tailPop(this);
	}
}
#endif