
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

void initialize_tilstandsMaskin(tilstandsMaskin * pTM);

void executeOrder (tilstandsMaskin* tilstand); //Utfører ordre 

void addOrder(tilstandsMaskin* tilstand, int floor, ButtonType button); //Legger til ordre i køen [check]

void removeOrder(tilstandsMaskin* tilstand); //Fjerner ordre fra køen [check]

void cleanQueue(tilstandsMaskin* tilstand); //Tømmer hele køen [check]

void buttonPushed(tilstandsMaskin* tilstand); //Itererer gjennom alle knappene og legger til ordre hvis de er trykket på

void doorOpen(tilstandsMaskin* tilstand); //Åpner døren og venter i 3 sekunder, endrer intern tilstand til åpen og lukket + lys

void etasjePanel(tilstandsMaskin* tilstand); //tenn etasjelysene på etasjepanelet

void stateRefresh(tilstandsMaskin* tilstand);

void nextOrder(tilstandsMaskin* tilstand);