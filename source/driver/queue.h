//created using chatGPT

#ifndef QUEUE_H
#define QUEUE_H
  // Adjust as needed
  #define MAX_ORDERS 20 

typedef struct {
    int floorOrderArray[MAX_ORDERS];
} Queue;

// Function prototypes
void addToQueue(Queue* queue, int order);
int getFirstInQueue(Queue* queue);
void removeFromQueue(Queue* queue);
void cleanQueue(Queue* queue);

#endif // QUEUE_H