#pragma once
#include <unistd.h> //sleep(3) for å vente i 3 sekunder
#include <elevio.h>
#include <con_load.h>


typedef struct {
    int floorState = elevio_floorSensor();
    int queueButtonType[20] = {};
    int queueFloor[20] = {};
    int motorDirection{0};
    int doorState{0};
    int elevatorMoving{0};
    int stopButton{0};
    int obstruction{0};
    

} tilstandsMaskin;

void addOrder(tilstandsMaskin* tilstand, int floor, ButtonType button){

    for(int i = 0; i < 19; i++){ 
        if(tilstand->queueButtonType[i] ==-1){
            tilstand->queueButtonType[i] = button;
            tilstand->queueFloor[i] = floor;
            break;
        } else {
            printf("Queue is full"); // Maa lage en funksjon som stopper heisen og gir beskjed om at køen er full
            
            
        }
    }
}

void removeOrder(tilstandsMaskin* tilstand){
    
    for(int i = 0; i < 19; i++){
        tilstand->queueButtonType[i] = tilstand->queueButtonType[i+1]; //Flytter alle elementene en plass frem
        tilstand->queueFloor[i] = tilstand->queueFloor[i+1];
        if (tilstand->queueButtonType[i] == -1){
            tilstand->queueButtonType[i] = -1; //Tømmer siste element
            tilstand->queueFloor[i] = -1;
            break;
        }
    }
    
}
void cleanQueue(tilstandsMaskin* tilstand){
    for(int i = 0; i < 20; i++){
        tilstand->queueButtonType[i] = -1;
        tilstand->queueFloor[i] = -1;
    }
}

void buttonPushed(tilstandsMaskin* tilstand){ //Itererer gjennom alle knappene og legger til ordre hvis de er trykket på
    for(int i = 0; i < N_FLOORS; i++){
        for(int j = 0; j < 3; j++){
            if(elevio_getButtonSignal(i, j)){
                addOrder(tilstand, i, j);
            }
        }
    }
}

/*
void executeOrder (tilstandsMaskin* tilstand){
    if(tilstand->queue[tilstand->floorState] == BUTTON_CAB){
        elevio_doorOpenLamp(1);
        sleep(3);
        elevio_doorOpenLamp(0);
        removeOrder(tilstand, tilstand->floorState);
    }
    else if(tilstand->queue[tilstand->floorState] == BUTTON_HALL_UP){
        elevio_motorDirection(DIRN_UP);
        elevio_buttonLamp(tilstand->floorState, BUTTON_HALL_UP, 1);
        sleep(3);
        elevio_buttonLamp(tilstand->floorState, BUTTON_HALL_UP, 0);
        removeOrder(tilstand, tilstand->floorState);
    }
    else if(tilstand->queue[tilstand->floorState] == BUTTON_HALL_DOWN){
        elevio_motorDirection(DIRN_DOWN);
        elevio_buttonLamp(tilstand->floorState, BUTTON_HALL_DOWN, 1);
        sleep(3);
        elevio_buttonLamp(tilstand->floorState, BUTTON_HALL_DOWN, 0);
        removeOrder(tilstand, tilstand->floorState);
    }
}
*/