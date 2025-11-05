/*
 * buttons.h
 *
 *  Created on: Nov 4, 2025
 *      Author: cmb
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "stm32h5xx_hal.h"

#define SELECT_BUTTON_PORT 	GPIOA
#define SELECT_BUTTON_PIN 	GPIO_PIN_0

#define BACK_BUTTON_PORT 	GPIOA
#define BACK_BUTTON_PIN		GPIO_PIN_0

#define ENC_A_PORT			GPIOA
#define ENC_A_PIN			GPIO_PIN_0
#define ENC_B_PORT			GPIOA
#define ENC_B_PIN			GPIO_PIN_0

#define readA HAL_GPIO_ReadPin(ENC_A_PORT,ENC_A_PIN)
#define readB HAL_GPIO_ReadPin(ENC_B_PORT,ENC_B_PIN)


enum encMovement{
	NONE,	// no movement
	CW,		// Clockwise Movement
	CCW		// Counter Clockwise Movement
};

unsigned short selectPressed = 0;
unsigned short backPressed = 0;
enum encMovement encoderState = NONE;


void selectButtonCallback();

void backButtonCallback();

void ENCACallback();

void ENCBCallback();


#endif /* BUTTONS_H_ */
