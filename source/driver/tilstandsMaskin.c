
#include "elevio.h"
#include "tilstandsMaskin.h"
#include <assert.h>

tilstandsMaskin TM;

void cleanQueue(){
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button =0; button < N_BUTTONS; button++) {
            TM .queue[floor][button]=0;
            elevio_buttonLamp(floor, button, OFF);
        }
    }
    
};

void initialize_tilstandsMaskin() {
    while (elevio_floorSensor()==-1)
    {
        elevio_motorDirection(DIRN_DOWN);
    }
    elevio_motorDirection(DIRN_STOP);
    cleanQueue();
    

    TM .floorState = elevio_floorSensor();
     for (int i = 0; i < 4; i++) {
        for (int j =0; j < 3; j++) {
            TM .queue[i][j]=0;
        }
    }
    TM.lastMovingDirection = DIRN_STOP;
    TM .motorDirection = DIRN_STOP;
    TM .doorState = CLOSE;
    TM .stopButton = OFF;
    TM .obstruction = OFF;
    
}

void addOrder( int floor, ButtonType button){
    if (!TM .queue[floor][button]) { // Stopper kontinuerlig setting av ordre dersom ordre fra før
        TM .queue[floor][button] = 1;
        elevio_buttonLamp(floor, button, ON);

    }
        
        
};

void removeOrder(int floor, ButtonType button){
    TM .queue[floor][button] = 0;
    
    
};
  
  void cleanFloor(){elevio_motorDirection(DIRN_STOP);
    
        for (int button =0; button < 3; button++) {
            TM .queue[TM.floorState][button]=0;
            elevio_buttonLamp(TM.floorState, button, OFF);
            //printf("%d", TM.floorState);
        }
    
};


void buttonPushed(){ //Itererer gjennom alle knappene og legger til ordre hvis de er trykket på
    for(int floor = 0; floor < N_FLOORS; floor++){
        for(int button  = 0; button < N_BUTTONS; button++){
            int btnPressed = elevio_callButton(floor, button);
            if (btnPressed){
                addOrder(floor,button);
            }
                
            
        }
    }
    
};


void executeOrder () {
    //tror ikke dette vil fungere dersom, vi tømmer queuen, og venter på nye bestillinger. 
    //Da må vi sørge for at last moving direction blir satt til dirn stop.

    //sjekker om en knapp tilhørende nåværende etasje er på
    if (orderFloor(TM.floorState) ) {
        elevio_motorDirection(DIRN_STOP);
        TM .motorDirection = DIRN_STOP;
        cleanFloor();
        return;
            
    }

// If the elevator is stopped and there's no last moving direction set, decide based on nearest order.
if (TM.motorDirection == DIRN_STOP && TM.lastMovingDirection == DIRN_STOP) {
    for (int f = 0; f < N_FLOORS; f++) {
        if (orderFloor(f)) {
            if (f > TM.floorState) {
                elevio_motorDirection(DIRN_UP);
                TM.motorDirection = DIRN_UP;
            } else {
                elevio_motorDirection(DIRN_DOWN);
                TM.motorDirection = DIRN_DOWN;
            }
            // Update lastMovingDirection to the chosen direction.
            TM.lastMovingDirection = TM.motorDirection;
            return;
        }
    }
}

// If the last moving direction is down, scan downward first.
if (TM.lastMovingDirection == DIRN_DOWN) {
    // Look downward from the current floor.
    for (int f = TM.floorState - 1; f >= 0; f--) {
        if (orderFloor(f)) {
            elevio_motorDirection(DIRN_DOWN);
            TM.motorDirection = DIRN_DOWN;
            // Keep lastMovingDirection as down.
            return;
        }
    }
    // If no orders downward, scan upward.
    for (int f = TM.floorState + 1; f < N_FLOORS; f++) {
        if (orderFloor(f)) {
            elevio_motorDirection(DIRN_UP);
            TM.motorDirection = DIRN_UP;
            TM.lastMovingDirection = DIRN_UP;
            return;
        }
    }
}

// If the last moving direction is up, scan upward first.
if (TM.lastMovingDirection == DIRN_UP) {
    // Look upward from the current floor.
    for (int f = TM.floorState + 1; f < N_FLOORS; f++) {
        if (orderFloor(f)) {
            elevio_motorDirection(DIRN_UP);
            TM.motorDirection = DIRN_UP;
            return;
        }
    }
    // If no orders upward, scan downward.
    for (int f = TM.floorState - 1; f >= 0; f--) {
        if (orderFloor(f)) {
            elevio_motorDirection(DIRN_DOWN);
            TM.motorDirection = DIRN_DOWN;
            TM.lastMovingDirection = DIRN_DOWN;
            return;
        }
    }
}

// If no orders exist, remain stopped.
elevio_motorDirection(DIRN_STOP);
TM.motorDirection = DIRN_STOP;

/*
        if (TM.motorDirection == DIRN_STOP) {
            
            for (int floor = 0; floor < 4; floor++) {
                if (orderFloor(floor)) {
                    if (floor > TM .floorState) {
                        elevio_motorDirection(DIRN_UP); TM .motorDirection=DIRN_UP;}
                    else 
                    {elevio_motorDirection(DIRN_DOWN); TM .motorDirection=DIRN_DOWN;}
                    return;
                }
            }

        }
        */
       //denne koden gjør at bestillinger i nedre etasjer prioriteres. 
};



void doorOpen(){
     
};

void etasjePanel(){

    if(elevio_floorSensor() != -1){
        elevio_floorIndicator(TM .floorState);
    }
};

int floorChange() {
    if(TM .floorState!= elevio_floorSensor() && elevio_floorSensor()!=-1) {
        
        return 1;
    }
    return 0;
    
}

void stateRefresh() {
    TM.changeFloor = floorChange();
    TM .obstruction = elevio_obstruction();
    TM .stopButton = elevio_stopButton();
    
    if (TM.motorDirection != DIRN_STOP){
        TM.lastMovingDirection = TM.motorDirection;
    }

    if(elevio_floorSensor()!=-1){
        TM .floorState = elevio_floorSensor();
        
    }
    if (TM.changeFloor) {
        
        elevio_floorIndicator(TM.floorState);
    }
    
}


int orderFloor(int floor) {
    for (int button = 0; button < 3; button++) {
        //printf("%d", TM.floorState);
        
        assert(floor>=0 && floor < N_FLOORS); //programmet prøver å akksessere
        //minne som er utenfor scopet til queue.
        if (TM.queue[floor][button]) {
            return ON;
        }        

    }
    return OFF;
};
