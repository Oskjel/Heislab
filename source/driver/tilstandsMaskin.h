
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
    int changeFloor;
} tilstandsMaskin;

extern tilstandsMaskin TM;



void initialize_tilstandsMaskin();

void executeOrder (); //Utfører ordre 

void addOrder(int floor, ButtonType button); //Legger til ordre i køen [check]

void cleanFloor();

void removeOrder(int floor, ButtonType button); //Fjerner ordre fra køen [check]

void cleanQueue(); //Tømmer hele køen [check]

void buttonPushed(); //Itererer gjennom alle knappene og legger til ordre hvis de er trykket på

void doorOpen(); //Åpner døren og venter i 3 sekunder, endrer intern tilstand til åpen og lukket + lys


void stateRefresh();

int orderFloor(int floor);