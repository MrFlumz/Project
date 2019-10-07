/*
 *  Open HR20
 *
 *  target:     ATmega169 @ 4 MHz in Honnywell Rondostat HR20E
 *
 *  ompiler:    WinAVR-20071221
 *              avr-libc 1.6.0
 *              GCC 4.2.2
 *
 *  copyright:  2008 Dario Carluccio (hr20-at-carluccio-dot-de)
 *
 *  license:    This program is free software; you can redistribute it and/or
 *              modify it under the terms of the GNU Library General Public
 *              License as published by the Free Software Foundation; either
 *              version 2 of the License, or (at your option) any later version.
 *
 *              This program is distributed in the hope that it will be useful,
 *              but WITHOUT ANY WARRANTY; without even the implied warranty of
 *              MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *              GNU General Public License for more details.
 *
 *              You should have received a copy of the GNU General Public License
 *              along with this program. If not, see http:*www.gnu.org/licenses
 */

/*!
 * \file       adc.h
 * \brief      header file for adc.c, functions to control A/D Converter
 * \author     Dario Carluccio <hr20-at-carluccio-dot-de>
 * \date       15.03.2008
 * $Rev: 8 $
 */


/*****************************************************************************
*   Macros
*****************************************************************************/

#define ADC_LOW_BATT_LEVEL 2000	   //!< Level for LOW Batt warning 
                                   //!< \todo move to config.h

// hardware configuration 
#define ADC_TEMP_MUX       0x02    //!< ADC-Channel for Temp-Sensor (ADMUX)
#define ADC_UB_MUX         0x1E    //!< ADC-Channel for Ub          (ADMUX)
#define ADC_ACT_TEMP       PF3     //!< Bit to activate the TempSensor
#define ADC_ACT_TEMP_P     PORTF   //!< Prot of ADC_ACT_TEMP

//Tilføjet for PID controller suppport
#define AVERAGE_LEN			15
#define AVGS_BUFFER_LEN (4*8) // 4 per minute * 8

extern int16_t ring_buf_temp_avgs [AVGS_BUFFER_LEN];
extern uint8_t ring_buf_temp_avgs_pos;

#define c2temp(c) (c*2)
#define calc_temp(t) (((uint16_t)t)*50)   // result unit is 1/100 C
#define TEMP_MIN    c2temp (5)   // 5?C
#define TEMP_DEFAULT    c2temp (20) // 20?C
#define TEMP_MAX    c2temp (30) // 30?C


#if THERMOTRONIC==1
#define TEMP_CAL_OFFSET 380 // offset of calibration points [ADC units]
#else
#define TEMP_CAL_OFFSET 256 // offset of calibration points [ADC units]
#endif
#define TEMP_CAL_STEP 500 // step between 2 calibration points [1/100?C]
#define TEMP_CAL_N 7 // // No. Values

int16_t ring_average[];

#define temp_average (ring_average[TEMP_RING_TYPE])




#define TEMP_RING_TYPE 1

#define bat_average (ring_average[BAT_RING_TYPE])

#define BAT_RING_TYPE 0


	


/*****************************************************************************
*   Typedefs
*****************************************************************************/

/*****************************************************************************
*   Prototypes
*****************************************************************************/
void ADC_Init(void);                        // Init motor control

void ADC_Measure_Ub(void);                  // Sample battery voltage
void ADC_Measure_Temp(void);                // Sample temperature value

int update_average_temp(void);

bool     ADC_Get_Bat_isOk(void);            // Status of battery ok?
uint16_t ADC_Get_Temp_Val(void);            // Get temperature ADC value 0-1023
int16_t  ADC_Get_Temp_Degree(void);         // Get temperature in 1/100 Deg °C
uint16_t ADC_Get_Bat_Val(void);             // Get batteriy ADC value 0-1023
uint16_t ADC_Get_Bat_Voltage(void);         // Get batteriy voltage in mV

uint16_t ADC_Sample_Channel(uint8_t);
int32_t  ADC_Convert_To_Degree(uint16_t);
