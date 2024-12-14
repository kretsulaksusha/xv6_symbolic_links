#include "types.h"
#include "stat.h"
#include "user.h"

int main(void){
    printf(1,"Number of free memory pages is %d\n", freememp());
    exit();
}