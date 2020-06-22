//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#define DIRECTION_KEY 224
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define PLAYER_1 0
#define PLAYER_2 1
#define BOTH_DIE 2

typedef struct Snake {
	int snake_x;
	int snake_y;
	struct Snake *next;
	struct Snake *prev;
}Snake;

typedef enum { right, up, left, down }SnakeDirection;

void OuterFrame(void);
void Initial(Snake *, Snake *, int, int, char, int);
void NewBody(int, int, Snake *);
int GoOrDie(void);
void ChangeDirection(int *, SnakeDirection *);							//in Go
void ChangeBody(Snake *, Snake *, Snake *, SnakeDirection *, int);		//in Go
int Star(int);
void GameOver(void);
void gotoxy(int, int);

Snake *head1, *current1, *rear1;
Snake *head2, *current2, *rear2;
SnakeDirection direction1, direction2, *direction;
int ifBody[100][25][2] = { 0 };

int main(void) {

	int final;

	OuterFrame();

	direction1 = right;
	direction2 = left;

	head1 = (Snake *)malloc(sizeof(Snake));
	rear1 = (Snake *)malloc(sizeof(Snake));
	head2 = (Snake *)malloc(sizeof(Snake));
	rear2 = (Snake *)malloc(sizeof(Snake));

	Initial(head1, rear1, 51, 12, 'o', PLAYER_1);
	Initial(head2, rear2, 49, 12, 'x', PLAYER_2);

	final = GoOrDie();

	Sleep(3000);
	gotoxy(0, 0);
	OuterFrame();
	gotoxy(45, 12);

	switch (final){

		case PLAYER_1:
			printf("player 1 win");
			break;
		case PLAYER_2:
			printf("player 2 win");
			break;
		case BOTH_DIE:
			printf("Both die");
	}

	Sleep(1500);
	gotoxy(45, 12);
	printf("             ");
	GameOver();
	Sleep(2000);
	return 0;
}


void OuterFrame(void) {				//outer frame y25*x100

	int frame1, frame2;

	for (frame1 = 0; frame1 < 100; frame1++) {
		printf("-");
	}
	printf("\n");
	for (frame1 = 0; frame1 < 23; frame1++) {
		printf("|");
		for (frame2 = 0; frame2 < 98; frame2++) {
			printf(" ");
		}
		printf("|\n");
	}
	for (frame1 = 0; frame1 < 100; frame1++) {
		printf("-");
	}
}

void Initial(Snake *head, Snake *rear, int x, int y, char ox, int player) {

	int bodyCounter;

	head->next = rear;
	rear->prev = head;
	rear->next = NULL;

	//6 body for snake 
	for (bodyCounter = 0; bodyCounter < 6; bodyCounter++) {
		NewBody(x, y, rear);
	}

	//initial snake
	gotoxy(head->next->snake_x, head->next->snake_y);
	printf("%c", ox);
	ifBody[x][y][player] = 5;
}

void NewBody(int x, int y, Snake *rear) {

	Snake *newBody = (Snake *)malloc(sizeof(Snake));

	newBody->snake_x = x;
	newBody->snake_y = y;

	newBody->next = rear;
	rear->prev->next = newBody;
	newBody->prev = rear->prev;
	rear->prev = newBody;
}

int GoOrDie(void) {

	int inputCount, input, *key;
	int final = 0;
	int fast = 100;

	while (1) {

		Sleep(fast);

		//change direction - player1(up/down/left/right) and player2(w/s/a/d)
		for (inputCount = 0; inputCount < 2; inputCount++) {
			key = &input;
			direction = &direction2;
			if (kbhit()) {
				*key = getch();
				if (*key == DIRECTION_KEY) {
					*key = getch();
					direction = &direction1;
				}
				ChangeDirection(key, direction);
			}
		}

		direction = &direction1;
		ChangeBody(head1, current1, rear1, direction, PLAYER_1);
		direction = &direction2;
		ChangeBody(head2, current2, rear2, direction, PLAYER_2);

		//end or continue
		current1 = head1->next;
		current2 = head2->next;

		if ((current1->snake_x == current2->snake_x) && (current1->snake_y == current2->snake_y)) {
			direction1 = (SnakeDirection)((direction1 + 2) % 4);		//�Ϥ�V�u�}
			direction2 = (SnakeDirection)((direction2 + 2) % 4);
		}
		else {
			if ((current1->snake_x == 0) || (current1->snake_x == 99) || (current1->snake_y == 0) || (current1->snake_y == 24)) {
				final = final + 1;
				gotoxy(current1->snake_x, current1->snake_y);
				printf("@");
			}
			if ((current2->snake_x == 0) || (current2->snake_x == 99) || (current2->snake_y == 0) || (current2->snake_y == 24)) {
				final = final + 2;
				gotoxy(current2->snake_x, current2->snake_y);
				printf("@");
			}
			if (ifBody[current1->snake_x][current1->snake_y][PLAYER_2]) {
				final = final + 10;
				gotoxy(current1->snake_x, current1->snake_y);
				printf("@");
			}
			if (ifBody[current2->snake_x][current2->snake_y][PLAYER_1]) {
				final = final + 20;
				gotoxy(current2->snake_x, current2->snake_y);
				printf("@");
			}
		}

		if (final) {
			if (!(final % 3)) {
				return BOTH_DIE;
			}
			else if (!((final + 1) % 3)) {
				gotoxy(current1->snake_x, current1->snake_y);
				printf("o");
				return PLAYER_1;
			}
			else {
				gotoxy(current2->snake_x, current2->snake_y);
				printf("x");
				return PLAYER_2;
			}
		}
		else {
			ifBody[current1->snake_x][current1->snake_y][PLAYER_1]++;
			gotoxy(current1->snake_x, current1->snake_y);
			printf("o");
			ifBody[current2->snake_x][current2->snake_y][PLAYER_2]++;
			gotoxy(current2->snake_x, current2->snake_y);
			printf("x");
		}

		fast = Star(fast);
	}
}

void ChangeDirection(int *key, SnakeDirection *direction) {

	switch (*key) {
		case UP:
		case 'w':
			*direction = up;
			break;
		case DOWN:
		case 's':
			*direction = down;
			break;
		case LEFT:
		case 'a':
			*direction = left;
			break;
		case RIGHT:
		case 'd':
			*direction = right;
	}
}

void ChangeBody(Snake *head, Snake *current, Snake *rear, SnakeDirection *direction, int player) {

	//space for rear
	current = rear->prev->prev;

	if (ifBody[current->snake_x][current->snake_y][player] == 1) {
		gotoxy(current->snake_x, current->snake_y);
		printf(" ");
	}
	ifBody[current->snake_x][current->snake_y][player]--;

	//change middle body
	current = current->next;

	while (current->prev != head) {
		current->snake_x = current->prev->snake_x;
		current->snake_y = current->prev->snake_y;
		current = current->prev;
	}

	//change head data
	switch (*direction) {

		case left:
			current->snake_x--;
			break;
		case right:
			current->snake_x++;
			break;
		case up:
			current->snake_y--;
			break;
		case down:
			current->snake_y++;
			break;
	}
}

int Star(int fast) {

	static int star[2] = { 51,12 };

	srand(time(NULL));

	if ((head1->next->snake_x == star[0]) && (head1->next->snake_y == star[1])) {
		NewBody(rear1->prev->snake_x, rear1->prev->snake_y, rear1);
		ifBody[rear1->prev->snake_x][rear1->prev->snake_y][PLAYER_1]++;
		gotoxy(rear1->prev->snake_x, rear1->prev->snake_y);
		printf("o");
		if (fast > 50)
			fast = fast - 5;
	}
	else if ((head2->next->snake_x == star[0]) && (head2->next->snake_y == star[1])) {
			NewBody(rear2->prev->snake_x, rear2->prev->snake_y, rear2);
			ifBody[rear2->prev->snake_x][rear2->prev->snake_y][PLAYER_2]++;
			gotoxy(rear2->prev->snake_x, rear2->prev->snake_y);
			printf("x");
			if (fast > 50)
				fast = fast - 5;
	}

	while (ifBody[star[0]][star[1]][PLAYER_1] || ifBody[star[0]][star[1]][PLAYER_2]) {
		star[0] = rand() % 98 + 1;
		star[1] = rand() % 23 + 1;
		gotoxy(star[0], star[1]);
		printf("*");
	}
	return fast;
}

void GameOver(void) {

	int i, j, k, m;

	//G
	gotoxy(0, 3);
	for (i = 0; i<2; i++) {
		printf("|   **********\n");
	}
	for (i = 0; i<7; i++) {
		printf("|   **\n");
	}
	for (i = 0; i<2; i++) {
		printf("|   **   *****\n");
	}
	for (i = 0; i<7; i++) {
		printf("|   **      **\n");
	}
	for (i = 0; i<2; i++) {
		printf("|   **********\n");
	}

	//A
	i = 15;
	m = 0;
	for (k = 0; k<2; k++) {
		for (j = 3; j<5; j++) {
			gotoxy(i, j + m);
			printf("**********\n");
		}
		for (j = 5; j<13; j++) {
			gotoxy(i, j + m);
			printf("**      **\n");
		}
		m = m + 10;
	}

	//M
	i = i + 11;
	j = 3;
	gotoxy(i, j++);
	printf("*        *\n");
	gotoxy(i, j++);
	printf("**      **\n");
	gotoxy(i, j++);
	printf("***    ***\n");
	gotoxy(i, j++);
	printf("****  ****\n");
	gotoxy(i, j++);
	printf("** **** **\n");
	gotoxy(i, j++);
	printf("**  **  **\n");
	for (j = 9; j<23; j++) {
		gotoxy(i, j);
		printf("**      **\n");
	}

	//E
	i = i + 11;
	m = 0;
	for (k = 0; k<2; k++) {
		for (j = 3; j<5; j++) {
			gotoxy(i, j + m);
			printf("**********\n");
		}
		for (j = 5; j<13; j++) {
			gotoxy(i, j + m);
			printf("**\n");
		}
		m = m + 9;
	}
	for (j = 3; j<5; j++) {
		gotoxy(i, j + m);
		printf("**********\n");
	}

	//O
	i = i + 15;
	for (j = 3; j<5; j++) {
		gotoxy(i, j);
		printf("**********\n");
	}
	for (j = 5; j<21; j++) {
		gotoxy(i, j);
		printf("**      **\n");
	}
	for (j = 21; j<23; j++) {
		gotoxy(i, j);
		printf("**********\n");
	}
	
	//V
	i = i + 11;
	for (j = 3; j<18; j++) {
		gotoxy(i, j);
		printf("**      **\n");
	}
	gotoxy(i, j++);
	printf("**      **\n");
	gotoxy(i, j++);
	printf(" **    ** \n");
	gotoxy(i, j++);
	printf("  **  **  \n");
	gotoxy(i, j++);
	printf("   ****   \n");
	gotoxy(i, j++);
	printf("    **    \n");

	//E
	i = i + 11;
	m = 0;
	for (k = 0; k<2; k++) {
		for (j = 3; j<5; j++) {
			gotoxy(i, j + m);
			printf("**********\n");
		}
		for (j = 5; j<13; j++) {
			gotoxy(i, j + m);
			printf("**\n");
		}
		m = m + 9;
	}
	for (j = 3; j<5; j++) {
		gotoxy(i, j + m);
		printf("**********\n");
	}

	//R
	i = i + 11;
	for (j = 3; j<5; j++) {
		gotoxy(i, j);
		printf("**********\n");
	}
	for (j = 5; j<14; j++) {
		gotoxy(i, j);
		printf("**      **\n");
	}
	for (j = 14; j<16; j++) {
		gotoxy(i, j);
		printf("**********\n");
	}
	for (j = 16; j<23; j++) {
		gotoxy(i, j);
		printf("**");
		for (k = 16; k<j; k++) {
			printf(" ");
		}
		printf("**\n");
	}
}

void gotoxy(int x, int y) {
	COORD loc;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	loc.X = x;
	loc.Y = y;
	SetConsoleCursorPosition(hOuput, loc);
}
