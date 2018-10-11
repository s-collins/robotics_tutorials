#ifndef TIMER_H
#define	TIMER_H

/*------------------------------- Helper Types -------------------------------*/

typedef enum { IDLE, RUNNING, DONE, UNINITIALIZED} TimerState;
typedef enum { TIMER_1, TIMER_2, INVALID_ID }      TimerID;

/*---------------------------------- Timer -----------------------------------*/

typedef struct
{
    TimerID             id;
    volatile TimerState state;
} Timer;

/*---------------------------- Function Interface ----------------------------*/

void
Timer_init_ms (
    Timer *  timer,     // address of the timer variable
    TimerID  id,        // ID of the physical timer module
    unsigned period_ms  // duration of timer (in milliseconds)
);

void
Timer_start (
    Timer * timer       // address of the timer variable
);

void
Timer_stop (
    Timer * timer       // address of the timer variable
);

#endif	/* TIMER_H */

