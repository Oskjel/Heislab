#ifndef SAFETY_H
#define SAFETY_H

#include "tilstandsMaskin.h"
#include "elevio.h"
#include <stdio.h>

void stopIfDoorOpen(tilstandsMaskin * tilstand);

void stopBtnPressed(tilstandsMaskin* tilstand);

void switchMotorDirAtEnd (tilstandsMaskin* tilstand); 


#endif SAFETY_H
