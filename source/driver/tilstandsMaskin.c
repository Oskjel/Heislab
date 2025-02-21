
#include "tilstandMaskin.h"

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
};

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
    
};
void cleanQueue(tilstandsMaskin* tilstand){
    for(int i = 0; i < 20; i++){
        tilstand->queueButtonType[i] = -1;
        tilstand->queueFloor[i] = -1;
    }
};

void buttonPushed(tilstandsMaskin* tilstand){ //Itererer gjennom alle knappene og legger til ordre hvis de er trykket på
    for(int i = 0; i < N_FLOORS; i++){
        for(int j = 0; j < 3; j++){
            if(elevio_callButton(i, j)){
                addOrder(tilstand, i, j);
            }
        }
    }
};


void executeOrder (tilstandsMaskin* tilstand) {

};

void doorOpen(tilstandsMaskin* tilstand){
    elevio_doorOpenLamp(1);
    tilstand->doorState = DOOR::OPEN;
    sleep(3);
    elevio_doorOpenLamp(0);
    tilstand->doorState = DOOR::CLOSE;
    executeOrder(tilstand);
};

void etasjePanel(tilstandsMaskin* tilstand){

    if(tilstand->floorState != -1){
        elevio_floorIndicator(tilstand->floorState);
    }
};


