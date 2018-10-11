#include "timer.h"
#include <stdint.h>
#include <stdlib.h>
#include <xc.h>

/*------------------------- System Clock Information -------------------------*/

#define CLOCK_RATE_KHZ 8000UL // assume 8,000 kHz clock frequency

/*------------------ Global Variables to Track Timer State -------------------*/

static TimerState * timer_1_status;
static TimerState * timer_2_status;

/*--------------------------- Timer Initialization ---------------------------*/

void Timer_init_ms (Timer * timer, TimerID id, unsigned period_ms)
{
    //--------------------------------------------------------------------------
    // initialize timer state and control block
    //--------------------------------------------------------------------------
    switch (id)
    {
    case TIMER_1:
        timer->id        = TIMER_1;
        timer->state     = IDLE;
        timer_1_status   = &(timer->state);
        T1CONbits.TCKPS0 = 1;
        T1CONbits.TCKPS1 = 1;
        PR1              = period_ms * CLOCK_RATE_KHZ / 256;
        break;
    
    case (TIMER_2):
        timer->id        = TIMER_2;
        timer->state     = IDLE;
        timer_2_status   = &(timer->state);
        T2CONbits.TCKPS0 = 1;
        T2CONbits.TCKPS1 = 1;
        PR2              = period_ms * CLOCK_RATE_KHZ / 256;
        break;

    default:
        timer->id        = INVALID_ID;
        timer->state     = UNINITIALIZED;
        return;
    }
}

/*------------------------------- Start Timer --------------------------------*/

void Timer_start (Timer * timer)
{
    // validate timer state and ID
    if (timer->state == UNINITIALIZED || timer->id == INVALID_ID)
        return;
    switch (timer->id)
    {
    case TIMER_1:
        timer->state = RUNNING;
        TMR1 = 0;
        T1CONbits.TON = 1;
        IEC0bits.T1IE = 1;
        return;
       
    case TIMER_2:
        timer->state = RUNNING;
        TMR2 = 0;
        T2CONbits.TON = 1;
        IEC0bits.T2IE = 1;
        return;

    default:
        return;
    }
}

/*-------------------------------- Stop Timer --------------------------------*/

void Timer_stop (Timer * timer)
{
    // validate timer state and ID
    if (timer->state == UNINITIALIZED || timer->id == INVALID_ID)
        return;

    // stop the timer
    switch (timer->id)
    {
    case TIMER_1:
        timer->state = IDLE;
        T1CONbits.TON = 0;
        return;

    case TIMER_2:
        timer->state = IDLE;
        T2CONbits.TON = 0;
        return;
        
    default:
        return;
    }
}

/*------------------------ Interrupt Service Routines ------------------------*/

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt ()
{
    *timer_1_status = DONE;
    IFS0bits.T1IF = 0;
    T1CONbits.TON = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt ()
{
    *timer_2_status = DONE;
    IFS0bits.T2IF = 0;
    T2CONbits.TON = 0;
}

