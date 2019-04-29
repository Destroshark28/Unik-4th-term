#include <dos.h>
#include <conio.h>
#include <stdio.h>

void interrupt newInt9(...);					
void interrupt(*oldInt9)(...);					

void indicator(unsigned char mask);				
void blinking(void);							

int isResend = 1;								
int quitFlag = 0;								
int blinkingON = 0;								
int count = 0;									

void main() {
	clrscr();
	_disable();
	oldInt9 = getvect(9);						
	setvect(9, newInt9);						
	_enable();

	while (!quitFlag) {							
		if (blinkingON)	blinking();							
	}

	_disable();
	setvect(9, oldInt9);						
	_enable();

	return;
}

void interrupt newInt9(...) {
	unsigned char value = 0;
	value = inp(0x60);							
	if (value == 0x01) quitFlag = 1;			

	if (value == 0x10 && blinkingON == 0){       
		blinkingON = 1;
	} else if (value == 0x10 && blinkingON == 1) { 
		blinkingON = 0;
	}
	
	if (value == 0xFE && blinkingON == 1) {		
		isResend = 1;							
		count++;
		if (count > 3) {
			printf("\n\nError!\n");
			quitFlag = 1;
			outp(0x20, 0x20);							
			return;
		}
	} else {
		isResend = 0;
	}
	
	printf("\t%x", value);
	outp(0x20, 0x20);							
}

void indicator(unsigned char mask) {
	isResend = 1;
	while (isResend) {							
		while (inp(0x64) & 0x02);		
		outp(0x60, 0xED);						
		delay(50);
	}

	isResend = 1;
	while (isResend) {							
		while (inp(0x64) & 0x02);		
		outp(0x60, mask);						
		delay(50);
	}
}

void blinking() {
	indicator(0x01);							
	delay(200);
	indicator(0x02);							
	delay(200);
	indicator(0x04);							
	delay(200);
}
