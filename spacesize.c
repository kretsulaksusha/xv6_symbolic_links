#include "types.h"
#include "stat.h"
#include "user.h"

int main(void){
    printf(1,"Size of process memory (bytes) is %d bytes\n", numopenfiles());
    exit();
}