#pragma once


#define N_FLOORS 4

typedef enum { 
    DIRN_DOWN   = -1,
    DIRN_STOP   = 0,
    DIRN_UP     = 1
} MotorDirection; //opp eller ned retning på heisen


#define N_BUTTONS 3

typedef enum { 
    BUTTON_HALL_UP      = 0,
    BUTTON_HALL_DOWN    = 1,
    BUTTON_CAB          = 2
} ButtonType;


void elevio_init(void); //initialiserer heisen og input output 

void elevio_motorDirection(MotorDirection dirn); // setter retning på fysisk motor/heis
void elevio_buttonLamp(int floor, ButtonType button, int value); //skrur på lys på etasjepanel
void elevio_floorIndicator(int floor); //Viser hvilken etasje heisen er i
void elevio_doorOpenLamp(int value); //Setter på dørlys dersom åpen. 
void elevio_stopLamp(int value);//setter på stopplys

int elevio_callButton(int floor, ButtonType button);
int elevio_floorSensor(void); //indikierer om heisen er i 
int elevio_stopButton(void); //indikerer om stopknappen er trykket
int elevio_obstruction(void);//indikerer om obstruksjon knappen er trykket. 

