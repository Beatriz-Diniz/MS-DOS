#ifndef MBR_H
#define MBR_H

/* Core functions */

/* Print the null-terminated buffer on standard output */
void date();
void time();

/* Commands */

/* Print a help message */
#define HELP_CMD "help"

/* Prints date */
#define DATE_CMD "date"

/* Prints time*/
#define TIME_CMD "time"

/* Quit */
#define QUIT_CMD "quit"
#define quit() printnl("Actually, I can't close the program with this function... You'll need to click on the X.")

#define NOT_FOUND " command not found"

/* MBR_H */
#endif

