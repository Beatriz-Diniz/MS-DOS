#include <tyos.h>
#include <mbr.h>
#include <runtime.h>
#include <utils.h>

int shell(){
        char cmd[BUFFER_MAX_LENGTH];
        /* Clear screen */
        clear();
        
        /* Main loop */
        print(NL INIT_MSG NL);	
        while(1){
                print(PROMPT);                          /* Show prompt. */
                read(cmd);                              /* Read user command. */
               
                /* Process user command. */
                if(compare(cmd, HELP_CMD))              /* Command help. */
                        help();
                else if(compare(cmd, DATE_CMD))         /* Command date*/
                        date();
                else if(compare(cmd, TIME_CMD))
                        time();
                else if(compare(cmd, QUIT_CMD))         /* Commandquit */
                        quit();
                else if(compare(cmd, CLEAR_CMD))
                        clear();
                else{
                        print(cmd);
                        printnl(NOT_FOUND);
                }
        }
        return 0;
}
