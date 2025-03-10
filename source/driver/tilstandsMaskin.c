
#include "tilstandsMaskin.h"


void initialize_tilstandsMaskin() {
    while (elevio_floorSensor()==-1)
    {
        elevio_motorDirection(DIRN_DOWN);
    }
    elevio_motorDirection(DIRN_STOP);

    TM .floorState = elevio_floorSensor();
     for (int i = 0; i < 4; i++) {
        for (int j =0; j < 3; j++) {
            TM .queue[i][j]=0;
        }
    }

    TM .motorDirection = DIRN_STOP;
    TM .doorState = CLOSE;
    TM .stopButton = OFF;
    TM .obstruction = OFF;
    
}

void addOrder( int floor, ButtonType button){
    if (!TM .queue[floor][button]) { // Stopper kontinuerlig setting av ordre dersom ordre fra før
        TM .queue[floor][button] = 1;

    }
        
        
};

void removeOrder(int floor, ButtonType button){
    TM .queue[floor][button] = 0;
    
    
};
  void cleanQueue(){
    for (int i = 0; i < 4; i++) {
        for (int j =0; j < 3; j++) {
            TM .queue[i][j]=0;
        }
    }
    
};
  void cleanFloor(){
    
        for (int j =0; j < 3; j++) {
            TM .queue[TM .floorState][j]=0;
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

    //sjekker om en knapp tilhørende nåværende etasje er på
        if (orderFloor(TM .floorState) ) {
            printf("hei\n");
            elevio_motorDirection(DIRN_STOP);
            TM .motorDirection = DIRN_STOP;
            cleanFloor();
            return;
            
        }

        if (TM .motorDirection == DIRN_STOP) {
            
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
};



void doorOpen(){
     
};

void etasjePanel(){

    if(elevio_floorSensor() != -1){
        elevio_floorIndicator(TM .floorState);
    }
};


void stateRefresh() {
    if(elevio_floorSensor()>-1){
        TM .floorState = elevio_floorSensor();
        etasjePanel();
    }
    
    TM .obstruction = elevio_obstruction();
    TM .stopButton = elevio_stopButton();
    
}

int orderFloor( int floor) {
    for (int button = 0; button < 3; button++) {
        if (TM .queue[floor][button]) {
            return ON;
        }
    }
    return OFF;
};
