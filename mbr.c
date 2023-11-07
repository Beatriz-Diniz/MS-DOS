#include <mbr.h>
#include <tyos.h>
#include <utils.h>

/* Output a help(-less) message. */
void __attribute__((naked)) help(void){
        printnl("I wish... (too small a program for anything else).");
        __asm__(
            "   ret                           ;"
        );                                      /* Naked functions lack return */
}

void date(void){
        char d, m, y0, y2;
        char data[11];

        __asm__ volatile (
        "       mov     $0x04, %%ah          ;" /* Read Real Time Clock Date */
        "       int     $0x1a                ;" /* Real Time Clock BIOS service */
        "       mov     %%ch, %[y0]          ;"
        "       mov     %%cl, %[y2]          ;"
        "       mov     %%dh, %[m0]          ;" /* get higher nibble bcd number */
        "       mov     %%dl, %[d0]          ;" /* get higher nibble bcd number */
        : [d0] "+m" (d), [m0] "+m" (m), [y0] "+m" (y0), [y2] "+m" (y2)
        :
        : "ax", "cx", "dx"
        );

        data[0] = (d >> 4) + 48;
        data[1] = (d & 15) + 48;
        data[2] = '/';
        data[3] = (m >> 4) + 48;
        data[4] = (m & 15) + 48;
        data[5] = '/';
        data[6] = (y0 >> 4) + 48;
        data[7] = (y0 & 15) + 48;
        data[8] = (y2 >> 4) + 48;
        data[9] = (y2 & 15) + 48;
        data[10] = 0;

        print(data);
        print(nl);

}

void time(void){
        char s, m, h;
        char time[9];

        __asm__ volatile(
            "   mov     $0x02, %%ah           ;" /* Read Real Time Clock time */
            "   int     $0x1a                 ;" /* Real Time Clock BIOS service */
            "   mov     %%ch, %[h0]           ;"
            "   mov     %%cl, %[m0]           ;"
            "   mov     %%dh, %[s0]           ;" /* Get higher nibble bcd number */
            :   [s0] "+m" (s), [m0] "+m" (m), [h0] "+m" (h)
            :
            :   "ax", "cx", "dx"
        );
        
        time[0] = (h >> 4) + 48;
        time[1] = (h & 15) + 48 - 3;            /* UTC−3 */
        time[2] = ':';
        time[3] = (m >> 4) + 48;
        time[4] = (m & 15) + 48;
        time[5] = ':';
        time[6] = (s >> 4) + 48;
        time[7] = (s & 15) + 48;
        time[8] = 0;

        print(time);
        printnl(nl);
}

void quit(){
        __asm__(
            "   mov     $0x1, %ax            ;" /* System call number for 'exit' (1) */
            "   xor     %bx, %bx             ;" /* Exit code (0 to indicate success) */
            "   int     $0x80                ;" /* System call */
            "   ret                          ;"
        );
}

/* Read string from terminal into buffer */
void __attribute__((fastcall, naked)) read (char *buffer){
    __asm__ volatile(
            "   mov     $0x0, %%si           ;" /* Iteration index for (%bx, %si) */
            "loop2:                          ;"
            "   movw    $0x0, %%ax           ;" /* Choose blocking read operation */
            "   int     $0x16               ;" /* Call BIOS keyboard read service */
            "   movb    %%al, %%es:(%%bx, %%si) ;" /* Fill in buffer pointed by %bx */
            "   inc     %%si                 ;"
            "   cmp     $0x0d, %%al          ;" /* Reiterate if not ascii 13 (CR) */

            
            "   mov     $0x0e, %%ah          ;" /* Echo character on the terminal */
            "   int     $0x10               ;"

            "   jne     loop2                ;"

            "   mov     $0x0e, %%ah          ;" /* Echo a newline */
            "   mov     $0x0a, %%al          ;"
            "   int     $0x10               ;"

            "   movb    $0x0, -1(%%bx, %%si)  ;" /* Add buffer a string delimiter */
            "   ret                         ;" /* REturn from function */
            
            :
            :   "b" (buffer)                   /* Ask gcc to store buffer in %bx */
            :   "ax", "cx", "si"               /* Additional clobbered registers */
    );
}

/* Compare two strings up to position BUFFER_MAX_LENGTH-1. */
int __attribute__((fastcall, naked)) compare(char *s1, char *s2){
        __asm__ volatile(
                "       mov     %[len], %%cx  ;"
                "       mov     $0x1, %%ax    ;"
                "       cld                  ;"
                "       repe    cmpsb        ;"
                "       jecxz   equal        ;"
                "       mov     $0x0, %%ax    ;"
                "equal:                      ;"
                "       ret                  ;"
                :
                :       [len] "n" (BUFFER_MAX_LENGTH-1), /* [len] is a constant */
                        "S" (s1),                        /* Ask gcc to store s1 in %si */
                        "D" (s2)                         /* Ask gcc to store s2 in %di */
                :       "ax", "cx", "dx"                 /* Additional clobbered registers */
        );
}
