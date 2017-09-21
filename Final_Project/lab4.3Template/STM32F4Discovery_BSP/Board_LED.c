#include "Board_LED.h"
#include "LED.h"


int32_t  LED_SetOut       (uint32_t val)
{
	LED_Out(val);
	return 0;
}

uint32_t LED_GetCount     (void)
{
	return LED_Num();
}