
#include "tilstandsMaskin.h"


void initialize_tilstandsMaskin(tilstandsMaskin * pTM) {
    while (elevio_floorSensor()==-1)
    {
        elevio_motorDirection(DIRN_DOWN);
    }
    pTM->floorState = elevio_floorSensor();
    pTM->queue = {{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}};
    pTM->motorDirection = DIRN_STOP;
    pTM->doorState = CLOSE;
    pTM->stopButton = OFF;
    pTM->obstruction = OFF;
}

void addOrder(tilstandsMaskin* tilstand, int floor, ButtonType button){
        tilstand->queue[floor][queue] = 1;
};

void removeOrder(tilstandsMaskin* tilstand,  int floor, ButtonType button){
    tilstand->queue[floor][queue] = 0;
    
};
void cleanQueue(tilstandsMaskin* tilstand){
    tilstand->queue = {{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}};
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

if (tilstand->motorDirection == DIRN_STOP) {
    for (int i = tilstand->floorState; i < 4; i++) {
        for (int j = 0; j < 3; j++)
        {
            if (tilstand->queue[i][j] && i > floorState) {
                elevio_motorDirection(DIRN_UP);
            }
            else if (tilstand->queue[i][j] && i < floorState) {
                elevio_motorDirection(DIRN_DOWN);
            }
        }
        
    }
}
else if (tilstand->motorDirection == DIRN_UP) {

    for (int i = tilstand->floorState; i < 4; i++) {
        
              if (tilstand->queue[i][0]||tilstand->queue[i][2]) {
                elevio_motorDirection(DIRN_UP);
                break;
              }      
              
    }
}
else  {
    for (int i = tilstand->floorState; i >=0 ; i--) {
        for (int j = 0; j < 2; j++) {
                if (tilstand->queue[i][0]||tilstand->queue[i][1]) {
                    void elevio_motorDirection(DIRN_DOWN);
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

    if(tilstand->floorState != -1){
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

void nextOrder(tilstandsMaskin* tilstand) {
    int currentFloor = tilstand->queueFloor[0];
    int currentButtonType = tilstand->queueButtonType[0];
    int least_dist = tilstand->queueFloor[1];

    for (int i = 1; i<20;i++ ) {
        if 
    } 
    
};