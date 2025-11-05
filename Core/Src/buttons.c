/*
 * buttons.c
 *
 *  Created on: Nov 4, 2025
 *      Author: cmb
 */

#include "buttons.h"

void selectButtonCallback(){
	selectPressed = 1;
}

void backButtonCallback(){
	backPressed = 1;
}

// called on rising A transition
// Assume A is high
void ENCACallback(){
	// check to see if B is already high
	if(readB)
		encoderState = CCW;
	else
		encoderState = CW;
}

// called on rising B transition
// Assume B is high
void ENCBCallback(){
	// check to see if A is already high
	if(readA)
		encoderState = CW;
	else
		encoderState = CCW;
}
