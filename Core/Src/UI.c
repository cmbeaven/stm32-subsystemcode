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
short lastSelection = 0;

dosage_t* dosageList = 0;
unsigned short numDosages = 0;

void drawMenuPage(const menuPage_t*);
void menuPageHandleSelect(const menuPage_t*);
void menuPageHandleBack(const menuPage_t*);
void menuPageHandleEnc(const menuPage_t*, short dir);


void UI_init(){
	currentSelection = 0;
	currentState = MAIN;
	prevState = NONE;
	drawScreen();
}


void changeState(UISTATE_t nextState){
	if(nextState == DISPENSE){
		if(dosageList == 0){
			free(dosageList);
		}
		dosageList = getDosages(&numDosages);
	}
	lastSelection = currentSelection;
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
			menuPageHandleSelect(&mainPage);
			break;
		case CONFIG:
			menuPageHandleSelect(&configPage);
			break;
		case SCHEDULE:
			menuPageHandleSelect(&schedulePage);
			break;
		case DISPENSE:
			changeState(DOSAGEINFO);
			break;
		case DOSAGEINFO:
			changeState(MAIN);
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
			menuPageHandleBack(&mainPage);
			break;
		case CONFIG:
			menuPageHandleBack(&configPage);
			break;
		case SCHEDULE:
			menuPageHandleBack(&schedulePage);
			break;
		case DISPENSE:
			changeState(MAIN);
			break;
		case DOSAGEINFO:
			changeState(DISPENSE);
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
			menuPageHandleEnc(&mainPage,dir);
			break;
		case CONFIG:
			menuPageHandleEnc(&configPage,dir);
			break;
		case SCHEDULE:
			menuPageHandleEnc(&schedulePage,dir);
			break;
		case DISPENSE:
			if(dir){
				++currentSelection;
				if(currentSelection >= numDosages){
					currentSelection = 0;
				}
			}
			else{
				--currentSelection;
				if(currentSelection < 0){
					currentSelection = numDosages - 1;
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
			drawMenuPage(&mainPage);
			break;
		case CONFIG:
			drawMenuPage(&configPage);
			break;
		case SCHEDULE:
			drawMenuPage(&schedulePage);
			break;
		case DISPENSE:
			ILI9341_DrawText("Dosages",FONT4,10,5,BLACK,WHITE);
			// display menu contents
			for(int i = 0; i < numDosages; i++){
				ILI9341_DrawText(dosageList[i].name,FONT3,25,i*font3Height + initialOffset,BLACK,WHITE);
			}
			ILI9341_DrawChar('>',FONT3,10,initialOffset,BLACK,WHITE);
			break;
		case DOSAGEINFO:
			dosage_t* dose = dosageList + lastSelection;
			ILI9341_DrawText(dose->name,FONT4,10,5,BLACK,WHITE);
			// handle time
			char doseTime[6] = "  :  ";
			doseTime[0] = dose->hour/10 + '0';
			doseTime[1] = dose->hour%10 + '0';
			doseTime[3] = dose->minute/10 + '0';
			doseTime[4] = dose->minute%10 + '0';
			ILI9341_DrawText(doseTime,FONT4,25,font3Height + initialOffset,BLACK,WHITE);
			char pillAmount[5] = "p : ";
			for(int i = 0; i < 4; ++i){
				pillAmount[1] = i + '0';
				pillAmount[3] = dose->pillAmounts[i] + '0';
				ILI9341_DrawText(pillAmount,FONT4,25,(1+i)*font3Height + initialOffset,BLACK,WHITE);
			}


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

void drawMenuPage(const menuPage_t* this){
	// display name
	ILI9341_DrawText(this->name,FONT4,10,5,BLACK,WHITE);
	// display menu contents
	for(int i = 0; i < this->numOptions; i++){
		ILI9341_DrawText(this->options[i].name,FONT3,25,i*font3Height + initialOffset,BLACK,WHITE);
	}
	ILI9341_DrawChar('>',FONT3,10,initialOffset,BLACK,WHITE);
}

void menuPageHandleSelect(const menuPage_t* this){
	changeState(this->options[currentSelection].next);
}

void menuPageHandleBack(const menuPage_t* this){
	changeState(this->backState);
}

void menuPageHandleEnc(const menuPage_t* this,short dir){
	// increment or decrement current option
	if(dir){
		++currentSelection;
		if(currentSelection >= this->numOptions){
			currentSelection = 0;
		}
	}
	else{
		--currentSelection;
		if(currentSelection < 0){
			currentSelection = this->numOptions - 1;
		}
	}
}

dosage_t* getDosages(unsigned short* num){
	// returning a set dosages
	// 4 different dosages
	*num = 4;
	dosage_t* dosages = malloc(sizeof(dosage_t)*(*num));

	dosage_t* dose = dosages;
	dose->name[0] = 'm';
	dose->name[1] = 'o';
	dose->name[2] = 'r';
	dose->name[3] = 'n';
	dose->name[4] = 'i';
	dose->name[5] = 'n';
	dose->name[6] = 'g';
	dose->name[7] = '\0';
	dose->hour = 6;
	dose->minute = 0;
	dose->pillAmounts[0] = 1;
	dose->pillAmounts[1] = 0;
	dose->pillAmounts[2] = 0;
	dose->pillAmounts[3] = 1;

	dose = dosages + 1;
	dose->name[0] = 'n';
	dose->name[1] = 'o';
	dose->name[2] = 'o';
	dose->name[3] = 'n';
	dose->name[4] = '\0';
	dose->hour = 12;
	dose->minute = 0;
	dose->pillAmounts[0] = 0;
	dose->pillAmounts[1] = 1;
	dose->pillAmounts[2] = 0;
	dose->pillAmounts[3] = 1;

	dose = dosages + 2;
	dose->name[0] = 'e';
	dose->name[1] = 'v';
	dose->name[2] = 'e';
	dose->name[3] = 'n';
	dose->name[4] = 'i';
	dose->name[5] = 'n';
	dose->name[6] = 'g';
	dose->name[7] = '\0';
	dose->hour = 15;
	dose->minute = 30;
	dose->pillAmounts[0] = 0;
	dose->pillAmounts[1] = 0;
	dose->pillAmounts[2] = 1;
	dose->pillAmounts[3] = 1;

	dose = dosages + 3;
	dose->name[0] = 'n';
	dose->name[1] = 'i';
	dose->name[2] = 'g';
	dose->name[3] = 'h';
	dose->name[4] = 't';
	dose->name[5] = '\0';
	dose->hour = 21;
	dose->minute = 0;
	dose->pillAmounts[0] = 0;
	dose->pillAmounts[1] = 0;
	dose->pillAmounts[2] = 0;
	dose->pillAmounts[3] = 1;

	return dosages;
}
