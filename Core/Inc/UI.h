/*
 * UI.h
 *
 *  Created on: Jan 20, 2026
 *      Author: cmb
 */

#ifndef UI_H_
#define UI_H_

#include "stm32h5xx_hal.h"
#include "buttons.h"

typedef enum UISTATE{
	NOSTATE = -1,
	MAIN = 0,
	CONFIG = 1,
	SCHEDULE = 2,
	DISPENSE = 3
}UISTATE_t;


typedef struct option{
	UISTATE_t next;
	char name[16];
}option_t;

typedef struct menuPage{
	char* name;
	UISTATE_t backState;
	unsigned short numOptions;
	option_t options[];
}menuPage_t;

extern UISTATE_t currentState;
extern UISTATE_t prevState;

/**
 * Handle changing between states
 * input: nextState - state to switch too
 */
void changeState(UISTATE_t nextState);

/**
 * Handle select input
 * To be called once per select button press
 */
void handleSelect();

/**
 * Handle back input
 * To be called once per back button press
 */
void handleBack();

/**
 * Handle Encoder input
 * Called once per encoder state change
 */
void handleEncoder(short dir);

/**
 * Redraws screen
 */
void drawScreen();

/**
 * Redraws Cursor on current menu
 */
void drawCursor();

#endif /* UI_H */
