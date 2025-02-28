#include "safety.h"

void stopIfDoorOpen(tilstandsMaskin *tilstand)
{
        elevio_motorDirection(DIRN_STOP); 
        tilstand->motorDirection = DIRN_STOP;
}

void stopBtnPressed(tilstandsMaskin *tilstand)
{
    elevio_motorDirection(DIRN_STOP); 
    tilstand->motorDirection = DIRN_STOP;
    cleanQueue(tilstand);
    tilstand->stopButton = ON;
}

void switchMotorDirAtEnd(tilstandsMaskin *tilstand)
{

}
