
#include "tilstandsMaskin.h"


void initialize_tilstandsMaskin(tilstandsMaskin * pTM) {
    while (elevio_floorSensor()==-1)
    {
        elevio_motorDirection(DIRN_DOWN);
    }
    elevio_motorDirection(DIRN_STOP);

    pTM->floorState = elevio_floorSensor();
     for (int i = 0; i < 4; i++) {
        for (int j =0; j < 3; j++) {
            pTM->queue[i][j]=0;
        }
    }

    pTM->motorDirection = DIRN_STOP;
    pTM->doorState = CLOSE;
    pTM->stopButton = OFF;
    pTM->obstruction = OFF;
}

void addOrder(tilstandsMaskin* tilstand, int floor, ButtonType button){
        tilstand->queue[floor][button] = 1;
};

void removeOrder(tilstandsMaskin* tilstand,  int floor, ButtonType button){
    tilstand->queue[floor][button] = 0;
    
};
void cleanQueue(tilstandsMaskin* tilstand){
    for (int i = 0; i < 4; i++) {
        for (int j =0; j < 3; j++) {
            tilstand->queue[i][j]=0;
        }
    }
};
void cleanFloor(tilstandsMaskin* tilstand){
    
        for (int j =0; j < 3; j++) {
            tilstand->queue[tilstand->floorState][j]=0;
        }
    
};


void buttonPushed(tilstandsMaskin* tilstand){ //Itererer gjennom alle knappene og legger til ordre hvis de er trykket på
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            int btnPressed = elevio_callButton(f, b);
            if (btnPressed){
                addOrder(tilstand, f,b);
            }
                
            elevio_buttonLamp(f, b, btnPressed);
        }
    }
};


void executeOrder (tilstandsMaskin* tilstand) {

    //sjekker om en knapp tilhørende nåværende etasje er på
        for (int j = 0; j < 3; j++) {
            if (tilstand->queue[tilstand->floorState][j]==ON) {
                elevio_motorDirection(DIRN_STOP);
                tilstand->motorDirection = DIRN_STOP;

                if(elevio_floorSensor() != -1){
                cleanFloor(tilstand); //fjerner bestillinger i nåværende etasje 
                }

                break;
            }
        }

    
        
if (tilstand->motorDirection == DIRN_STOP) {
    for (int floor = tilstand->floorState; floor < 4; floor++) {
        for (int j = 0; j < 3; j++)
        {
            if (tilstand->queue[floor][j] && floor > tilstand->floorState) {
                elevio_motorDirection(DIRN_UP);
                tilstand->motorDirection = DIRN_UP;

            }
            else if (tilstand->queue[floor][j] && floor < tilstand->floorState) {
                elevio_motorDirection(DIRN_DOWN);
                tilstand->motorDirection = DIRN_DOWN;
            }
        }
        
    }
}
else if (tilstand->motorDirection == DIRN_UP) {

    for (int i = tilstand->floorState+1; i < 4; i++) {
        
              if (tilstand->queue[i][0]||tilstand->queue[i][2]) {
                elevio_motorDirection(DIRN_UP);
                tilstand->motorDirection = DIRN_UP;
                break;
              }      
              
    }
}
else  {
    for (int i = tilstand->floorState +1; i >=0 ; i--) {
        for (int j = 0; j < 2; j++) {
                if (tilstand->queue[i][0]||tilstand->queue[i][1]) {
                    elevio_motorDirection(DIRN_DOWN);
                    tilstand->motorDirection = DIRN_DOWN;
                    break;
              }      
        }      
    }
}


}



void doorOpen(tilstandsMaskin* tilstand){
    elevio_doorOpenLamp(1);
    tilstand->doorState = OPEN;
    sleep(3);
    elevio_doorOpenLamp(0);
    tilstand->doorState = CLOSE;
    executeOrder(tilstand);
};

void etasjePanel(tilstandsMaskin* tilstand){

    if(elevio_floorSensor() != -1){
        elevio_floorIndicator(tilstand->floorState);
    }
};


void stateRefresh(tilstandsMaskin* tilstand) {
    if(elevio_floorSensor()>-1){
        tilstand->floorState = elevio_floorSensor();
    }
    
    tilstand->obstruction = elevio_obstruction();
    tilstand->stopButton = elevio_stopButton();
}
/*
void nextOrder(tilstandsMaskin* tilstand) {
    int currentFloor = tilstand->queueFloor[0];
    int currentButtonType = tilstand->queueButtonType[0];
    int least_dist = tilstand->queueFloor[1];

    for (int i = 1; i<20;i++ ) {
        
    } 
    
};
*/