/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             controlbuzzer_sm.c                                         */
/*                                                                              */
/* DESCRIPTION:      State machine that processes buzzer commands               */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include <stdint.h>
#include "controlbuzzer_sm.h"
#include "../../../System/HAL/hw_general.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/

/*--------------------------------- Star Wars ----------------------------------*/
/* Starwars Complete Sound Track */
float32_t Starwars_Track_Notes[MAX_TRACK_STARWARS] =
{
	FREQ_DO3,    FREQ_DO3,   FREQ_DO3,     FREQ_FA3,    FREQ_DO4,       FREQ_LA3_S,  FREQ_LA3,
	FREQ_SOL3,   FREQ_FA4,   FREQ_DO3,     FREQ_LA3_S,  FREQ_LA3,       FREQ_SOL3,   FREQ_FA4,

	FREQ_DO4,    FREQ_LA3_S, FREQ_LA3,     FREQ_SI3,    FREQ_SOL3,      FREQ_DO3,    FREQ_DO3,
	FREQ_RE3,    FREQ_LA3_S, FREQ_LA3,     FREQ_SOL3,   FREQ_FA3,       FREQ_FA3,    FREQ_SOL3,

	FREQ_LA3,    FREQ_SOL3,  FREQ_RE3,     FREQ_MI3,    FREQ_DO3,       FREQ_RE3,    FREQ_LA3_S,
	FREQ_LA3,    FREQ_SOL3,  FREQ_FA3,     FREQ_DO4,    FREQ_SOL3,      FREQ_RE3,    FREQ_LA3_S,

	FREQ_LA3,    FREQ_SOL3,  FREQ_FA3,     FREQ_FA3,    FREQ_SOL3,      FREQ_LA3,    FREQ_SOL3,
	FREQ_RE3,    FREQ_MI3,   FREQ_DO4,     FREQ_DO4,    FREQ_FA4,       FREQ_RE4_S,  FREQ_DO4_S,

	FREQ_DO4,    FREQ_LA3_S, FREQ_SOL3_S,  FREQ_SOL3,   FREQ_FA3,       FREQ_DO4,    SILENCE,
	FREQ_DO3,    FREQ_DO3,   FREQ_FA3,     FREQ_DO4,    FREQ_LA3_S,     FREQ_LA3,    FREQ_SOL3,

	FREQ_FA4,    FREQ_DO4,   FREQ_LA3_S,   FREQ_LA3,    FREQ_SOL3,      FREQ_FA4,    FREQ_DO4,
	FREQ_LA3_S,  FREQ_LA3,   FREQ_SI3,     FREQ_SOL3,   FREQ_DO3,       FREQ_DO3,    FREQ_DO3,

	FREQ_LA3_S,  FREQ_LA3,   FREQ_SI3,     FREQ_SOL3
};

/* Durations */
uint8_t Starwars_Track_Durations[MAX_TRACK_STARWARS] =
{
	CORCHEA,     CORCHEA,    CORCHEA,      BLANCA,      BLANCA,         CORCHEA,     CORCHEA,
	CORCHEA,     BLANCA,     NEGRA,        CORCHEA,     CORCHEA,        CORCHEA,     BLANCA,

	NEGRA,       CORCHEA,    CORCHEA,      CORCHEA,     BLANCA_CORCHEA, CORCHEA,     CORCHEA,
	BLANCA,      CORCHEA,    CORCHEA,      CORCHEA,     CORCHEA,        CORCHEA,     CORCHEA,

	CORCHEA,     CORCHEA,    SEMICORCHEA,  NEGRA,       NEGRA,          BLANCA,      CORCHEA,
	CORCHEA,     CORCHEA,    CORCHEA,      BLANCA,      BLANCA,         BLANCA,      CORCHEA,

	CORCHEA,     CORCHEA,    CORCHEA,      CORCHEA,     CORCHEA,        CORCHEA,     CORCHEA,
	SEMICORCHEA, NEGRA,      CORCHEA,      SEMICORCHEA, CORCHEA,        SEMICORCHEA, CORCHEA,

	SEMICORCHEA, CORCHEA,    SEMICORCHEA,  CORCHEA,     SEMICORCHEA,    BLANCA,      SILENCE_DUR,
	CORCHEA,     CORCHEA,    BLANCA,       BLANCA,      CORCHEA,        CORCHEA,     CORCHEA,

	BLANCA,      NEGRA,      CORCHEA,      CORCHEA,     CORCHEA,        BLANCA,      NEGRA,
	CORCHEA,     CORCHEA,    CORCHEA,      BLANCA,      CORCHEA,        CORCHEA,     CORCHEA,

	CORCHEA,     CORCHEA,    CORCHEA,      BLANCA
};

/* Starwars Imperial March Complete Sound Track */
float32_t StarwarsImp_Track_Notes[MAX_TRACK_STARWARSIMP] =
{
	FREQ_LA3,    FREQ_LA3,   FREQ_LA3,     FREQ_FA3,    FREQ_DO4,
	FREQ_LA3,    FREQ_FA3,   FREQ_DO4,     FREQ_LA3,
	FREQ_MI4,    FREQ_MI4,   FREQ_MI4,     FREQ_FA4,    FREQ_DO4,
	FREQ_SOL3_S, FREQ_FA3,   FREQ_DO4,     FREQ_LA3,
	FREQ_LA5,    FREQ_LA4,   FREQ_LA4,     FREQ_LA5,    FREQ_SOL4_S,   FREQ_SOL4,
	FREQ_FA4_S,  FREQ_FA4,   FREQ_FA4_S,   SILENCE,     FREQ_LA3_S,    FREQ_RE4_S,   FREQ_RE4,   FREQ_DO4_S,

	FREQ_DO4,    FREQ_SI3,   FREQ_DO4,     SILENCE,     FREQ_FA3,      FREQ_SOL3_S,  FREQ_FA3,   FREQ_SOL3_S,
	FREQ_DO4,    FREQ_LA3,   FREQ_DO4,     FREQ_MI4,
	FREQ_LA4,    FREQ_LA3,   FREQ_LA3,     FREQ_LA4,    FREQ_SOL4_S,   FREQ_SOL4,
	FREQ_FA4_S,  FREQ_FA4,   FREQ_FA4_S,   SILENCE,     FREQ_LA3_S,    FREQ_RE4_S,   FREQ_RE4,   FREQ_DO4_S,
	FREQ_DO4,    FREQ_SI3,   FREQ_DO4,     SILENCE,     FREQ_FA3,      FREQ_SOL3_S,  FREQ_FA3,   FREQ_DO4,
	FREQ_LA3,    FREQ_FA3,   FREQ_DO4,     FREQ_LA3,

	FREQ_DO4,    FREQ_SI3,   FREQ_DO4,     SILENCE,     FREQ_FA3,      FREQ_LA3,     FREQ_FA3,   FREQ_DO4,
	FREQ_SOL3_S, FREQ_FA3,   FREQ_DO4,     FREQ_SOL3_S,
	FREQ_LA3,    FREQ_LA3,   FREQ_LA3,     FREQ_FA3,    FREQ_DO4,
	FREQ_LA3,    FREQ_FA3,   FREQ_DO4,     FREQ_LA3,
	FREQ_MI4,    FREQ_MI4,   FREQ_MI4,     FREQ_FA4,    FREQ_DO4,

	FREQ_SOL3_S, FREQ_FA3,   FREQ_DO3,     FREQ_LA3,
	FREQ_LA4,    FREQ_LA3,   FREQ_LA3,     FREQ_LA4,    FREQ_SOL4_S,   FREQ_SOL4,
	FREQ_FA4_S,  FREQ_FA4,   FREQ_FA4_S,   SILENCE,     FREQ_LA3_S,    FREQ_RE4_S,   FREQ_RE4,   FREQ_DO4_S,
	FREQ_DO4,    FREQ_SI3,   FREQ_DO4,     SILENCE,     FREQ_FA3,      FREQ_SOL3_S,  FREQ_FA3,   FREQ_SOL3_S,
	FREQ_LA3,    FREQ_FA3,   FREQ_DO4,     FREQ_LA3,

	FREQ_LA4,    FREQ_LA3,   FREQ_LA3,     FREQ_LA4,    FREQ_SOL4_S,   FREQ_SOL4,
	FREQ_FA4_S,  FREQ_FA4,   FREQ_FA4_S,   SILENCE,     FREQ_LA3_S,    FREQ_RE4_S,   FREQ_RE4,   FREQ_DO4_S,
	FREQ_DO4,    FREQ_SI3,   FREQ_DO4,     SILENCE,     FREQ_FA3,      FREQ_SOL3_S,  FREQ_FA3,   FREQ_DO4,
	FREQ_LA3,    FREQ_FA3,   FREQ_DO4,     FREQ_LA3
};

/* Durations */
uint8_t StarwarsImp_Track_Durations[MAX_TRACK_STARWARSIMP] =
{
	NEGRA,       NEGRA,                  NEGRA,        CORCHEA_SEMICORCHEA,    SEMICORCHEA,
	NEGRA,       CORCHEA_SEMICORCHEA,    SEMICORCHEA,  BLANCA,
	NEGRA,       NEGRA,                  NEGRA,        CORCHEA_SEMICORCHEA,    SEMICORCHEA,
	NEGRA,       CORCHEA_SEMICORCHEA,    SEMICORCHEA,  BLANCA,
	NEGRA,       CORCHEA_SEMICORCHEA,    SEMICORCHEA,  NEGRA,                  CORCHEA_SEMICORCHEA,    SEMICORCHEA,
	SEMICORCHEA, SEMICORCHEA,            CORCHEA,      SILENCE_CORCH_DUR,      CORCHEA,      NEGRA,   CORCHEA_SEMICORCHEA, SEMICORCHEA,

	SEMICORCHEA, SEMICORCHEA,            CORCHEA,      SILENCE_CORCH_DUR,      CORCHEA,      NEGRA,   CORCHEA_SEMICORCHEA, SEMICORCHEA,
	NEGRA,       CORCHEA_SEMICORCHEA,    SEMICORCHEA,  BLANCA,
	NEGRA,       CORCHEA_SEMICORCHEA,    SEMICORCHEA,  NEGRA,                  CORCHEA_SEMICORCHEA,    SEMICORCHEA,
	SEMICORCHEA, SEMICORCHEA,            CORCHEA,      SILENCE_CORCH_DUR,      CORCHEA,      NEGRA,   CORCHEA_SEMICORCHEA, SEMICORCHEA,
	SEMICORCHEA, SEMICORCHEA,            CORCHEA,      SILENCE_CORCH_DUR,      CORCHEA,      NEGRA,   CORCHEA_SEMICORCHEA, SEMICORCHEA,
	NEGRA,       CORCHEA_SEMICORCHEA,    SEMICORCHEA,  BLANCA,

	SEMICORCHEA, SEMICORCHEA,            CORCHEA,      SILENCE_CORCH_DUR,      CORCHEA,      NEGRA,   CORCHEA_SEMICORCHEA, SEMICORCHEA,
	NEGRA,       CORCHEA_SEMICORCHEA,    SEMICORCHEA,  BLANCA,
	NEGRA,       NEGRA,                  NEGRA,        CORCHEA_SEMICORCHEA,    SEMICORCHEA,
	NEGRA,       CORCHEA_SEMICORCHEA,    SEMICORCHEA,  BLANCA,
	NEGRA,       NEGRA,                  NEGRA,        CORCHEA_SEMICORCHEA,    SEMICORCHEA,

	NEGRA,       CORCHEA_SEMICORCHEA,    SEMICORCHEA,  BLANCA,
	NEGRA,       CORCHEA_SEMICORCHEA,    SEMICORCHEA,  NEGRA,                  CORCHEA_SEMICORCHEA,    SEMICORCHEA,
	SEMICORCHEA, SEMICORCHEA,            CORCHEA,      SILENCE_CORCH_DUR,      CORCHEA,      NEGRA,   CORCHEA_SEMICORCHEA, SEMICORCHEA,
	SEMICORCHEA, SEMICORCHEA,            CORCHEA,      SILENCE_CORCH_DUR,      CORCHEA,      NEGRA,   CORCHEA_SEMICORCHEA, SEMICORCHEA,
	NEGRA,       CORCHEA_SEMICORCHEA,    SEMICORCHEA,  BLANCA,

	NEGRA,       CORCHEA_SEMICORCHEA,    SEMICORCHEA,  NEGRA,                  CORCHEA_SEMICORCHEA,   SEMICORCHEA,
	SEMICORCHEA, SEMICORCHEA,            CORCHEA,      SILENCE_CORCH_DUR,      CORCHEA,      NEGRA,   CORCHEA_SEMICORCHEA, SEMICORCHEA,
	SEMICORCHEA, SEMICORCHEA,            CORCHEA,      SILENCE_CORCH_DUR,      CORCHEA,      NEGRA,   CORCHEA_SEMICORCHEA, SEMICORCHEA,
	NEGRA,       CORCHEA_SEMICORCHEA,    SEMICORCHEA,  BLANCA
};

/*--------------------------------- Mario Bros ----------------------------------*/
float32_t MarioBros_Track_Notes[MAX_TRACK_MARIO_BROS] =
{
    FREQ_MI6,             FREQ_MI6,            SILENCE,          FREQ_MI6,
	SILENCE,              FREQ_DO6,            FREQ_MI6,         SILENCE,
	FREQ_SOL6,            SILENCE,             SILENCE,          SILENCE,
	FREQ_SOL5,            SILENCE,             SILENCE,          SILENCE,

	FREQ_DO6,             SILENCE,             SILENCE,          FREQ_SOL5,
	SILENCE,              SILENCE,             FREQ_MI5,         SILENCE,
	SILENCE,              FREQ_LA5,            SILENCE,          FREQ_SI5,
	SILENCE,              FREQ_LA5_S,          FREQ_LA5,         SILENCE,

	FREQ_SOL5,            FREQ_MI6,            FREQ_SOL6,        FREQ_LA6,
	SILENCE,              FREQ_FA6,            FREQ_SOL6,        SILENCE,
	FREQ_MI6,             SILENCE,             FREQ_DO6,         FREQ_RE6,
	FREQ_SI5,             SILENCE,             SILENCE,

	FREQ_DO6,             SILENCE,             SILENCE,          FREQ_SOL5,
	SILENCE,              SILENCE,             FREQ_MI5,         SILENCE,
	SILENCE,              FREQ_LA5,            SILENCE,          FREQ_SI5,
	SILENCE,              FREQ_LA5_S,          FREQ_LA5,         SILENCE,

	FREQ_SOL5,            FREQ_MI6,            FREQ_SOL6,        FREQ_LA6,
	SILENCE,              FREQ_FA6,            FREQ_SOL6,        SILENCE,
	FREQ_MI6,             SILENCE,             FREQ_DO6,         FREQ_RE6,
	FREQ_SI5,             SILENCE,             SILENCE
};

/* Durations */
uint8_t MarioBros_Track_Durations[MAX_TRACK_MARIO_BROS] =
{
	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,
	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,
	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,
	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,

	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,
	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,
	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,
	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,

	SEMICORCHEA_FUSA,     SEMICORCHEA_FUSA,    SEMICORCHEA_FUSA, CORCHEA,
	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,
	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,
	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,

	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,
	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,
	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,
	CORCHEA,              CORCHEA,             CORCHEA,          SEMICORCHEA_FUSA,

	SEMICORCHEA_FUSA,     SEMICORCHEA_FUSA,    CORCHEA,          CORCHEA,
	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,
	CORCHEA,              CORCHEA,             CORCHEA,          CORCHEA,
	CORCHEA,              CORCHEA
};

/*--------------------------------- Underworld ----------------------------------*/
float32_t Underworld_Track_Notes[MAX_TRACK_UNDERWORLD] =
{
    FREQ_DO3,             FREQ_DO4,        FREQ_LA2,       FREQ_LA3,
    FREQ_LA2_S,           FREQ_LA3_S,      SILENCE,
    SILENCE,

    FREQ_DO3,             FREQ_DO4,        FREQ_LA2,       FREQ_LA3,
    FREQ_LA2_S,           FREQ_LA3_S,      SILENCE,
    SILENCE,

    FREQ_DO3,             FREQ_DO4,        FREQ_LA2,       FREQ_LA3,
    FREQ_LA2_S,           FREQ_LA3_S,      SILENCE,
    SILENCE,

    FREQ_FA2,             FREQ_FA3,        FREQ_RE2,       FREQ_RE3,
    FREQ_RE2_S,           FREQ_RE3_S,      SILENCE,
    SILENCE,              FREQ_RE3_S,      FREQ_DO3_S,     FREQ_RE3,
    FREQ_DO3_S,           FREQ_RE3_S,
    FREQ_RE3_S,           FREQ_SOL2_S,
    FREQ_SOL2,            FREQ_DO3_S,
    FREQ_DO3,             FREQ_FA3_S,      FREQ_FA3,       FREQ_MI2,   FREQ_LA3_S,  FREQ_LA3,
    FREQ_SOL3_S,          FREQ_RE3_S,      FREQ_SI2,
    FREQ_LA2_S,           FREQ_LA2,        FREQ_SOL2_S
};

/* Durations */
uint8_t Underworld_Track_Durations[MAX_TRACK_UNDERWORLD] =
{
	CORCHEA,              CORCHEA,             CORCHEA,              CORCHEA,
	CORCHEA,              CORCHEA,             SEMICORCHEA,
	FUSA,

	CORCHEA,              CORCHEA,             CORCHEA,              CORCHEA,
	CORCHEA,              CORCHEA,             SEMICORCHEA,
	FUSA,

	CORCHEA,              CORCHEA,             CORCHEA,              CORCHEA,
	CORCHEA,              CORCHEA,             SEMICORCHEA,
	FUSA,

	CORCHEA,              CORCHEA,             CORCHEA,              CORCHEA,
	CORCHEA,              CORCHEA,             SEMICORCHEA,
	SEMICORCHEA,          CORCHEA_SEMICORCHEA, CORCHEA_SEMICORCHEA,  CORCHEA_SEMICORCHEA,
	SEMICORCHEA,          SEMICORCHEA,
	SEMICORCHEA,          SEMICORCHEA,
	SEMICORCHEA,          SEMICORCHEA,
	CORCHEA_SEMICORCHEA,  CORCHEA_SEMICORCHEA, CORCHEA_SEMICORCHEA,  CORCHEA_SEMICORCHEA, CORCHEA_SEMICORCHEA, CORCHEA_SEMICORCHEA,
	SEMICORCHEA_FUSA,     SEMICORCHEA_FUSA,    SEMICORCHEA_FUSA,
	SEMICORCHEA_FUSA,     SEMICORCHEA_FUSA,    SEMICORCHEA_FUSA,
};

/*--------------------------------- Tetris ----------------------------------*/
float32_t Tetris_Track_Notes[MAX_TRACK_TETRIS] =
{
    FREQ_SOL4,  FREQ_RE4,  FREQ_MI4,  FREQ_FA4,  FREQ_MI4,  FREQ_RE4,
    FREQ_DO4,   FREQ_DO4,  FREQ_MI4,  FREQ_SOL4, FREQ_FA4,  FREQ_MI4,
    FREQ_RE4,   FREQ_MI4,  FREQ_FA4,  FREQ_SOL4, FREQ_MI4,  FREQ_DO4,
    FREQ_DO4
};

/* Durations */
uint8_t Tetris_Track_Durations[MAX_TRACK_TETRIS] =
{
	NEGRA,         CORCHEA,   CORCHEA,   NEGRA,      CORCHEA,   CORCHEA,
	NEGRA,         CORCHEA,   CORCHEA,   NEGRA,      CORCHEA,   CORCHEA,
	NEGRA_CORCHEA, CORCHEA,   NEGRA,     NEGRA,      NEGRA,     NEGRA,
	NEGRA
};

/*------------------------------------------------------------------------------*/
/* Local Function Prototypes                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Functions                                                                    */
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
void init_controlbuzzer_sm(void)
{
	/* Initial States */
	controlbuzzer_sm.statem.state = INIT_BUZZER;
	/* Set up "in-between" notes pause time */
	controlbuzzer_sm.track_pausetime = 0;
	/* Initialize pointer to tracks */
	/* Star Wars */
	Tracks[STAR_WARS].duration = Starwars_Track_Durations;
	Tracks[STAR_WARS].note = Starwars_Track_Notes;
	Tracks[STAR_WARS].num_notes = MAX_TRACK_STARWARS;
	Tracks[STAR_WARS].tempo = TEMPO_STARWARS;
	Tracks[STAR_WARSIMP].duration = StarwarsImp_Track_Durations;
	Tracks[STAR_WARSIMP].note = StarwarsImp_Track_Notes;
	Tracks[STAR_WARSIMP].num_notes = MAX_TRACK_STARWARSIMP;
	Tracks[STAR_WARSIMP].tempo = TEMPO_STARWARSIMP;
	Tracks[MARIO_BROS].duration = MarioBros_Track_Durations;
	Tracks[MARIO_BROS].note = MarioBros_Track_Notes;
	Tracks[MARIO_BROS].num_notes = MAX_TRACK_MARIO_BROS;
	Tracks[MARIO_BROS].tempo = TEMPO_MARIO_BROS;
	Tracks[UNDERWORLD].duration = Underworld_Track_Durations;
	Tracks[UNDERWORLD].note = Underworld_Track_Notes;
	Tracks[UNDERWORLD].num_notes = MAX_TRACK_UNDERWORLD;
	Tracks[UNDERWORLD].tempo = TEMPO_UNDERWORLD;
	Tracks[TETRIS].duration = Tetris_Track_Durations;
	Tracks[TETRIS].note = Tetris_Track_Notes;
	Tracks[TETRIS].num_notes = MAX_TRACK_TETRIS;
	Tracks[TETRIS].tempo = TEMPO_TETRIS;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            controlbuzzer_sm_main                                       */
/*                                                                              */
/* DESCRIPTION:     Main function from the Buzzer State Machine                 */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void controlbuzzer_sm_main(eventtype const *event)
{
	/* Auxiliary Signal */
	#if(BUZZ_ACT_INIT_SONG == INIT_SONG_ON)
	SignalSM loc_signal;
	SignalSM *loc_signal_p = &loc_signal;
	#endif
	/* Aux. Variables */
	uint8_t aux1, aux2;

	/* SM */
	switch (controlbuzzer_sm.statem.state)
	{
		case INIT_BUZZER:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case GOALIVE_SIG:
					/* Actions */

					#if(BUZZ_ACT_INIT_SONG == INIT_SONG_ON)
					/* Play Startup Music */
					loc_signal_p->event.sig = BUZZREQ_SIG;
					loc_signal_p->param = 0;
					loc_signal_p->SM_receiver = CONTROLBUZZER_SM;
					/* Send Signal */
					sendq_Sevent((eventtype *) loc_signal_p);
					#endif

					controlbuzzer_sm.statem.state = STANDBY;
					controlbuzzer_sm_main(EXIT_STATE);
				break;
				default:
				break;
			};
		break;
		case STANDBY:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					BUZZER(SILENCE, OFF_DUTY);
				break;
				case BUZZREQ_SIG:
					/* Actions */
					/* Initialize Play Counter and Track Number based of Track Request */
					aux1 = (((SignalSM const *) event)->param);
					if(aux1 < MAX_TRACKS)
					{
						controlbuzzer_sm.track_num = aux1;
						if((Tracks[controlbuzzer_sm.track_num].num_notes) > MAX_PLAY)
						{
							controlbuzzer_sm.track_ctr = MAX_PLAY;
						}
						else
						{
							controlbuzzer_sm.track_ctr = Tracks[controlbuzzer_sm.track_num].num_notes;
						}
						/* Initialize first note duration as well as note */
						controlbuzzer_sm.track_notetime = (Tracks[controlbuzzer_sm.track_num].duration[0] * Tracks[controlbuzzer_sm.track_num].tempo);
						controlbuzzer_sm.track_note = 0;
						controlbuzzer_sm.statem.state = BUZZING;
						controlbuzzer_sm_main(EXIT_STATE);
					}
				break;
				default:
				break;
			};
		break;
		case BUZZING:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case TSLOT_10ms:
					/* As long as track is not completely played */
					if(controlbuzzer_sm.track_ctr > controlbuzzer_sm.track_note)
					{
						aux1 = controlbuzzer_sm.track_num;
						aux2 = controlbuzzer_sm.track_note;
						/* Play Note if it is the first time we get here */
						if(controlbuzzer_sm.track_notetime == Tracks[controlbuzzer_sm.track_num].duration[aux2])
						{
							BUZZER(Tracks[aux1].note[aux2], STD_DUTY);
							/* Reduce Track duration Note */
							controlbuzzer_sm.track_notetime--;
						}
						/* Hold on the note during its duration */
						else if(controlbuzzer_sm.track_notetime > 0)
						{
							/* Reduce Track duration Note */
							controlbuzzer_sm.track_notetime--;
						}
						/* Note is done */
						else
						{
							/* Play Pause Sound */
							if(controlbuzzer_sm.track_pausetime < INBETW_NOTES)
							{
								BUZZER(SILENCE, OFF_DUTY);
								controlbuzzer_sm.track_pausetime++;
							}
							/* Go to next note  */
							else
							{
								/* Reset Pause Time */
								controlbuzzer_sm.track_pausetime = 0;
								/* Go to next note */
								aux2++;
								/* Set duration */
								controlbuzzer_sm.track_notetime = (Tracks[controlbuzzer_sm.track_num].duration[aux2] * Tracks[controlbuzzer_sm.track_num].tempo);
								controlbuzzer_sm.track_note = aux2;
							}
						}
					}
					/* Go to Standby */
					else
					{
						controlbuzzer_sm.statem.state = STANDBY;
						controlbuzzer_sm_main(EXIT_STATE);
					}
				break;
				case BUZZREQ_SIG:
					/* Actions */
					/* Stop current melody */
					BUZZER(SILENCE, OFF_DUTY);
					/* Initialize Play Counter and Track Number based of Track Request */
					aux1 = (((SignalSM const *) event)->param);
					if(aux1 < MAX_TRACKS)
					{
						controlbuzzer_sm.track_num = aux1;
						if((Tracks[controlbuzzer_sm.track_num].num_notes) > MAX_PLAY)
						{
							controlbuzzer_sm.track_ctr = MAX_PLAY;
						}
						else
						{
							controlbuzzer_sm.track_ctr = Tracks[controlbuzzer_sm.track_num].num_notes;
						}
						/* Initialize first note duration as well as note */
						controlbuzzer_sm.track_notetime = (Tracks[controlbuzzer_sm.track_num].duration[0] * Tracks[controlbuzzer_sm.track_num].tempo);
						controlbuzzer_sm.track_note = 0;
					}
				break;
				default:
				break;
			};
		break;
		default:
		break;
	}
}

/*
 * controlbuzzer_sm.c
 *
 *  Created on: 26/11/2013
 *      Author: arigita
 */


