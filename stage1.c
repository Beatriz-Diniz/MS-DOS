#include <utils.h>

/* Load stage2 */
void __attribute__((naked)) load_stage2(){
        __asm__(
                /* Save all registers */
                "       pusha                           ;"

                /* Compute the size of the second stage */
                "       xor     %dx, %dx                ;"
                "       mov     $_STAGE2_SIZE, %ax      ;" /* Stage2 size determined by the linker */
                "       mov     $512, %cx               ;"
                "       div     %cx                     ;"
                "       add     $1, %ax                 ;"
                "       mov     %ax, size2              ;"

                /* Reset floppy just for the case */
                "reset:                                 ;"
                "       mov     $0x0, %ah               ;" /* Teset disk */
                "       mov     drive, %dl              ;" /* The boot drive */
                "       int     $0X13                   ;" /* Call Bios*/

                "       jnc     load                    ;" /* On error, abort */
                "       mov     $err2, %cx              ;"
                "       call    fatal                   ;"
                
                /* Load stage 2 */
                "load:                                  ;"
                "       mov     drive, %dl              ;" /* The boot drive */
                "       mov     $0x2, %ah               ;" /* Means read sector */
                "       mov     size2, %al              ;" /* Number of sectors to read */
                "       mov     $0x0, %ch               ;" /* Cylinder (starts at 0) */
                "       mov     $0x2, %cl               ;" /* Sector (starts at 1) */
                "       mov     $0x0, %dh               ;" /* Head (starts at 0) */
                "       mov     $_STAGE2_ADDR, %bx      ;" /* Where to load data */
                "       int     $0x13                   ;" /* Call Bios */

                "       mov     $err1, %cx              ;"
                "       jc      fatal                   ;" /* On error, abort */

                /* Restore all registers */
                "       popa                            ;"
                "       ret                             ;"
               );
}

/* This function is the entry point of stage2 */
void init();

int main(){
        clear();
        echo(" Stage 1: loading second stage..." NL NL);
        load_stage2();

        /* Call init() in stage2 */
        init();
        
        return 0;
}

const char err1[] = " Error: device reset failure." NL;
const char err2[] = " Error: device read failure." NL;
