/*
 * UI.c
 *
 *  Created on: Jan 20, 2026
 *      Author: cmb
 */

#include "UI.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "fonts.h"

#define font3Height 20
#define initialOffset 30

const menuPage_t mainPage = {
		.name = "MAIN",
		.backState = MAIN,
		.numOptions = 3,
		.options = {
				{.next = CONFIG, .name = "CONFIG"},
				{.next = SCHEDULE, .name = "SCHEDULE"},
				{.next = DISPENSE, .name = "DISPENSE"}
		}
};

const menuPage_t configPage = {
		.name = "CONFIG",
		.backState = MAIN,
		.numOptions = 3,
		.options = {
				{.next = MAIN, .name = "MAIN"},
				{.next = SCHEDULE, .name = "SCHEDULE"},
				{.next = DISPENSE, .name = "DISPENSE"}
		}
};

const menuPage_t schedulePage = {
		.name = "SCHEDULE",
		.backState = MAIN,
		.numOptions = 3,
		.options = {
				{.next = CONFIG, .name = "CONFIG"},
				{.next = MAIN, .name = "MAIN"},
				{.next = DISPENSE, .name = "DISPENSE"}
		}
};

const menuPage_t dispensePage = {
		.name = "DISPENSE",
		.backState = MAIN,
		.numOptions = 3,
		.options = {
				{.next = CONFIG, .name = "CONFIG"},
				{.next = SCHEDULE, .name = "SCHEDULE"},
				{.next = MAIN, .name = "MAIN"}
		}
};

const short numMenus = 4;
const menuPage_t *menus[] = {&mainPage,&configPage,&schedulePage,&dispensePage};

enum UISTATE currentState = MAIN;
enum UISTATE prevState = NONE;

short currentSelection = 0;

void UI_init(){
	currentSelection = 0;
	currentState = MAIN;
	prevState = NONE;
	drawScreen();
}


void changeState(UISTATE_t nextState){
	currentSelection = 0;
	prevState = currentState;
	currentState = nextState;
	drawScreen();
}

void handleSelect(){
	// verify in valid state
	if(currentState < 0){
		changeState(MAIN);
		return;
	}

	switch(currentState){
		case MAIN:
		case CONFIG:
		case SCHEDULE:
		case DISPENSE:
			changeState(menus[currentState]->options[currentSelection].next);
			break;
		default:
			break;
	}

}

void handleBack(){
	// verify in valid state
	if(currentState < 0){
		changeState(MAIN);
		return;
	}

	switch(currentState){
		case MAIN:
		case CONFIG:
		case SCHEDULE:
		case DISPENSE:
			changeState(menus[currentState]->backState);
			break;
		default:
			break;
	}
}

void handleEncoder(short dir){
	// verify in valid state
	if(currentState < 0){
		return;
	}

	switch(currentState){
		case MAIN:
		case CONFIG:
		case SCHEDULE:
		case DISPENSE:
			// increment or decrement current option
			if(dir){
				++currentSelection;
				if(currentSelection >= menus[currentState]->numOptions){
					currentSelection = 0;
				}
			}
			else{
				--currentSelection;
				if(currentSelection < 0){
					currentSelection = menus[currentState]->numOptions - 1;
				}
			}
			break;
		default:
			break;
	}
	drawCursor();
}

void drawScreen(){
	// verify in valid state
	if(currentState < 0){
		return;
	}
    ILI9341_FillScreen(BLACK);
	switch(currentState){
		case MAIN:
		case CONFIG:
		case SCHEDULE:
		case DISPENSE:
			// display name
			ILI9341_DrawText(menus[currentState]->name,FONT4,10,5,BLACK,WHITE);
			// display menu contents
			for(int i = 0; i < menus[currentState]->numOptions; i++){
				ILI9341_DrawText(menus[currentState]->options[i].name,FONT3,25,i*font3Height + initialOffset,BLACK,WHITE);
			}
			ILI9341_DrawChar('>',FONT3,10,initialOffset,BLACK,WHITE);
			break;
		default:
			break;
	}
}

void drawCursor(){
	// verify in valid state
	if(currentState < 0){
		return;
	}

	switch(currentState){
		case MAIN:
		case CONFIG:
		case SCHEDULE:
		case DISPENSE:
			ILI9341_DrawFilledRectangleCoord(0,initialOffset,25,menus[currentState]->numOptions*font3Height + initialOffset,BLACK);
			ILI9341_DrawChar('>',FONT3,10,initialOffset + font3Height * currentSelection,BLACK,WHITE);
			break;
		default:
			break;
	}
}
