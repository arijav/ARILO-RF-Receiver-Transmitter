/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             display_sm.c                                               */
/*                                                                              */
/* DESCRIPTION:      State machine that controls the display                    */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>
#include "display_sm.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Function Prototypes                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Functions                                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            init_display_sm                                             */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void init_display_sm(void)
{
	display_sm.statem.state = FIRST_SCREEN;
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            display_sm_main                                             */
/*                                                                              */
/* DESCRIPTION:     Main function from the Display State Machine                */
/*                                                                              */
/* PARAMETERS:      In: Event                                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void display_sm_main(eventtype const *event)
{
	switch (display_sm.statem.state)
	{
		case FIRST_SCREEN:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					//lcd_clrscr();
					//lcd_gotoxy(0,0);
					//lcd_puts("ARILO_TX");
				break;
				case INFO_RF_SIG:
				case INFO_NO_RF_SIG:
				case TEL_BATT_SIG:
				case TEL_RPM_SIG:
				case TEL_DIST_SIG:
				case TEL_CURR_SIG:
				case TEL_TEMPRX_SIG:
				case TEL_TEMPTX_SIG:
				case CTR_NCHANNELS_SIG:
				case DEBUG_SIG_1:
				case DEBUG_SIG_2:
				case DEBUG_SIG_3:
					display_info_ms((uint8_t) event->sig, (((SignalSM const *) event)->param));
				break;
				case INT_0:
				case INT_1:
					display_sm.statem.state = SECOND_SCREEN;
					display_sm_main(EXIT_STATE);
				break;
				default:
				break;
			};
		break;
		case SECOND_SCREEN:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					//lcd_clrscr();
					//lcd_gotoxy(0,0);
			        //lcd_puts("ACCELEROMETER{X,Y,Z}");
					//lcd_gotoxy(0,2);
			        //lcd_puts("GYROMETER{X,Y,Z}");
				break;
				case TEL_IMUACELX_SIG:
				case TEL_IMUACELY_SIG:
				case TEL_IMUACELZ_SIG:
				case TEL_IMUGYROX_SIG:
				case TEL_IMUGYROY_SIG:
				case TEL_IMUGYROZ_SIG:
					display_info_ss((uint8_t) event->sig, (((SignalSM const *) event)->param));
				break;
				case INT_0:
				case INT_1:
					display_sm.statem.state = THIRD_SCREEN;
					display_sm_main(EXIT_STATE);
				break;
				default:
				break;
			};
		break;
		case THIRD_SCREEN:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					//lcd_clrscr();
					//lcd_gotoxy(0,0);
					//lcd_puts("DEBUG_INFO");
				break;
				case DEBUG_SIG_1:
				case DEBUG_SIG_2:
				case DEBUG_SIG_3:
					display_info_ss((uint8_t) event->sig, (((SignalSM const *) event)->param));
				break;
				case INT_0:
				case INT_1:
					display_sm.statem.state = FIRST_SCREEN;
					display_sm_main(EXIT_STATE);
				break;
				default:
				break;
			};
		break;
		default:
		break;
	};
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            display_info _ms                                            */
/*                                                                              */
/* DESCRIPTION:     Displays the required information of the main screen        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void display_info_ms(uint8_t source, int32_t info)
{
	(void)info;
	//char cadena[10];
	switch(source)
	{
		case INFO_RF_SIG:
			/* Show information */
//			lcd_gotoxy(11,0);
//			lcd_puts("LINK:");
//			lcd_gotoxy(16,0);
//			lcd_puts("UP  ");
		break;
		case INFO_NO_RF_SIG:
			/* Show information */
//			lcd_gotoxy(11,0);
//			lcd_puts("LINK:");
//			lcd_gotoxy(16,0);
//			lcd_puts("DOWN");
		break;
		case TEL_BATT_SIG:
			/* Show information */
//			lcd_gotoxy(0,1);
//			lcd_puts("    ");
//			lcd_gotoxy(0,1);
//			lcd_puts(itoa((uint16_t)info,cadena,10));
//			lcd_gotoxy(4,1);
//			lcd_puts("[mV]");
		break;
		case TEL_RPM_SIG:
			/* Show information */
//			lcd_gotoxy(10,1);
//			lcd_puts("     ");
//			lcd_gotoxy(10,1);
//			lcd_puts(itoa((uint16_t)info,cadena,10));
//			lcd_gotoxy(15,1);
//			lcd_puts("[rpm]");
		break;
		case TEL_DIST_SIG:
			/* Show information */
//			lcd_gotoxy(0,2);
//			lcd_puts("    ");
//			lcd_gotoxy(0,2);
//			lcd_puts(itoa((uint16_t)info,cadena,10));
//			lcd_gotoxy(4,2);
//			lcd_puts("[cm]");
		break;
		case TEL_CURR_SIG:
			/* Show information */
//			lcd_gotoxy(11,2);
//			lcd_puts("     ");
//			lcd_gotoxy(11,2);
//			lcd_puts(itoa((uint16_t)info,cadena,10));
//			lcd_gotoxy(16,2);
//			lcd_puts("[mA]");
		break;
		case TEL_TEMPRX_SIG:
			/* Show information */
//			lcd_gotoxy(0,3);
//			lcd_puts("   ");
//			lcd_gotoxy(0,3);
//			lcd_puts(itoa((int16_t)(info),cadena,10));
//			lcd_gotoxy(3,3);
//			lcd_puts("[oC]");
		break;
		case TEL_TEMPTX_SIG:
			/* Show information */
//			lcd_gotoxy(0,3);
//			lcd_puts("   ");
//			lcd_gotoxy(0,3);
//			lcd_puts(itoa((int16_t)(info),cadena,10));
//			lcd_gotoxy(3,3);
//			lcd_puts("[oC]");
		break;
		case CTR_NCHANNELS_SIG:
			/* Show information */
//			lcd_gotoxy(14,3);
//			lcd_puts("CH:   ");
//			lcd_gotoxy(17,3);
//			lcd_puts(itoa((uint16_t)info,cadena,10));
		break;
		default:
		break;
	};
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            display_info_ss                                             */
/*                                                                              */
/* DESCRIPTION:     Displays the required information of the second screen      */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void display_info_ss(uint8_t source, int32_t info)
{
	(void)info;
	//char cadena[10];
	switch(source)
	{
		case TEL_IMUACELX_SIG:
			/* Show information */
//			lcd_gotoxy(0,1);
//			lcd_puts("      ");
//			lcd_gotoxy(0,1);
//			lcd_puts(itoa((int16_t)info,cadena,10));
		break;
		case TEL_IMUACELY_SIG:
			/* Show information */
//			lcd_gotoxy(7,1);
//			lcd_puts("      ");
//			lcd_gotoxy(7,1);
//			lcd_puts(itoa((int16_t)info,cadena,10));
		break;
		case TEL_IMUACELZ_SIG:
			/* Show information */
//			lcd_gotoxy(13,1);
//			lcd_puts("      ");
//			lcd_gotoxy(13,1);
//			lcd_puts(itoa((int16_t)info,cadena,10));
		break;
		case TEL_IMUGYROX_SIG:
			/* Show information */
//			lcd_gotoxy(0,3);
//			lcd_puts("      ");
//			lcd_gotoxy(0,3);
//			lcd_puts(itoa((int16_t)info,cadena,10));
		break;
		case TEL_IMUGYROY_SIG:
			/* Show information */
//			lcd_gotoxy(7,3);
//			lcd_puts("      ");
//			lcd_gotoxy(7,3);
//			lcd_puts(itoa((int16_t)info,cadena,10));
		break;
		case TEL_IMUGYROZ_SIG:
			/* Show information */
//			lcd_gotoxy(13,3);
//			lcd_puts("      ");
//			lcd_gotoxy(13,3);
//			lcd_puts(itoa((int16_t)info,cadena,10));
		break;
		default:
		break;
	};
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            display_info _ts                                            */
/*                                                                              */
/* DESCRIPTION:     Displays the required information of the third screen       */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void display_info_ts(uint8_t source, int32_t info)
{
	(void)info;
	//char cadena[10];
	switch(source)
	{
		case DEBUG_SIG_1:
			/* Show information */
//			lcd_gotoxy(0,1);
//			lcd_puts("DB1:      ");
//			lcd_gotoxy(4,1);
//			lcd_puts(itoa((int16_t)info,cadena,10));
		break;
		case DEBUG_SIG_2:
			/* Show information */
//			lcd_gotoxy(0,2);
//			lcd_puts("DB2:      ");
//			lcd_gotoxy(4,2);
//			lcd_puts(itoa((int16_t)info,cadena,10));
		break;
		case DEBUG_SIG_3:
			/* Show information */
//			lcd_gotoxy(10,3);
//	        lcd_puts("   ");
//			lcd_gotoxy(10,3);
//			lcd_puts(itoa((uint16_t)info,cadena,10));
		break;
		default:
		break;
	};
};


/*
 * display_sm.c
 *
 *  Created on: 22.01.2013
 *      Author: ArigitaJ
 */


