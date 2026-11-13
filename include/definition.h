#ifndef DEFINITION_H
#define DEFINITION_H

#include <stdbool.h>

#define MAX_LINE 20
#define MAX_COLUMN 50

#define CASE_WIDTH 35
#define CASE_HEIGHT 35

#define WINDOW_WIDTH (MAX_COLUMN * CASE_WIDTH)
#define WINDOW_HEIGHT (MAX_LINE * CASE_HEIGHT)

#define TOTAL_CELLS ((MAX_LINE) * (MAX_COLUMN))
#define RAW_NB_BOMBS (((TOTAL_CELLS) / 10) + ((TOTAL_CELLS) / 65))

#define NB_BOMBS                                                      \
	((TOTAL_CELLS) <= 1 ? 0                                           \
	 : (RAW_NB_BOMBS) < 1                                             \
			 ? 1                                                      \
			 : ((RAW_NB_BOMBS) >= (TOTAL_CELLS) ? ((TOTAL_CELLS) - 1) \
												: (RAW_NB_BOMBS)))

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
