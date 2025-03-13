#include <stdio.h>

#include "driver/elevio.h"
#include "driver/tilstandsMaskin.h"
#include "driver/helpful_func.h"




int main(){
    elevio_init();  
 
    initialize_tilstandsMaskin();
    
    printf("=== Elevator active & initilized ===\n");
    
    while(1){
        //printf("Neste etasje: %d\n DIR: %d \n",nextProjectedFloor(),TM.lastMovingDirection);
        stateRefresh();
        buttonPushed(); // Itererer gjennom alle knapper og legger til order dersom kanpp trykkes
        executeOrder();
        TM.projectedFloor = nextProjectedFloor();
      
    }
    
   elevio_motorDirection(DIRN_STOP);

    return 0;
}
