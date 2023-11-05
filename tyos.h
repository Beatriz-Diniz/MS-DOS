#ifndef TOS_H
#define TOS_H

#define STAGE2_ADDR 0x7e00

#define VIDEO_MEMORY 0xb8000
#define VIDEO_ATTRIBUTE 0X02



/* Prints a help message. */
void __attribute__((naked)) help (void);

/* Compare to strings up to BUFFER_MAX_LENGTH-1. */
#define BUFFER_MAX_LENGTH 3
int __attribute__((fastcall, naked)) compare (char *s1, char *s2);

/* Load stage 2 as a block. */
void load_stage2_block();
#define PROMPT ">"
#define INIT_MSG "Welcome! The commands are: date, time, quit and help!\n\n"

/* Shell */
int shell();

#endif
