#ifndef MBR_H
#define MBR_H

/* Core functions */

/* Print the null-terminated buffer on standard output */
void date();
void time();
void quit();

/* Commands */

/* Print a help message */
#define HELP_CMD "help"

/* Prints date */
#define DATE_CMD "date"

/* Prints time*/
#define TIME_CMD "time"

/* Quit */
#define QUIT_CMD "quit"

/* Clear */
#define CLEAR_CMD "clear"

#define NOT_FOUND " command not found"

/* MBR_H */
#endif

