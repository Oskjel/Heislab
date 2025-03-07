
#include <unistd.h> //sleep(3) for å vente i 3 sekunder
#include "elevio.h"
#include "con_load.h"
#include "helpful_func.h"

typedef enum {
    OPEN = 1,
    CLOSE = 0
} DOOR;

typedef enum {
    ON = 1,
    OFF = 0
} BUTTON;

typedef struct {
    int floorState;
    int queue[4][3];
    int motorDirection;
    int doorState;
    int stopButton;
    int obstruction;
} tilstandsMaskin;

tilstandsMaskin initialize_tilstandsMaskin(tilstandsMaskin    pTM);

tilstandsMaskin executeOrder (tilstandsMaskin   tilstand); //Utfører ordre 

tilstandsMaskin addOrder(tilstandsMaskin   tilstand, int floor, ButtonType button); //Legger til ordre i køen [check]

tilstandsMaskin cleanFloor(tilstandsMaskin   tilstand);

tilstandsMaskin removeOrder(tilstandsMaskin   tilstand,int floor, ButtonType button); //Fjerner ordre fra køen [check]

tilstandsMaskin cleanQueue(tilstandsMaskin   tilstand); //Tømmer hele køen [check]

void buttonPushed(tilstandsMaskin   tilstand); //Itererer gjennom alle knappene og legger til ordre hvis de er trykket på

tilstandsMaskin doorOpen(tilstandsMaskin   tilstand); //Åpner døren og venter i 3 sekunder, endrer intern tilstand til åpen og lukket + lys


tilstandsMaskin stateRefresh(tilstandsMaskin   tilstand);

int orderFloor(tilstandsMaskin   tilstand, int floor);