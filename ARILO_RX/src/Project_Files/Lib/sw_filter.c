/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             sw_filter.c                                                */
/*                                                                              */
/* DESCRIPTION:      Filter library                                             */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "sw_filter.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/
/* TODO: Test */
float N[]={1.67e-02, 5.01e-02, 5.01e-02, 1.67e-02};
float D[]={1, -1.798, 1.221, -.2898};
float Z[4]={200.001,200.001,200.001,200.001};

/*------------------------------------------------------------------------------*/
/* Local Function Prototypes                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Functions                                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             iir_lowpass_fil                                            */
/*                                                                              */
/* DESCRIPTION:      Infinite Impulse Response Lowpass filter.                  */
/*                                                                              */
/* PARAMETERS:       In: val_act_raw: Actual unfiltered sample                  */
/*                       val_old_fil: Last filtered sample                      */
/*                       dt: Time in ms between samples                         */
/*                       time_const: Time Constant                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
int32_t iir_lowpass_fil(int32_t val_act_raw, int32_t val_old_fil, uint16_t dt, uint16_t time_const)
{
	int32_t val_act_fil;
	int32_t temp;
	temp = (int32_t)(val_act_raw - val_old_fil);
	temp = (int32_t)(((int32_t)dt) * temp);
	temp = (int32_t)(temp / (int32_t)(time_const + dt));
	val_act_fil = (int32_t)(val_old_fil + temp);
	return(val_act_fil);
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             fir_lowpass_fil                                            */
/*                                                                              */
/* DESCRIPTION:      Finite Impulse Response Lowpass filter (average).          */
/*                                                                              */
/* PARAMETERS:       In: array_val: Pointer to array of samples                 */
/*                       array_length: Length of samples array                  */
/*                   NOTE: Array order is position 0 NEWEST value               */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint16_t fir_lowpass_fil(uint16_t *array_val, uint8_t array_length)
{
	uint32_t val_act_fil = 0;
	uint8_t i;
	/* Calculate filtered value */
	for(i = 0; i < array_length; i++)
	{
		///* bk = 1/(n + 1) */
		///* y[n] = sum(bk * x[n - k]) */
		//val_act_fil = val_act_fil + ((1 / (i + 1)) * array_val[i]);
		val_act_fil = val_act_fil + (uint32_t)array_val[i];
	};
	val_act_fil = (uint32_t)((uint32_t)val_act_fil / (uint32_t)array_length);

	/* Update array values */
	/* To be done inside each SW component */

	return((uint16_t)val_act_fil);
	//return((uint16_t)1000);
};

/* TODO: Test */
float filter(float s)
{
  Z[3]=Z[2];
  Z[2]=Z[1];
  Z[1]=Z[0]; // the rolling buffer
  Z[0]= s - (D[1]*Z[1] + D[2]*Z[2] + D[3]*Z[3]);
  return Z[0]*N[0] + Z[1]*N[1] + Z[2]*N[2] + Z[3]*N[3];
}

/*
 * sw_filter.c
 *
 *  Created on: 22.03.2013
 *      Author: ArigitaJ
 */


