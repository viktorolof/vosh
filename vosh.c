#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define clear_terminal() printf("\033[H\033[J")
#define purple() printf("\033[1;35m")
#define reset_color() printf("\033[0m")
#define EXIT_OK 1
#define MAX_CMD_LEN 512

void vosh_loop(void);
void read_cmd(char *buf);
void parse_cmd(char *cmd, char **formatted_cmd);
int execute_cmd(char**);
void start_shell(void);
void print_wd(void);
void save_to_history(void);
void *safe_alloc(int size_of_mem);

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
    char cmd_buffer[MAX_CMD_LEN];
    read_cmd(cmd_buffer);
    char **formatted_cmd = safe_alloc(MAX_CMD_LEN * 0.5 * sizeof(char *));
    
    parse_cmd(cmd_buffer, formatted_cmd);


    // debug print
    for(int i = 0 ; i < 100 ; i++) {
        if(formatted_cmd[i] == NULL) {
            break;
        }
        printf("%s i loopn\n", formatted_cmd[i]);
    }

    
    // parse - seperate command string into a program and arguments
    // Execute - run the  command
   
}

void read_cmd(char *buf) {
    fgets(buf, MAX_CMD_LEN, stdin);
    if(strlen(buf) != 0) {
        save_to_history();
    }
}

void remove_newline(char *arr) {
    arr[strcspn(arr, "\n")] = 0;
    arr[strcspn(arr, "\r")] = 0; //unnecessary? probably but test it
}

void save_to_history(void) {
    // save a command (not passwords)
}

// bör skicka med en int * för att kolla hur många cmds och flaggor?
void parse_cmd(char *cmd, char **formatted_cmd) {
    for(int i = 0 ; i < MAX_CMD_LEN * 0.5 ; i++) {
        char *tempbuf = strsep(&cmd, " ");
        if(tempbuf == NULL) {
            remove_newline(formatted_cmd[i -1]); // Remove newline from last command or flag
            break;
        } else if(strlen(tempbuf) == 0) {
            // vad fångar detta för cases?
            i--;
        } else{
            printf("tempbuf är: %s\n", tempbuf);
            formatted_cmd[i] = (char *)safe_alloc((strlen(tempbuf) + 1) * sizeof(char)); // +1 for \0
            strcpy(formatted_cmd[i], tempbuf);
        }
    }
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
    char current_directory[MAX_CMD_LEN];
    getcwd(current_directory, MAX_CMD_LEN * sizeof(char));
    fprintf(stdout, "%s> ", current_directory);
    fflush(stdout);
}

// built in command for printing useful commands when hacking
void print_hack_cmds(char *arg) {

}

void *safe_alloc(int size_of_mem) {
    void *v;
    if ((v = malloc(size_of_mem)) == NULL) {
        perror("failed malloc");
        exit(1);
    }
    return v;
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
