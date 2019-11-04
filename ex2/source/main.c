/*	Author: yxing024
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;



void TimerOn()
{
  TCCR1B = 0x0B;
  OCR1A = 125;
  TIMSK1 = 0x02;
  TCNT1 = 0;
  _avr_timer_cntcurr = _avr_timer_M;
  SREG |= 0x80;
}

void TimerOff()
{
  TCCR1B = 0x00;
}

void TimerISR()
{
  TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect)
{
  _avr_timer_cntcurr--;
  if (_avr_timer_cntcurr == 0)
    {
      TimerISR();
      _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M)
{
  _avr_timer_M = M;
  _avr_timer_cntcurr = _avr_timer_M;
}

//////////////////////////////////////
unsigned char threeLEDs;
unsigned char blinkingLED;
unsigned char combineLEDs;
//////////////////////////////////////
///////////////////////////ThreeLEDsSM
enum TSM_states { TSM_start, TSM_init, TSM_on} TSM_state;
 
  unsigned char T_out = 0x00;

TSM_Tick()
{
  


  switch(TSM_state)
    {
    case TSM_start:
      TSM_state = TSM_init; 
      break;
    case TSM_init:
      TSM_state = TSM_on;
      break;
    case TSM_on:
      TSM_state = TSM_on;
      break;
    default:
      TSM_state = TSM_start;
      break;
    }

  switch(TSM_state)
    {
    case TSM_start:
      break;
    case TSM_init:
      T_out = 0x01;
      break;
    case TSM_on:
      T_out = (T_out == 0x04)?0x01:T_out<<1;
      break;
    default:
      break;
    }
  threeLEDs = T_out;

  return TSM_state;
}
//////////////////////////////////////
///////////////////////////blinkingSM
enum BSM_states {BSM_start, BSM_init, BSM_on} BSM_state;

 unsigned char B_out = 0x00;

BSM_Tick()
{
  

 

  switch(BSM_state)
    {
    case BSM_start:
      BSM_state = BSM_init; 
      break;
    case BSM_init:
      BSM_state = BSM_on;
      break;
    case BSM_on:
      BSM_state = BSM_on;
      break;
    default:
      BSM_state = BSM_start;
      break;
    }

  switch(BSM_state)
    {
    case BSM_start:
      break;
    case BSM_init:
      B_out = 0x01;
      break;
    case BSM_on:
      B_out = (B_out == 0x08)?0x01:0x08;
      break;
    default:
      break;
    }
  blinkingLED = B_out;

  return BSM_state;
}

//////////////////////////////////////
///////////////////////////CombineLEDsSM
enum CSM_states {CSM_start, CSM_init, CSM_on} CSM_state;


  unsigned char C_out = 0x00;


CSM_Tick()
{
  


  switch(CSM_state)
    {
    case CSM_start:
      CSM_state = CSM_init; 
      break;
    case CSM_init:
      CSM_state = CSM_on;
      break;
    case CSM_on:
      CSM_state = CSM_on;
      break;
    default:
      CSM_state = CSM_start;
      break;
    }

  switch(CSM_state)
    {
    case CSM_start:
      break;
    case CSM_init:
      C_out = 0x00;
      break;
    case CSM_on:
      C_out = blinkingLED | threeLEDs;
      break;
    default:
      break;
    }
  combineLEDs = C_out;
  PORTB = combineLEDs;

  return CSM_state;
}

void main()
{
  //////////////////define var
  unsigned long TSM_elapsedTime = 0;
  unsigned long BSM_elapsedTime = 0;
  unsigned long CSM_elapsedTime = 0;
  const unsigned long timerPeriod = 100;
  //////////////////port
  DDRB = 0xff;
  PORTB = 0x00;
  //////////////////timer
  TimerSet(100);
  TimerOn();
  /////////////////init state
  TSM_state = TSM_start;
  BSM_state = BSM_start;
  CSM_state = CSM_start;
 
  while(1)
    {
      if (TSM_elapsedTime >= 300) {
	TSM_Tick();
	TSM_elapsedTime = 0;
      }
      if (BSM_elapsedTime >= 1000) { 
	BSM_Tick();
	BSM_elapsedTime = 0;
      }
      if (CSM_elapsedTime >= 300) {
	CSM_Tick();
	CSM_elapsedTime = 0;
      }
      while (!TimerFlag){}
      TimerFlag = 0;
       TSM_elapsedTime += timerPeriod;
       BSM_elapsedTime += timerPeriod;
       CSM_elapsedTime += timerPeriod;
    }
   
  //return 1;
}
