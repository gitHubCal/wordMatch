#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "Board_Buttons.h"
#include "Board_LED.h"
#include "stm32f4xx.h"
#include "microphone_functions.h"
#include "sound_loudness.h"
#include "led_timer.h"
#include "led_pwm.h"

//uint16_t array length for sound filter input buffer
#define AUDIO_BUF_SIZE 2048
float audio_buffer[AUDIO_BUF_SIZE];

char *sound[] = {"Cow","Duck","Geese","Dog","Cat","Pig"};
volatile uint32_t soft = 0;
volatile uint32_t medium = 0;
volatile uint32_t loud = 0; 

int main(void)
{
	// initialize
	SystemInit();
	Buttons_Initialize();
	LED_Initialize();	
	led_timer_init();
	// The functions setTIM4_PWMFrequency and setTIM4_PWMDuty are provided.
	
	microphone_init();
	microphone_start();
	// larger buffer into which data is copied from the array pointer returned by microphone_get_data_if_ready 	
	uint32_t audio_buffer_index = 0; // index variable
	float sound_loudness;
	
	while (1)
	{
		// read one array of samples from the microphone
	uint32_t n_samples; // number of samples read in one batch from the microphone
	uint16_t *audio_samples;
	while (1)
	{
		audio_samples = microphone_get_data_if_ready(&n_samples); // try to read an array of samples from the microphone
		if (audio_samples != 0) break;       // if array of samples was read
	}
	
	// copy array of samples from microphone into our array audio_buffer
	for(int i = 0; i < n_samples; i++){
			audio_buffer[audio_buffer_index ++] = (int16_t) audio_samples[i];
		  if (audio_buffer_index >= AUDIO_BUF_SIZE) break;
	}
	
	// calculate sound loudness and update brightness of LED
	if (audio_buffer_index >= AUDIO_BUF_SIZE)
	{
		 sound_loudness = calculate_sound_loudness(audio_buffer, AUDIO_BUF_SIZE);
      
			// use sound_loudness to control brightness of LED
			
		if(sound_loudness < 500) {
			setTIM4_PWMDuty(1,90);
			setTIM4_PWMDuty(2,0);
			setTIM4_PWMDuty(3,0);
			setTIM4_PWMDuty(4,0);
			soft++;
		}
		else if(sound_loudness > 10000) {
			setTIM4_PWMDuty(4,90);
			setTIM4_PWMDuty(1,0);
			setTIM4_PWMDuty(2,0);
			setTIM4_PWMDuty(3,0);
			loud++;
		}
		else {
			int var = 20 + 50*log(3.14) *((sound_loudness - 500)/(10000 - 500));
			setTIM4_PWMDuty(3,var); 
			medium++;
		}
		uint32_t softIndex = soft % 5;
		uint32_t mediumIndex = medium % 5;
		uint32_t loudIndex = loud % 5;
		printf("%s\n",sound[softIndex]);
		printf("%s\n",sound[mediumIndex]);
		printf("%s\n",sound[loudIndex]);
		
		audio_buffer_index  = 0;
		}
	}
}