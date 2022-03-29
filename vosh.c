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
    start_shell();
    
    vosh_loop();

    // gör cleanup

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
    // Get the inputted command and store it in the buffer
    char buf[512];
    fgets(buf, 512, stdin);
    fprintf(stdout, "buffer: %s\n", buf);

    
    // parse - seperate command string into a program and arguments
    // Execute - run the  command

}

char *read_cmd(void) {
    // Command is entered and if length is non-null, keep it in history. Dont save passwords.
}

char **parse_cmd(void) {
 /*
    Parsing can be done by using strsep(“”). It will separate words based on spaces. Always 
    skip words with zero length to avoid storing of extra spaces.
 */
}

int execute_cmd(char** cmd) {
/*
    After parsing, check the list of built-in commands, and if present, execute it. If not, 
    execute it as a system command. To check for built-in commands, store the commands in 
    an array of character pointers, and compare all with strcmp().

    “cd” does not work natively using execvp, so it is a built-in command, executed with 
    chdir().
*/
}

// Prints working directory
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
Command is entered and if length is non-null, keep it in history, else break and call init_shell().

Parsing, break up commands and arguments into individual words and strings

Checking for special characters like pipes, etc is done

Checking if built-in commands are asked for.
    - skapa ett par built in commands

If pipes are present, handling pipes.
    - kika mexec lösningen

Executing system commands and libraries by forking a child and calling execvp.

Printing current directory name and asking for next input
*/

// DEPENDENCIES
/*
sudo apt-get install libreadline-dev


*/
