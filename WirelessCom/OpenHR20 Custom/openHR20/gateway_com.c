/*
 * gateway_com.c
 *
 * Created: 25-09-2019 09:32:21
 *  Author: aksel
 */ 

#include <stdint.h>
#include <stdlib.h>
#include <avr/pgmspace.h>



#include "main.h"
#include "motor.h"
#include "controller.h"
#include "adc.h"



void handle_com_recieved(char* recieved)
{
	//If the first character is an s, we need to set a value on the thermostat
	
	if(recieved[0] == 's') 
	{
		//We have recieved a temperature target from the gateway, and need to set it.
		
		 //The new target temperature is read as an 8 bit signed value with the unit 0.5 degrees C
		char new_temp_target = recieved[1];
		
		if(new_temp_target != CTL_temp_wanted)
		{
			if ((new_temp_target > 10)&&(new_temp_target < 60))//Error protection
			{
				CTL_temp_change_inc(new_temp_target - CTL_temp_wanted);
			} //Todo: Inform the gateway that the temp target was wrong	
		}
	}
	
	//If the first character is a g, we need to send a value back to the gateway
	else if (recieved[0] == 'g')
	{
		int temp_celsius;
		int battery_coltage_mv;
		
		
		switch (recieved[1])
		{
		case 't':  //temperature read from HR20 temperature sensor
			temp_celsius = ADC_Get_Temp_Degree();  //Temperature in 1/100 Deg °C
			
			//Todo: Send value
		
			break;
		case 'b':  //Battery read from adc
			battery_coltage_mv = ADC_Get_Bat_Voltage();  // Get Battery Voltage in mV
		
			//Todo: Send value
					
			break;
		case 's':  //Setpoint (target temperature for PI control
			//CTL_temp_wanted;
		
		
			break;			
		}
		//MOTOR_GetPosPercent(void)
		
	}
	
}