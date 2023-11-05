#include "syscall.h"

/* Issue syscall number with arguments arg1, arg2, arg3. 
   Return the value in %ax. 
*/
int syscall(int number, int arg1, int arg2, int arg3){
        /* Declare variables are registers */
        int register ax __asm__("ax");
        int register bx __asm__("bx");
        int register cx __asm__("cx");
        int register dx __asm__("dx");
        int ax2, bx2, cx2, dx2;
        int status;

        /* Save current registers */
        ax2 = ax;
        bx2 = bx;
        cx2 = cx;
        dx2 = dx;

        /* Load registers */
        ax = number;
        bx = arg1;
        cx = arg2;
        dx = arg3;

        __asm__("int $0x21");

        status = ax;

        /* Restore registers */
        ax = ax2;
        bx = bx2;
        cx = cx2;
        dx = dx2;

        return status;
}

int puts(const char *buffer){
        int status;
        status = syscall(SYS_WRITE, (int) buffer, 0, 0);
        return status;
}
