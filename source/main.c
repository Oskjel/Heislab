#include <stdio.h>

#include "driver/elevio.h"
#include "driver/tilstandsMaskin.h"
#include "driver/helpful_func.h"




int main(){
    elevio_init();  
 
    initialize_tilstandsMaskin();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");


    while(1){
        printf("Neste etasje: %d\n DIR: %d \n",nextProjectedFloor(),TM.lastMovingDirection);
        stateRefresh();
        buttonPushed(); // Itererer gjennom alle knapper og legger til order dersom kanpp trykkes
        executeOrder();
        TM.projectedFloor = nextProjectedFloor();
      
    }
    
    


    
   elevio_motorDirection(DIRN_STOP);
    
    //tilstandsMaskin* tilstand = new tilstandsMaskin();
/*
    while(1){
        //tilstand->etasjePanel();
        int floor = elevio_floorSensor();
        printf("Current floor: %d\n", floor);
    
        if(floor == 0){
            elevio_motorDirection(DIRN_UP);
        }

        if(floor == N_FLOORS-1){
            elevio_motorDirection(DIRN_DOWN);
        }


        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                elevio_buttonLamp(f, b, btnPressed);
                printf("Button at floor %d, button type %d is %s\n", f, b, btnPressed ? "pressed" : "not pressed");
            }
        }

        if(elevio_obstruction()){
            elevio_stopLamp(1);elevio_motorDirection(DIRN_UP);
    tilstandsMaskin* tilstand = &tilstandsMaskin();
        } else {
            elevio_stopLamp(0);
        }
        
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
        //husk å frigjør minne etter heisobjekt
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
    */

    //free(tilstand);

    return 0;
}
