
#include <unistd.h> //sleep(3) for å vente i 3 sekunder
#include "elevio.h"
#include "con_load.h"

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
    int queueButtonType[20];
    int queueFloor[20];
    int motorDirection;
    int doorState;
    int stopButton;
    int obstruction;
} tilstandsMaskin;

void initialize_tilstandsMaskin() {
    tilstandsMaskin.floorState = elevio_floorSensor();
    for (int i = 0; i < 20; i++) {
        tilstandsMaskin.queueButtonType[i] = 0;  // Initialize to default value
        tilstandsMaskin.queueFloor[i] = 0;      // Initialize to default value
    }
    tilstandsMaskin.motorDirection = MotorDirection::DIRN_STOP;
    tilstandsMaskin.doorState = DOOR::CLOSE;
    tilstandsMaskin.stopButton = BUTTON::OFF;
    tilstandsMaskin.obstruction = BUTTON::OFF;
}


/*
static struct {
    int floorState = elevio_floorSensor();
    int queueButtonType[20] = {};
    int queueFloor[20] = {};
    int motorDirection{MotorDirection::DIRN_STOP};
    int doorState{DOOR::CLOSE};
    int stopButton{BUTTON::OFF};
    int obstruction{BUTTON::OFF};
    

} tilstandsMaskin;
*/

void executeOrder (tilstandsMaskin* tilstand); //Utfører ordre

void addOrder(tilstandsMaskin* tilstand, int floor, ButtonType button); //Legger til ordre i køen

void removeOrder(tilstandsMaskin* tilstand); //Fjerner ordre fra køen

void cleanQueue(tilstandsMaskin* tilstand); //Tømmer hele køen

void buttonPushed(tilstandsMaskin* tilstand); //Itererer gjennom alle knappene og legger til ordre hvis de er trykket på

void doorOpen(tilstandsMaskin* tilstand); //Åpner døren og venter i 3 sekunder, endrer intern tilstand til åpen og lukket + lys

void etasjePanel(tilstandsMaskin* tilstand); //tenn etasjelysene på etasjepanelet

