#include "Board_Buttons.h"
#include "Keyboard.h"

int32_t  Buttons_Initialize   (void)
{
	Keyboard_Initialize();
	return 0;
}

int32_t  Buttons_Uninitialize (void)
{
	Keyboard_Uninitialize();
	return 0;
}

uint32_t Buttons_GetState     (void)
{
	return Keyboard_GetKeys();
}

uint32_t Buttons_GetCount     (void)
{
	return Keyboard_NumKeys();
}