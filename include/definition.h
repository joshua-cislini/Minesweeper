#ifndef DEFINITION_H
#define DEFINITION_H

#include <stdbool.h>

#define MAX_LINE 10
#define MAX_COLUMN 10

#define CASE_WIDTH 35
#define CASE_HEIGHT 35

#define WINDOW_WIDTH (MAX_COLUMN * CASE_WIDTH)
#define WINDOW_HEIGHT (MAX_LINE * CASE_HEIGHT)

#define NB_BOMBS 10

#define STATE_IDLE 0
#define STATE_RUN 1
#define STATE_WIN 2
#define STATE_LOST 3

typedef enum {
	eEmpty,
	eOne,
	eTwo,
	eThree,
	eFour,
	eFive,
	eSix,
	eSeven,
	eEight,
	eCase,
	eFlag,
	eBomb,
	eRedbomb,
	eNberOfTexture,
} listoftexture_t;

#define eRedBomb eRedbomb
#define eCaseButton eCase
#define NB_TEXTURES eNberOfTexture

typedef struct {
	bool bomb;
	bool open;
	bool flag;
} case_t;

#endif // DEFINITION_H
