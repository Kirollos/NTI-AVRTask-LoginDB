/*
 * main.c
 *
 * Created: 26/10/2023 12:31:51 AM
 *  Author: Kirollos
 */ 

#define F_CPU 16000000
#include <util/delay.h>
#include <string.h>
#include "util.h"
#include "MCAL/PortInterface.h"
#include "HAL/Keypad.h"
#include "HAL/LCD/LCD.h"

#define MAX_LEN		32		/* Maximum length of Name and Password arrays */

typedef struct
{
	u32 ID;
	u8 Name[MAX_LEN];
	u8 Password[MAX_LEN];
} user;

user DB[] =
{
	{1234, "Ahmed Ali", "5060"},
	{5678, "Sara Omar", "1020"},
	{1999, "Kirollos N.", "14041999"}
};

int main(void)
{
	// Init our keypad and LCD
	Keypad_Init();
	H_LCD_void_Init();
	
	while(1)
	{
		H_LCD_void_clearScreen();							// Clear the screen
		H_LCD_void_sendString("Welcome!");					// Greeting messages
		H_LCD_void_gotoXY(1,0);
		H_LCD_void_sendString("Please login");
		H_LCD_void_gotoXY(3,0);
		H_LCD_void_sendString("Press # to continue.");
		H_LCD_void_gotoXY(2,0);
		H_LCD_void_sendString("User ID: ");
		u8 k = 0;
		u8 buff[MAX_LEN];
		u8* ptr = buff;
		while(k != '#')  // Hash key is our enter key.
		{
			k = Keypad_GetKey();
			if(k == '*') // delete char
			{
				if(ptr == buff) continue; // If no input was given at all
				u8 x,y;
				H_LCD_void_getXY(&x,&y);
				H_LCD_void_gotoXY(x, y-1);
				H_LCD_void_sendData(' ');
				H_LCD_void_gotoXY(x, y-1);
				ptr--;
				continue;
			}
			if(k != 0 && k != '#')	{
				H_LCD_void_sendData(k);
				*(ptr++) = k;
			}
		}
		*ptr = '\0';
		
		u32 ID = my_atoi(buff);
		u8 userfound = 0xff;
		for(u8 i = 0; i < sizeof(DB)/sizeof(user); i++)
		{
			if(DB[i].ID == ID)
			{
				userfound = i;
				break;
			}
		}
		H_LCD_void_gotoXY(0,0);
		if(userfound != 0xff)
		{
			H_LCD_void_clearScreen();
			H_LCD_void_sendString("Welcome!");
			H_LCD_void_gotoXY(3,0);
			H_LCD_void_sendString("Press # to continue.");
			H_LCD_void_gotoXY(1,0);
			H_LCD_void_sendString("Please enter your");
			H_LCD_void_gotoXY(2,0);
			H_LCD_void_sendString("Password: ");
		}
		else
		{
			H_LCD_void_gotoXY(2,0);
			H_LCD_void_sendString("User not found!");
			H_LCD_void_clearLine(3);
			H_LCD_void_gotoXY(3,0);
			H_LCD_void_sendString("Please wait");
			for(u8 i = 0; i < 5; i++)
			{
				H_LCD_void_sendData('.');
				_delay_ms(1000);
			}
			continue;
		}
		
		for(u8 i = 0; i < MAX_LEN; i++)
			buff[i] = 0;				// Clear the buffer
		
		ptr = buff;
		k = 0;
		while(k != '#')
		{
			k = Keypad_GetKey();
			if(k == '*') // delete char
			{
				if(ptr == buff) continue; // If no input was given at all
				u8 x,y;
				H_LCD_void_getXY(&x,&y);
				H_LCD_void_gotoXY(x, y-1);
				H_LCD_void_sendData(' ');
				H_LCD_void_gotoXY(x, y-1);
				ptr--;
				continue;
			}
			if(k != 0 && k != '#')	{
				H_LCD_void_sendData('*');
				*(ptr++) = k;
			}
		}
		*ptr = '\0';
		
		H_LCD_void_gotoXY(0,0);
		if(strcmp(buff, DB[userfound].Password) == 0) // Successful login
		{
			H_LCD_void_clearScreen();
			H_LCD_void_sendString("Welcome!");
			H_LCD_void_gotoXY(1,0);
			H_LCD_void_sendString("Hello ");
			H_LCD_void_sendString(DB[userfound].Name);
			
			H_LCD_void_gotoXY(3,0);
			H_LCD_void_sendString("Press * to log out.");
			while(Keypad_GetKey() != '*');					// Poll for the * key
			H_LCD_void_clearScreen();
			H_LCD_void_gotoXY(2,0);
			H_LCD_void_sendString("Logging out.");
			H_LCD_void_gotoXY(3,0);
			H_LCD_void_sendString("Please wait");
			for(u8 i = 0; i < 5; i++)
			{
				H_LCD_void_sendData('.');
				_delay_ms(1000);
			}
		}
		else
		{
			H_LCD_void_gotoXY(2,0);
			H_LCD_void_sendString("Incorrect password!");
			H_LCD_void_clearLine(3);
			H_LCD_void_gotoXY(3,0);
			H_LCD_void_sendString("Please wait");
			for(u8 i = 0; i < 5; i++)
			{
				H_LCD_void_sendData('.');
				_delay_ms(1000);
			}
			continue;
		}
	}
}