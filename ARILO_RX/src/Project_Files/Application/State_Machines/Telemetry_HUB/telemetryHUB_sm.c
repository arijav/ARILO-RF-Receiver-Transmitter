/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             telemetryHUB_sm.c                                          */
/*                                                                              */
/* DESCRIPTION:      State machine that processes the telemetry information     */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include <stdlib.h>
#include "config_switches.h"
#include "telemetryHUB_sm.h"
//#include "../../../System/Modules/MPU6050/hw_mpu6050.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/

#if(ARILORX_HWREV == ARILORX_HW1)
/* T-N variables for filtering purposes (RAW ADC Information) */
static uint16_t battery_old[N_SAMPLES] = {0,0,0,0,0};
static uint16_t current_old[N_SAMPLES_CURR] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static uint16_t temperature_old[N_SAMPLES] = {0,0,0,0,0};
static int16_t vref_old[N_SAMPLES] = {0,0,0,0,0};

/* Hall RPM Structures */
static HallRPM HallRPMsens[NUM_HALL];
#endif

/* Variables for distance measurement */
static uint16_t rx_sonar = 0;

/*------------------------------------------------------------------------------*/
/* Local Function Prototypes                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Functions                                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            init_telemetryHUBm                                          */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void init_telemetryHUB_sm(void)
{
    #if(ARILORX_HWREV == ARILORX_HW1)
	/* Aux variables */
	uint8_t k;

	telemetry_sm.statemADC.state = INIT_TELEMETRY;
	telemetry_sm.statemPulse.state = INIT_TELEMETRY;
    #endif
	telemetry_sm.statemIMU.state = INIT_TELEMETRY;
	telemetry_sm.statemUSART.state = INIT_TELEMETRY;

    #if(ARILORX_HWREV == ARILORX_HW1)
	for(k = 0; k < NUM_HALL; k++)
	{
    #if(RPM_MEAS_METHOD == RPM_TIMEWINDOW_METHOD)
		/* Aux variables */
		uint8_t i, j;
		telemetry_sm.statemPulse.state = CALCULATE_TIMEWINDOWS_RANGE;
		/* Init RPM Hall Structures and related info (head, tails) */
		for(i = 0; i < MAX_X_ARRAY_PULSES; i++)
		{
			for(j = 0; i < MAX_Y_ARRAY_PULSES; j++)
			{
				HallRPMsens[0].pulse_array[i][j] = 0;
				HallRPMsens[0].pulse_array[i][j] = 0;
			}
		}
		HallRPMsens[0].head = 0;
		HallRPMsens[0].head = 0;
		HallRPMsens[k].tail_array[T10MS][WINDOW_LENGTH] = 1;
		HallRPMsens[k].tail_array[T100MS][WINDOW_LENGTH] = 10;
		HallRPMsens[k].tail_array[T500MS][WINDOW_LENGTH] = 50;
		HallRPMsens[k].tail_array[T1S][WINDOW_LENGTH] = 100;
		for(i = 0; i < TOTAL_TIMEWINDOWS_X; i++)
		{
			HallRPMsens[k].tail_array[i][TAIL] = 0;
			HallRPMsens[k].tail_array[i][TAIL] = 0;
		}
		telemetry_sm.statemPulse.state = CALCULATE_RPM;
		for(i = 0; i < TOTAL_TIMEWINDOWS_X; i++)
		{
				HallRPMsens[k].rpm_array[i] = 0;
		}
    #elif(RPM_MEAS_METHOD == RPM_TIMESTAMPS_METHOD)
		telemetry_sm.statemPulse.state = INIT_TELEMETRY;
        HallRPMsens[k].rpm = 0;
    #endif
	}
    #endif
};

#if(ARILORX_HWREV == ARILORX_HW1)
/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            telemetryHUB_adcmeassm_main                                 */
/*                                                                              */
/* DESCRIPTION:     Main function from the ADC Telemetry Sub-State Machine      */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void telemetryHUB_adcmeassm_main(eventtype const *event)
{
	/* Auxiliary Signal */
	SignalSM loc_signal;
	SignalSM *loc_signal_p = &loc_signal;

	/* Auxiliary Measurement Value Variable */
	int32_t aux_val;
	uint8_t i;

	/* Sub-State machine for ADC measurements */
	switch (telemetry_sm.statemADC.state)
	{
		case INIT_TELEMETRY:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case GOALIVE_SIG:
					telemetry_sm.statemADC.state = MEASURE_BATTERY;
				break;
				default:
				break;
			}
		break;
		case MEASURE_BATTERY:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case TSLOT_10ms:
					/* Get value */
					aux_val = (int32_t)GET_MEASUREMENT(VBAT_S);

					/* Get filtered value */
					aux_val = iir_lowpass_fil(aux_val, (int32_t)battery_old[0], T_DIFF_TEL, TC_BATTERY);

					/* Reorder and update values */
					for(i = 0; i < (N_SAMPLES - 1); i++)
					{
						battery_old[i + 1] = battery_old[i];
					};

					/* Update last value for next filtering */
					battery_old[0] = (uint16_t)aux_val;

					/* FIR Filtering */
					aux_val = (int32_t)fir_lowpass_fil(&battery_old[0], N_SAMPLES);

					/* Conversion Factor */
					aux_val = (int32_t)(CONV_FACT_BATT * aux_val);

					/* Send Battery Measurement to Communications State Machine */
					/* Fill appropriate Data */
					loc_signal_p->SM_sender = TELEMETRYHUB_SM;
					loc_signal_p->SM_receiver = COMMUNICATIONS_SM;

					loc_signal_p->event.sig = TEL_BATT_SIG;
					loc_signal_p->param = aux_val;

					/* Send Signal */
					sendq_Sevent((eventtype *) loc_signal_p);

					/* Change State to */
					telemetry_sm.statemADC.state = MEASURE_CURRENT;
					/* Exit State Signal */
					telemetryHUB_adcmeassm_main(EXIT_STATE);
				break;
				default:
				break;
			};
		break;
		case MEASURE_CURRENT:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */

					/* Get value */
					aux_val = (int32_t)GET_MEASUREMENT(ACS758_S);

					/* Filter non valid values */
					if(aux_val < (CURR_OFFSET)) aux_val = (CURR_OFFSET);

					/* Get filtered value */

					/* IIR Filtering */
					aux_val = iir_lowpass_fil(aux_val, current_old[0], T_DIFF_TEL, TC_CURRENT);

					/* Reorder and update values */
					for(i = 0; i < (N_SAMPLES_CURR - 1); i++)
					{
						current_old[i + 1] = current_old[i];
					};

					current_old[0] = aux_val;

					/* FIR Filtering */
					aux_val = fir_lowpass_fil(&current_old[0], N_SAMPLES_CURR);

					/* Conversion Factors */
					aux_val = (int32_t)aux_val - ((int32_t)CURR_OFFSET);
					aux_val = (aux_val * CURR_A_MV);

					/* Send Battery Measurement to Communications State Machine */
					/* Fill appropriate Data */
					loc_signal_p->SM_sender = TELEMETRYHUB_SM;
					loc_signal_p->SM_receiver = COMMUNICATIONS_SM;

					loc_signal_p->event.sig = TEL_CURR_SIG;
					loc_signal_p->param = (int32_t) (aux_val);

					/* Send Signal */
					sendq_Sevent((eventtype *) loc_signal_p);

					/* Change State to */
					telemetry_sm.statemADC.state = MEASURE_TEMPERATURE;
					/* Exit State Signal */
					telemetryHUB_adcmeassm_main(EXIT_STATE);

				break;
				default:
				break;
			};
		break;
		case MEASURE_TEMPERATURE:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */

					/* Get value */
					aux_val = (int32_t)GET_MEASUREMENT(TEMP_S);

					/* Get filtered value */
					aux_val = iir_lowpass_fil(aux_val, temperature_old[0], T_DIFF_TEL, TC_TEMPERATURE);

					/* Update last value for next filtering */
					temperature_old[0] = (uint16_t)aux_val;

					/* Conversion to proper units */
					aux_val = (aux_val - TEMP_OFFSET) / TEMP_MV_D;

					/* Send Battery Measurement to Communications State Machine */
					/* Fill appropriate Data */
					loc_signal_p->SM_sender = TELEMETRYHUB_SM;
					loc_signal_p->SM_receiver = COMMUNICATIONS_SM;

					loc_signal_p->event.sig = TEL_TEMP_SIG;
					loc_signal_p->param = (int32_t) (aux_val);

					/* Send Signal */
					sendq_Sevent((eventtype *) loc_signal_p);

					/* Change State to */
					telemetry_sm.statemADC.state = MEASURE_VREF;
					/* Exit State Signal */
					telemetryHUB_adcmeassm_main(EXIT_STATE);
				break;
				default:
				break;
			};
		break;
		case MEASURE_VREF:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */

					/* Get value */
					aux_val = (int32_t)GET_MEASUREMENT(VREF_S);

					/* Get filtered value */
					aux_val = iir_lowpass_fil(aux_val, vref_old[0], T_DIFF_TEL, TC_VREF);

					/* Change State to */
					telemetry_sm.statemADC.state = MEASURE_BATTERY;
					/* Exit State Signal */
					telemetryHUB_adcmeassm_main(EXIT_STATE);
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
/* NAME:            telemetryHUB_hallmeassm_main                                */
/*                                                                              */
/* DESCRIPTION:     Main function from the Hall Telemetry Sub-State Machine     */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void telemetryHUB_hallmeassm_main(eventtype const *event)
{
	/* Auxiliary Signal */
	SignalSM loc_signal;
	SignalSM *loc_signal_p = &loc_signal;

#if(RPM_MEAS_METHOD == RPM_TIMEWINDOW_METHOD)
	/* Auxiliary Variables */
	uint32_t hallmeas_time = 0;
	uint32_t hallmeas_pulses = 0;
	int32_t aux1, aux2, aux3 = 0;
	uint32_t pulse_diff, time_diff = 0;
	uint16_t rpm = 0;

	/* Sub-State machine for pulse measurements */
	switch (telemetry_sm.statemPulse.state)
	{
		case CALCULATE_TIMEWINDOWS_RANGE:
			switch(event->sig)
			{
				case ENTRY_SIG:
				  /* Entry call code */

				break;
				case TSLOT_10ms:
					hallmeas_time = GET_MILLIS;
					hallmeas_pulses = GET_HALL1_PULSES;
					HallRPMsens[0].head = (((HallRPMsens[0].head)++) % MAX_X_ARRAY_PULSES);
					HallRPMsens[0].pulse_array[HallRPMsens[0].head][TIME_PULS] = hallmeas_time;
					HallRPMsens[0].pulse_array[HallRPMsens[0].head][NUM_PULS] = hallmeas_pulses;
					/* Calculate Pulse Array Tails for each RPM Window */
					for(aux1 = 0; aux1 < TOTAL_TIMEWINDOWS_X; aux1++)
					{
					  /* Calculates the Tail position (head minus window length */
					  aux2 = (HallRPMsens[0].head - HallRPMsens[0].tail_array[aux1][WINDOW_LENGTH]);
					  /* Pulse Array Tail for window rpm calculations */
					  if(aux2 < 0)
					  {
						  HallRPMsens[0].tail_array[aux1][TAIL] = (MAX_X_ARRAY_PULSES - aux2);
					  }
					  else
					  {
						  HallRPMsens[0].tail_array[aux1][TAIL] = aux2;
					  }
					}
					/* Change State to */
					telemetry_sm.statemPulse.state = CALCULATE_TIMEWINDOWS_RPM;
					/* Exit State Signal */
					telemetryHUB_hallmeassm_main(EXIT_STATE);
			  break;
			  default:
			  break;
			}
		break;
		case CALCULATE_TIMEWINDOWS_RPM:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					/* Calculate Pulse Difference and Time Difference */
					for(aux1 = 0; aux1 < TOTAL_TIMEWINDOWS_X; aux1++)
					{
					  /* Calculate Pulse Difference */
					  aux2 = HallRPMsens[0].tail_array[aux1][TAIL];
					  aux3 = HallRPMsens[0].pulse_array[HallRPMsens[0].head][NUM_PULS] - HallRPMsens[0].pulse_array[aux2][NUM_PULS];
					  if(aux3 < 0) aux3 = ((MAX_PULSES - HallRPMsens[0].pulse_array[aux2][NUM_PULS]) + HallRPMsens[0].pulse_array[HallRPMsens[0].head][NUM_PULS]);

					  pulse_diff = (uint32_t)aux3;

					  /* Calculate Time Difference (in ms */
					  aux3 = HallRPMsens[0].pulse_array[HallRPMsens[0].head][TIME_PULS] - HallRPMsens[0].pulse_array[aux2][TIME_PULS];
					  if(aux3 < 0) aux3 = ((MAX_TIME - HallRPMsens[0].pulse_array[aux2][TIME_PULS]) + HallRPMsens[0].pulse_array[HallRPMsens[0].head][TIME_PULS]);

					  time_diff = aux3;

					  /* Calculate RPM */
					  if((pulse_diff >= MIN_PULSES) && (time_diff != 0))
					  {
						  HallRPMsens[0].rpm_array[aux1] = (pulse_diff / time_diff)*(60000 / PULSE_REV);
					  }
					  else
					  {
						  HallRPMsens[0].rpm_array[aux1] = 0;
					  }
					}
				/* Change State to */
				telemetry_sm.statemPulse.state = SEND_RPM_MEASUREMENTS;
				/* Exit State Signal */
				telemetryHUB_hallmeassm_main(EXIT_STATE);
				break;
				default:
				break;
			}
		break;
		case SEND_RPM_MEASUREMENTS:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					for(aux1 = (TOTAL_TIMEWINDOWS_X - 1); aux1 >= 0 ; aux1--)
					{
						aux2 = HallRPMsens[0].rpm_array[aux1];
						if(aux2 > 0) rpm = aux2;
					}
					/* Send Signals */
					/* Send HALL Measurements to Communications State Machine */
					/* Fill appropriate Data */
					loc_signal_p->SM_sender = TELEMETRY_HUB;
					loc_signal_p->SM_receiver = COMMUNICATIONS;

					/* At first send just signal from sensor 1 */
					loc_signal_p->event.sig = TEL_RPM_SIG;
					loc_signal_p->param = (int32_t) (rpm);

					/* Send Signal */
					sendq_Sevent((eventtype *) loc_signal_p);

					/* Change State to */
					telemetry_sm.statemPulse.state = CALCULATE_TIMEWINDOWS_RANGE;
					/* Exit State Signal */
					telemetryHUB_hallmeassm_main(EXIT_STATE);
				break;
				default:
				break;
			}
		break;
		default:
		break;
	}
#elif(RPM_MEAS_METHOD == RPM_TIMESTAMPS_METHOD)
	uint8_t aux = 0;
	float32_t tdiff;
	uint32_t maxtime;
	uint32_t lastpulsetime;
	uint32_t tdiffmax;
	/* Sub-State machine for rpm measurements */
	switch (telemetry_sm.statemPulse.state)
	{
		case INIT_TELEMETRY:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case GOALIVE_SIG:
					telemetry_sm.statemPulse.state = CALCULATE_RPM;
				break;
				default:
				break;
			}
		break;
		case CALCULATE_RPM:
			switch(event->sig)
			{
				case ENTRY_SIG:
				  /* Entry call code */
				break;
				case TSLOT_10ms:
					for(aux = 0; aux < NUM_HALL; aux++)
					{
						/* Check if no pulse in MAX_RPM_PERIOD */
						maxtime = GET_MILLIS;
						lastpulsetime = (uint32_t)GET_HALL_TIMELASTPULSE(aux);
						if(maxtime >= lastpulsetime)
						{
							tdiffmax = maxtime - lastpulsetime;
						}
						else
						{
							tdiffmax = MAX_TIME - lastpulsetime + maxtime;
						};
						if(tdiffmax >= MAX_RPM_PERIOD)
						{
							HallRPMsens[aux].rpm = 0;
						}
						else
						{
							/* Get Time Difference between the pulses used for RPM calculation in milliseconds */
							tdiff = GET_HALL_TIMEDIFF(aux);
							/* Calculate RPM */
							if(tdiff != 0)
							{
								HallRPMsens[aux].rpm = (uint16_t)((float32_t)(((float32_t)PULSES_RPM_CALC(aux) * (float32_t)60000) / ((float32_t)PULSE_REV * tdiff)));
							}
							else
							{
								HallRPMsens[aux].rpm = (uint16_t)MAX_RPM;
							}
						}
					}
					/* Change State to */
					telemetry_sm.statemPulse.state = SEND_RPM_MEASUREMENTS;
					/* Exit State Signal */
					telemetryHUB_hallmeassm_main(EXIT_STATE);
				break;
				default:
				break;
			}
		break;
		case SEND_RPM_MEASUREMENTS:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					/* Send Signals */
					/* Send HALL Measurements to Communications State Machine */
					/* Fill appropriate Data */
					loc_signal_p->SM_sender = TELEMETRYHUB_SM;
					loc_signal_p->SM_receiver = COMMUNICATIONS_SM;

					/* At first send just signal from sensor 1 */
					loc_signal_p->event.sig = TEL_RPM_SIG;
					loc_signal_p->param = (int32_t) (HallRPMsens[HALL1].rpm);

					/* Send Signal */
					sendq_Sevent((eventtype *) loc_signal_p);

					/* Change State to */
					telemetry_sm.statemPulse.state = CALCULATE_RPM;
					/* Exit State Signal */
					telemetryHUB_hallmeassm_main(EXIT_STATE);
				break;
				default:
				break;
			}
		break;
		default:
		break;
	}
#endif

}
#endif

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            telemetryHUB_imusm_main                                     */
/*                                                                              */
/* DESCRIPTION:     Main function from the IMU Telemetry Sub-State Machine      */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void telemetryHUB_imusm_main(eventtype const *event)
{
	/* Auxiliary Signal */
	SignalSM loc_signal;
	SignalSM *loc_signal_p = &loc_signal;

	/* TODO: Test */
	int16_t /*double*/ gx = 0, gy = 0, gz = 0, ax = 0, ay = 0, az = 0;

	/* IMU Data */

	/* Sub-State machine for pulse measurements */
	switch (telemetry_sm.statemIMU.state)
	{
		case INIT_TELEMETRY:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case GOALIVE_SIG:
					telemetry_sm.statemIMU.state = PROCESS_IMU_MEASUREMENTS;
				break;
				default:
				break;
			}
		break;
		case PROCESS_IMU_MEASUREMENTS:
			switch(event->sig)
			{
				case ENTRY_SIG:
				break;
				case TSLOT_10ms:
					/* Entry call code */
					/* Receive i2c Data */
					/* Send IMU Measurements to Communications State Machine */
					/* Fill appropriate Data */
					loc_signal_p->SM_sender = TELEMETRYHUB_SM;
					loc_signal_p->SM_receiver = COMMUNICATIONS_SM;

					/* Get raw data from IMU */
					//mpu6050_getRawData(&ax, &ay, &az, &gx, &gy, &gz);
					//mpu6050_getConvData(&ax, &ay, &az, &gx, &gy, &gz);

					/* Send Signal */
					loc_signal_p->event.sig = TEL_IMUACELX_SIG;
					loc_signal_p->param = (int32_t) ((int16_t)ax++);
					sendq_Sevent((eventtype *) loc_signal_p);

					/* Send Signal */
					loc_signal_p->event.sig = TEL_IMUACELY_SIG;
					loc_signal_p->param = (int32_t) ((int16_t)ay++);
					sendq_Sevent((eventtype *) loc_signal_p);

					/* Send Signal */
					loc_signal_p->event.sig = TEL_IMUACELZ_SIG;
					loc_signal_p->param = (int32_t) ((int16_t)az++);
					sendq_Sevent((eventtype *) loc_signal_p);

					/* Send Signal */
					loc_signal_p->event.sig = TEL_IMUGYROX_SIG;
					loc_signal_p->param = (int32_t) ((int16_t)gx++);
					sendq_Sevent((eventtype *) loc_signal_p);

					/* Send Signal */
					loc_signal_p->event.sig = TEL_IMUGYROY_SIG;
					loc_signal_p->param = (int32_t) ((int16_t)gy++);
					sendq_Sevent((eventtype *) loc_signal_p);

					/* Send Signal */
					loc_signal_p->event.sig = TEL_IMUGYROZ_SIG;
					loc_signal_p->param = (int32_t) ((int16_t)gz++);
					sendq_Sevent((eventtype *) loc_signal_p);

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
/* NAME:            telemetryHUB_usartsm_main                                   */
/*                                                                              */
/* DESCRIPTION:     Main function from the USART Telemetry Sub-State Machine    */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void telemetryHUB_usartsm_main(eventtype const *event)
{
	/* Auxiliary variables and Signal */
	uint8_t aux;
	char aux2[3];
	SignalSM loc_signal;
	SignalSM *loc_signal_p = &loc_signal;

	/* Sonar distance */
	uint8_t dist_available;
	uint8_t *data_sonar = 0;

	/* Sub-State machine for pulse measurements */
	switch (telemetry_sm.statemUSART.state)
	{
		case INIT_TELEMETRY:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case GOALIVE_SIG:
					telemetry_sm.statemUSART.state = GET_DISTANCE;
				break;
				default:
				break;
			}
		break;
		case GET_DISTANCE:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case TSLOT_80ms:
					dist_available = GET_DISTANCE_BUFF(data_sonar);
					if(dist_available == DISTANCE_AVAILABLE)
					{
					 for(aux = 0; aux < DIST_BUFFSIZE; aux++)
					 {
						 if(data_sonar[aux] == 'R')
						 {
							 aux2[0] = data_sonar[(aux + 1)%DIST_BUFFSIZE];
							 aux2[1] = data_sonar[(aux + 2)%DIST_BUFFSIZE];
							 aux2[2] = data_sonar[(aux + 3)%DIST_BUFFSIZE];
							 rx_sonar = (uint16_t)((float32_t)(atoi(aux2)) * (float32_t)RX_CONV_FACT);
							 break;
						 }
					 }
					}

					/* Change State to */
					telemetry_sm.statemUSART.state = SEND_DISTANCE;
					/* Exit State Signal */
					telemetryHUB_usartsm_main(EXIT_STATE);
				break;
				default:
				break;
			}
		break;
		case SEND_DISTANCE:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					/* Send Battery Measurement to Communications State Machine */
					/* Fill appropriate Data */
					loc_signal_p->SM_sender = TELEMETRYHUB_SM;
					loc_signal_p->SM_receiver = COMMUNICATIONS_SM;

					loc_signal_p->event.sig = TEL_DIST_SIG;
					loc_signal_p->param = (int32_t) (rx_sonar);

					/* Send Signal */
					sendq_Sevent((eventtype *) loc_signal_p);

					/* Change State to */
					telemetry_sm.statemUSART.state = GET_DISTANCE;
					/* Exit State Signal */
					telemetryHUB_usartsm_main(EXIT_STATE);
				break;
				default:
				break;
			}
		break;
		default:
		break;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            telemetryHUB_sm_main                                        */
/*                                                                              */
/* DESCRIPTION:     Main function from the Telemetry State Machine              */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void telemetryHUB_sm_main(eventtype const *event)
{
	/* Auxiliary Signal */
	//SignalSM loc_signal;
	//SignalSM *loc_signal_p = &loc_signal;

	/* TODO: Test, debugging */
	//state_counter_sm[(TELEMETRY_HUB - 1)][telemetry_sm.statemADC.state]++;

    #if(ARILORX_HWREV == ARILORX_HW1)
	/* Call main first Sub-SM */
	telemetryHUB_adcmeassm_main(event);

	/* Call main second Sub-SM */
	telemetryHUB_hallmeassm_main(event);
    #endif

	/* Call main third Sub-SM */
	telemetryHUB_imusm_main(event);

	/* Call main fourth Sub-SM */
	telemetryHUB_usartsm_main(event);

	/* Send Debug Signal */

	switch(event->sig)
	{
		case TSLOT_10ms:
			/* Fill appropriate Data */
			//loc_signal_p->SM_sender = TELEMETRYHUB_SM;
			//loc_signal_p->SM_receiver = COMMUNICATIONS_SM;

			/* Send Signal */
			//loc_signal_p->event.sig = DEBUG_SIG_1;
			//loc_signal_p->param = (int32_t) MPU6050_TestConnection();
			//loc_signal_p->param = (int32_t) (GET_HALL1_PULSES);
			//loc_signal_p->param = (int32_t) (DEBUG_HOPERF);
			//sendq_Sevent((eventtype *) loc_signal_p);
		break;
		default:
		break;
	};
};


/*
 * telemetryHUB_sm.c
 *
 *  Created on: 04.03.2013
 *      Author: ArigitaJ
 */


