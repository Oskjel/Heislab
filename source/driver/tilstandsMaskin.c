
#include "tilstandsMaskin.h"


void initialize_tilstandsMaskin(tilstandsMaskin * pTM) {
    while (elevio_floorSensor()==-1)
    {
        elevio_motorDirection(DIRN_DOWN);
    }
    pTM->floorState = elevio_floorSensor();
    for(int i = 0; i < 20; i++){
        pTM->queueButtonType[i] = -1;
        pTM->queueFloor[i] = -1;
    }
    pTM->motorDirection = DIRN_STOP;
    pTM->doorState = CLOSE;
    pTM->stopButton = OFF;
    pTM->obstruction = OFF;
}

void addOrder(tilstandsMaskin* tilstand, int floor, ButtonType button){

    for(int i = 0; i < 20; i++){ 
        
        if(tilstand->queueButtonType[i] ==-1){
        
            tilstand->queueButtonType[i] = button;
            tilstand->queueFloor[i] = floor;
            break;
        } else {
            if(tilstand->queueFloor[i] == floor && tilstand->queueButtonType[i]==button) { // Legger ikke til eksisterende order
                break;
            }
        }           
}
};

void removeOrder(tilstandsMaskin* tilstand){
    
    for(int i = 0; i < 20; i++){
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
    int currentOrderFloor;
    int currentButtonType;
    
    int projected_motorDir;
    for (int i =0; i<20; i++) {
       currentOrderFloor=tilstand->queueFloor[i];
       currentButtonType=tilstand->queueButtonType[i];
       projected_motorDir = sign(currentOrderFloor - tilstand->floorState);

       
    }
};



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