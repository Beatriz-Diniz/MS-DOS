#include <utils.h>

void __attribute__((naked)) init(){
        echo (" Stage 2: second stage loaded sucessuflly!" NL);

        system_halt();
}
