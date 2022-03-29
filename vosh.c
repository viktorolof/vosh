#include <stdio.h>
#include<unistd.h>

#define clear_terminal() printf("\033[H\033[J")
#define purple() printf("\033[1;35m")
#define reset_color() printf("\033[0m")
#define EXIT_OK 1


void vosh_loop(void);
char *read_cmd(void);
char **parse_cmd(void);
int execute_cmd(char**);
void start_shell(void);
void print_wd(void);

int main(int argc, char **argv) {
    // kanske man kan ta in grejer för att göra nåt kul?

    start_shell();
    // Load config, if any 
    /*
    In this step, a typical shell would read and execute its
    configuration files. These change aspects of the shell’s
    behavior.
    */

    // Command loop
    vosh_loop();

    /*
    Next, the shell reads commands from stdin (which could be 
    interactive, or a file) and executes them.
    */

    // Cleanup
    /*
    After its commands are executed, the shell executes any 
    shutdown commands, frees up any memory, and terminates.
    */

    return EXIT_OK;
}

void start_shell(void) {
    clear_terminal();
    purple();
    printf("#vosh ");
    reset_color();
    print_wd();
}

void vosh_loop(void) {
    while(1) {

    }
    // read one line from stdin
    // parse - seperate command string into a program and arguments
    // Execute - run the  command

}

char *read_cmd(void) {
    // Command is entered and if length is non-null, keep it in history. Dont save passwords.
}

char **parse_cmd(void) {

}

int execute_cmd(char** cmd) {

}

// Prints working directory and 
void print_wd(void) {
    char current_directory[512];
    getcwd(current_directory, 512 * sizeof(char));
    fprintf(stdout, "%s> ", current_directory);
    fflush(stdout);
}

// built in command for printing useful commands when hacking
void print_hack_cmds(char *arg) {

}

// built in command for printing most used commands in the history file

/*  Pseudo code  */
/*
Command is entered and if length is non-null, keep it in history.

Parsing, break up commands and arguments into individual words and strings

Checking for special characters like pipes, etc is done

Checking if built-in commands are asked for.
    - skapa ett par built in commands

If pipes are present, handling pipes.
    - kika mexec lösningen

Executing system commands and libraries by forking a child and calling execvp.

Printing current directory name and asking for next input
    - Printing the directory can be done using getcwd.
*/
