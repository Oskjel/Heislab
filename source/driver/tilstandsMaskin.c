
#include "elevio.h"
#include "tilstandsMaskin.h"
#include <assert.h>
#include <stdio.h>

tilstandsMaskin TM;

void cleanQueue(){
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button =0; button < N_BUTTONS; button++) {
            TM .queue[floor][button]=0;
            elevio_buttonLamp(floor, button, OFF);
        }
    }
    
};

void set_lastFloor(){
    if (elevio_floorSensor()==-1) {
        TM.lastFloor = TM.floorState;
    }
}

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
    printf("Done");
    
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

void while_stop_hold() {
   
    if(TM.stopButton && elevio_floorSensor()!=-1 ) {
        while (TM.stopButton && elevio_floorSensor()!=-1 || TM.obstruction && TM.doorState) {
            stateRefresh();
        if (TM.doorState == CLOSE) {
            elevio_doorOpenLamp(ON);
            TM.doorState = OPEN;
        } 
       
    } 
      
    timer_3s();
    TM.doorState = CLOSE;
    elevio_doorOpenLamp(OFF);  
    cleanQueue();
    return;
    } 
    
    if (TM.stopButton)  {
        while (TM.stopButton) {
            stateRefresh();
            TM.doorState = CLOSE;
            TM.motorDirection = DIRN_STOP;
            elevio_motorDirection(DIRN_STOP);
        }
        cleanQueue();
        return;
    }
    
}


void executeOrder () {
    while_stop_hold();
  
    if (TM.doorState || TM.stopButton) {
        elevio_motorDirection(DIRN_STOP);
        cleanQueue();
        return;
    }
    
    // if Motordir == Stop
    // Gå til hva som helst
    if (TM.lastMovingDirection == DIRN_STOP && elevio_floorSensor()==-1){
        while(elevio_floorSensor()==-1) {
            elevio_motorDirection(DIRN_DOWN);
            
        }
        elevio_motorDirection(DIRN_STOP);
        return;
    }
    
    if (TM.lastMovingDirection == DIRN_STOP){
        if (nextProjectedFloor()==TM.floorState) {
            printf("Hola");
            doorOpen();
        }

        for (int floor = 0; floor < N_FLOORS; floor++){
            if(orderFloor(floor)){
                TM.motorDirection = sign(floor - TM.floorState);
                elevio_motorDirection(TM.motorDirection);
                return;
            }
        }
        
    }

    if (ordersInQueue() == OFF && TM.motorDirection == DIRN_STOP){
        TM.lastMovingDirection = DIRN_STOP;
        elevio_motorDirection(TM.motorDirection);
        return;
    }

   

    // If no projected floor exists, stop.
    if (TM.projectedFloor == -1) {
        TM.motorDirection = DIRN_STOP;
        elevio_motorDirection(DIRN_STOP);
        return;
    }

    // Decide on movement based on the projected floor relative to the current floor.
    if (sign(TM.projectedFloor - TM.floorState) > 0) {
        TM.motorDirection = DIRN_UP;
        elevio_motorDirection(TM.motorDirection);
    }
    else if (sign(TM.projectedFloor - TM.floorState) < 0) {
        TM.motorDirection = DIRN_DOWN;
        elevio_motorDirection(TM.motorDirection);
    }
    // Otherwise (projectedFloor equals current floor) we assume it's time to open the door.
    // However, also verify that the floor sensor shows a valid floor.

    //sTOPPER KUN NÅR DET ER -1
    else if ((elevio_floorSensor()!=-1 || TM.projectedFloor == TM.floorState)) {
        doorOpen();
    }


};

int ordersInQueue(){
    for (int floor = 0; floor < N_FLOORS; floor++){
        for (int button = 0; button < N_BUTTONS; button++){
            if (TM.queue[floor][button]){
                return ON;
            }
        }
    }
    return OFF;
}

void doorOpen(){
    if (elevio_floorSensor()==-1 ){
        return;
    }
    printf("hei\n");
    elevio_motorDirection(DIRN_STOP);
    TM .motorDirection = DIRN_STOP;
    cleanFloor();
    TM.doorState = ON;
    elevio_doorOpenLamp(ON);
    timer();
    TM.doorState = CLOSE;
    elevio_doorOpenLamp(OFF); 
};

void timer_3s(){
  
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);
    do {
        clock_gettime(CLOCK_MONOTONIC, &now);
        buttonPushed();
    } while ((now.tv_sec - start.tv_sec) < 3);
   
}
void timer(){
  
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);
    do {
        clock_gettime(CLOCK_MONOTONIC, &now);
        buttonPushed();
        stateRefresh();
    } while ((now.tv_sec - start.tv_sec) < 3 || TM.stopButton && elevio_floorSensor()!=-1 || TM.obstruction && TM.doorState) ;
   if (now.tv_sec - start.tv_sec > 3.5) {
    timer_3s();
   }
}


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
    set_lastFloor();
    
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
   
        for (int j =0; j < 3; j++) {
            if (TM.queue[floor][j]) {
                return 1;
            }
        }
    
    return 0;
};

int nextFloor(){
    int currentFloor = TM.floorState;
    int direction = sign(TM.lastMovingDirection);

    if (direction==DIRN_STOP){

    }

    
    // Check if the next floor in the direction is valid.
    int nextFloor = currentFloor + direction;
    if (nextFloor < 0 || nextFloor >= N_FLOORS) {
        //her må det gjøres noe!
    }
        

    // Moving upward.
    if (direction > 0){
        for (int floor = currentFloor + 1; floor < N_FLOORS; floor++){
            // At the top floor, only the hall down button exists.
            if (floor == N_FLOORS - 1){
                if (TM.queue[floor][BUTTON_HALL_DOWN] || TM.queue[floor][BUTTON_CAB])
                    return N_FLOORS-1;
            }
            // For middle floors, only hall up calls are accepted.
            else {
                if (TM.queue[floor][BUTTON_HALL_UP] || TM.queue[floor][BUTTON_CAB])
                    return floor;
            }
        }
        
    }
    // Moving downward.
    else if (direction < 0){
        // Change loop condition to include floor 0.
        for (int floor = currentFloor - 1; floor >= 0; floor--){
            // At the ground floor, only the hall up button exists.
            if (floor == 0){
                if (TM.queue[floor][BUTTON_HALL_UP] || TM.queue[floor][BUTTON_CAB])
                    return 0;
            }
            // For middle floors, only hall down calls are accepted.
            else {
                if (TM.queue[floor][BUTTON_HALL_DOWN] || TM.queue[floor][BUTTON_CAB])
                    return floor;
            }
        }
        
    }
    //sjekke om det er bestillinger i andre etasjer
    
   return 10000;
}

int nextProjectedFloor(void) {
    int currentFloor = TM.floorState;
    int direction = TM.motorDirection;  // current movement direction

    // If idle, choose the nearest floor with an order.
    if (direction == DIRN_STOP) {
        int bestFloor = -1;
        int minDiff = N_FLOORS;  // initialize to a large value
        for (int floor = 0; floor < N_FLOORS; floor++) {
            // Use orderAtFloor() or direct check if an order exists at this floor.
            // Here we assume that if any button is active at 'floor', there is an order.
            if (TM.queue[floor][BUTTON_CAB] ||
                TM.queue[floor][BUTTON_HALL_UP] ||
                TM.queue[floor][BUTTON_HALL_DOWN]) {
                int diff = abs(floor - currentFloor);
                if (diff < minDiff) {
                    minDiff = diff;
                    bestFloor = floor;
                }
            }
        }
        return bestFloor;
    }

    int floor;
    int fallbackCandidate = -1;
    // First, scan in the current moving direction.
    if (direction == DIRN_UP) {
        for (floor = currentFloor + 1; floor < N_FLOORS; floor++) {
            if (floor == N_FLOORS - 1) {
                // At the top floor, only the hall down button is physically present.
                
                if (TM.queue[floor][BUTTON_HALL_DOWN] || TM.queue[floor][BUTTON_CAB]) {
                    return floor;
                }
                
                    
                    
                    // For middle floors, only hall up calls are accepted.
                } 
                
                else if (TM.queue[floor][BUTTON_HALL_UP] || TM.queue[floor][BUTTON_CAB]) {
                    return floor;
                }
                else if (TM.queue[floor][BUTTON_HALL_DOWN] && orderCount()==1) {
                    return floor;
                }
                else if (TM.queue[1][1] && TM.queue[2][1] && orderCount()==2) {
                    return 2;
                }
                
            }
          /*  
            if (TM.queue[floor][BUTTON_HALL_DOWN] && fallbackCandidate == -1){
                fallbackCandidate =
            }
*/
        } else if (direction == DIRN_DOWN) {
            for (floor = currentFloor - 1; floor >= 0; floor--) {
            if (floor == 0) {
                // At the ground floor, only the hall up button is physically present.
                if (TM.queue[floor][BUTTON_HALL_UP] || TM.queue[floor][BUTTON_CAB])
                    return floor;
            } else {
                // For middle floors, only hall down calls are accepted.
                
                if (TM.queue[floor][BUTTON_HALL_DOWN] || TM.queue[floor][BUTTON_CAB])
                    return floor;

                else if (TM.queue[floor][BUTTON_HALL_UP] && orderCount()==1) {
                        return floor;
                    }
                else if (TM.queue[1][0] && TM.queue[2][0] && orderCount()==2) {
                    return 1;
                }
            }
        }
    }
    //Vi har ikke tatt med et tilfelle der vi skal ned og har trykket på opp eller vice versa. Derfor får vi rar oppførsel
    //Dersom vi bare tar disse med i eller-statement vil heisen stoppe  alle etasjer


    // If no orders are found in either direction, return a sentinel value.
    return -1;
}

int orderCount() {
    int antall = 0;
    for (int i  = 0; i< N_FLOORS; i++) {
        for (int j = 0; j < N_BUTTONS; j++) {
            if (TM.queue[i][j]) {
                antall++;
            }
        }
    }
    return antall;
}
