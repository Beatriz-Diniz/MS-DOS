#ifndef UTILS_H
#define UTILS_H

#define VIDEO_MEMORY 0xb8000
#define VIDEO_ATTRIBUTE 0X02

/* Print string str on standard output.
 * CR-LF sequence.
*/
#define NL "\r\n"   

void __attribute((naked, fastcall)) echo (const char *str);

/* Clear the screen. */
void __attribute__((naked, fastcall)) clear (void);

/* Halt the system. */
void system_halt();

/* UTILS_H. */
#endif	
