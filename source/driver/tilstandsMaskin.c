
#include "tilstandsMaskin.h"


tilstandsMaskin initialize_tilstandsMaskin(tilstandsMaskin pTM) {
    while (elevio_floorSensor()==-1)
    {
        elevio_motorDirection(DIRN_DOWN);
    }
    elevio_motorDirection(DIRN_STOP);

    pTM .floorState = elevio_floorSensor();
     for (int i = 0; i < 4; i++) {
        for (int j =0; j < 3; j++) {
            pTM .queue[i][j]=0;
        }
    }

    pTM .motorDirection = DIRN_STOP;
    pTM .doorState = CLOSE;
    pTM .stopButton = OFF;
    pTM .obstruction = OFF;
    return pTM;
}

tilstandsMaskin addOrder(tilstandsMaskin tilstand, int floor, ButtonType button){
        tilstand .queue[floor][button] = 1;
        return tilstand;
};

tilstandsMaskin removeOrder(tilstandsMaskin  tilstand,  int floor, ButtonType button){
    tilstand .queue[floor][button] = 0;
    return tilstand;
    
};
  tilstandsMaskin cleanQueue(tilstandsMaskin tilstand){
    for (int i = 0; i < 4; i++) {
        for (int j =0; j < 3; j++) {
            tilstand .queue[i][j]=0;
        }
    }
    return tilstand;
};
  tilstandsMaskin cleanFloor(tilstandsMaskin tilstand){
    
        for (int j =0; j < 3; j++) {
            tilstand .queue[tilstand .floorState][j]=0;
        }
    return tilstand;
};


void buttonPushed(tilstandsMaskin tilstand){ //Itererer gjennom alle knappene og legger til ordre hvis de er trykket på
    for(int floor = 0; floor < N_FLOORS; floor++){
        for(int button  = 0; button < N_BUTTONS; button++){
            int btnPressed = elevio_callButton(floor, button);
            if (btnPressed){
                addOrder(tilstand, floor,button);
            }
                
            
        }
    }
};


tilstandsMaskin executeOrder (tilstandsMaskin tilstand) {

    //sjekker om en knapp tilhørende nåværende etasje er på
        if (orderFloor(tilstand,tilstand .floorState) ) {
            printf("hei\n");
            elevio_motorDirection(DIRN_STOP);
            tilstand .motorDirection = DIRN_STOP;
            cleanFloor(tilstand);
            return tilstand;
        }

        if (tilstand .motorDirection == DIRN_STOP) {
            
            for (int floor = 0; floor < 4; floor++) {
                if (orderFloor(tilstand,floor)) {
                    if (floor > tilstand .floorState) {
                        elevio_motorDirection(DIRN_UP); tilstand .motorDirection=DIRN_UP;}
                    else 
                    {elevio_motorDirection(DIRN_DOWN); tilstand .motorDirection=DIRN_DOWN;}
                    return tilstand;
                }
            }

        }
        

    














};



tilstandsMaskin doorOpen(tilstandsMaskin   tilstand){
    return tilstand;  
};

void etasjePanel(tilstandsMaskin   tilstand){

    if(elevio_floorSensor() != -1){
        elevio_floorIndicator(tilstand .floorState);
    }
};


tilstandsMaskin stateRefresh(tilstandsMaskin   tilstand) {
    if(elevio_floorSensor()>-1){
        tilstand .floorState = elevio_floorSensor();
    }
    
    tilstand .obstruction = elevio_obstruction();
    tilstand .stopButton = elevio_stopButton();
    return tilstand;
}

int orderFloor(tilstandsMaskin tilstand, int floor) {
    for (int button = 0; button < 3; button++) {
        if (tilstand .queue[floor][button]) {
            return ON;
        }
    }
    return OFF;
};
