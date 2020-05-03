/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             controlbuzzer_sm.h                                         */
/*                                                                              */
/* DESCRIPTION:      State machine that processes buzzer commands               */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef CONTROLBUZZER_SM_H_
#define CONTROLBUZZER_SM_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include "../../../Event_Manager/event_manager.h"
#include "../../../Event_Manager/state_machine_fw.h"
#include "../../../config_switches.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/* Config Init Song */
#define INIT_SONG_OFF 0
#define INIT_SONG_ON 1

/* Standard Duty Cycle (%) */
#define STD_DUTY 50
#define OFF_DUTY 0

/* Reference to each Track */
#define STAR_WARS      0
#define STAR_WARSIMP   1
#define MARIO_BROS     2
#define UNDERWORLD     3
#define TETRIS         4
#define MAX_TRACKS     5

/* Maximum Sound Track length */
#define MAX_TRACK_STARWARS      88
#define MAX_TRACK_STARWARSIMP   152
#define MAX_TRACK_MARIO_BROS    78
#define MAX_TRACK_UNDERWORLD    53
#define MAX_TRACK_TETRIS        19

/* Track Tempo */
#define TEMPO_STARWARS      1
#define TEMPO_STARWARSIMP   1
#define TEMPO_MARIO_BROS    1
#define TEMPO_UNDERWORLD    1
#define TEMPO_TETRIS        1

/* Maximum Number of Notes to be played */
#define MAX_PLAY 19

/* Buzzer related aspects */
/* Durations (unit is 10ms) */
#define SEMIFUSA             1u
#define FUSA                 2u
#define SEMICORCHEA          4u
#define SEMICORCHEA_FUSA     6u
#define CORCHEA              8u
#define CORCHEA_SEMICORCHEA  12u
#define NEGRA                16u
#define NEGRA_CORCHEA        24u
#define BLANCA               32u
#define BLANCA_CORCHEA       40u
#define REDONDA              80u
#define SILENCE_CORCH_DUR    8u
#define SILENCE_DUR          16u
#define INBETW_NOTES         1u

/* Notes */
#define SILENCE     0.0f
/* Second Scale */
#define FREQ_DO2    130.813f
#define FREQ_DO2_S  138.591f
#define FREQ_RE2    146.832f
#define FREQ_RE2_S  155.563f
#define FREQ_MI2    164.814f
#define FREQ_FA2    174.614f
#define FREQ_FA2_S  184.997f
#define FREQ_SOL2   195.998f
#define FREQ_SOL2_S 207.652f
#define FREQ_LA2    220.0f
#define FREQ_LA2_S  233.082f
#define FREQ_SI2    246.942f
/* Third Scale */
#define FREQ_DO3    261.626f
#define FREQ_DO3_S  277.183f
#define FREQ_RE3    293.665f
#define FREQ_RE3_S  311.127f
#define FREQ_MI3    329.628f
#define FREQ_FA3    349.228f
#define FREQ_FA3_S  369.994f
#define FREQ_SOL3   391.995f
#define FREQ_SOL3_S 415.305f
#define FREQ_LA3    440.0f
#define FREQ_LA3_S  466.164f
#define FREQ_SI3    493.883f
/* Fourth Scale */
#define FREQ_DO4    523.251f
#define FREQ_DO4_S  554.365f
#define FREQ_RE4    587.33f
#define FREQ_RE4_S  622.254f
#define FREQ_MI4    659.255f
#define FREQ_FA4    698.456f
#define FREQ_FA4_S  739.989f
#define FREQ_SOL4   783.991f
#define FREQ_SOL4_S 830.609f
#define FREQ_LA4    880.0f
#define FREQ_LA4_S  932.328f
#define FREQ_SI4    987.767f
/* Fifth Scale */
#define FREQ_DO5    1046.502f
#define FREQ_DO5_S  1108.731f
#define FREQ_RE5    1174.659f
#define FREQ_RE5_S  1244.508f
#define FREQ_MI5    1318.51f
#define FREQ_FA5    1396.913f
#define FREQ_FA5_S  1479.978f
#define FREQ_SOL5   1567.982f
#define FREQ_SOL5_S 1661.219f
#define FREQ_LA5    1760.0f
#define FREQ_LA5_S  1864.655f
#define FREQ_SI5    1975.533f
/* Sixth Scale */
#define FREQ_DO6    2093.005f
#define FREQ_DO6_S  2217.461f
#define FREQ_RE6    2349.318f
#define FREQ_RE6_S  2489.016f
#define FREQ_MI6    2637.02f
#define FREQ_FA6    2793.826f
#define FREQ_FA6_S  2959.955f
#define FREQ_SOL6   3135.963f
#define FREQ_SOL6_S 3322.438f
#define FREQ_LA6    3520.0f
#define FREQ_LA6_S  3729.31f
#define FREQ_SI6    3951.066f

/* All Piano Freqs */
//Do 1: 65,406
//Do# 1: 69,296
//Re 1: 73,416
//Re# 1: 77,782
//Mi 1: 82,407
//Fa 1: 87,307
//Fa# 1: 92,499
//Sol 1: 97,999
//Sol#1: 103,826
//La 1: 110
//La# 1: 116,541
//Si 1: 123,471
//
//Do 2: 130,813
//Do# 2: 138,591
//Re 2: 146,832
//Re# 2: 155,563
//Mi 2: 164,814
//Fa 2: 174,614
//Fa# 2: 184,997
//Sol 2: 195,998
//Sol#2: 207,652
//La 2: 220
//La# 2: 233,082
//Si 2: 246,942
//
//Do 3: 261,626
//Do# 3: 277,183
//Re 3: 293,665
//Re# 3: 311,127
//Mi 3: 329,628
//Fa 3: 349,228
//Fa# 3: 369,994
//Sol 3: 391,995
//Sol#3: 415,305
//La 3: 440
//La# 3: 466,164
//Si 3: 493,883
//
//Do 4: 523,251
//Do# 4: 554,365
//Re 4: 587,33
//Re# 4: 622,254
//Mi 4: 659,255
//Fa 4: 698,456
//Fa# 4: 739,989
//Sol 4: 783,991
//Sol#4: 830,609
//La 4: 880
//La# 4: 932,328
//Si 4: 987,767
//
//Do 5: 1046,502
//Do# 5: 1108,731
//Re 5: 1174,659
//Re# 5: 1244,508
//Mi 5: 1318,51
//Fa 5: 1396,913
//Fa# 5: 1479,978
//Sol 5: 1567,982
//Sol#5: 1661,219
//La 5: 1760
//La# 5: 1864,655
//Si 5: 1975,533
//
//Do 6: 2093,005
//Do# 6: 2217,461
//Re 6: 2349,318
//Re# 6: 2489,016
//Mi 6: 2637,02
//Fa 6: 2793,826
//Fa# 6: 2959,955
//Sol 6: 3135,963
//Sol#6: 3322,438
//La 6: 3520
//La# 6: 3729,31
//Si 6: 3951,066
//
//Do 7: 4186,009
//Do# 7: 4434,922
//Re 7: 4698,636
//Re# 7: 4978,032
//Mi 7: 5274,041
//Fa 7: 5587,652
//Fa# 7: 5919,911
//Sol 7: 6271,927
//Sol#7: 6644,875
//La 7: 7040
//La# 7: 7458,62
//Si 7: 7902,133
//
//Do 8: 8372,018
//Do# 8: 8869,844
//Re 8: 9397,273
//Re# 8: 9956,063
//Mi 8: 10548,082
//Fa 8: 11175,303
//Fa# 8: 11839,822
//Sol 8: 12543,854
//Sol#8: 13289,75
//La 8: 14080
//La# 8: 14917,24
//Si 8: 15804,266

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/

enum States_ControlBuzzer {                      /* States Control State Machine*/
	INIT_BUZZER = 1,              			                   /* Initial State */
	STANDBY,                                                 /* No action State */
    BUZZING,                                                   /* Buzzing State */

    MAX_STATE_BUZZER_SM
};

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
/* Control Buzzer State Machine typedef */
typedef struct ControlBuzzerSMTag {
	/* Derived from basis State Machine */
	StateMachine statem;
	/* Control track play (num. remaining notes) */
	uint8_t track_ctr;
	/* Current track to play */
	uint8_t track_num;
	/* Current remaining note time (in 80ms units) */
	uint8_t track_notetime;
	/* Current remaining "in-between" notes time (in 80ms units) */
	uint8_t track_pausetime;
	/* Current note to play */
	uint8_t track_note;
} ControlBuzzerSM;

/* Music Tracks */
typedef struct SoundTrackTag {
	/* Array with notes (freqs) */
	float32_t *note;
	/* Array with times (in 10ms units) */
	uint8_t *duration;
	/* Num Notes */
	uint8_t num_notes;
	/* Tempo (less is quicker) */
	uint8_t tempo;
} SoundTrack;

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
/* Control Buzzer State Machine */
ControlBuzzerSM controlbuzzer_sm;

/* Sound Track Pointers */
SoundTrack Tracks[MAX_TRACKS] ALIGN_4;

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            init_controlbuzzer_sm                                       */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void init_controlbuzzer_sm(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            controlbuzzer_sm_main                                       */
/*                                                                              */
/* DESCRIPTION:     Main function from the Buzzer State Machine                 */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void controlbuzzer_sm_main(eventtype const *event);

#endif /* CONTROLBUZZER_SM_H_ */

/*
 * controlbuzzer_sm.h
 *
 *  Created on: 26/11/2013
 *      Author: arigita
 */
