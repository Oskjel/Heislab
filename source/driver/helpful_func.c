
#include "helpful_func.h"
int sign(int x) {
    return (x>0) - (x<0);
}

int abs (int x){
    if (x > 0){
        return x;
    }
    else  {
        return -x;
    }
}