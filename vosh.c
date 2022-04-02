#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define clear_terminal() printf("\033[H\033[J")
#define purple() printf("\033[1;35m")
#define reset_color() printf("\033[0m")
#define EXIT_OK 0
#define MAX_CMD_LEN 512

void vosh_loop(void);
void read_cmd(char *buf);
int parse_cmd(char *cmd, char **formatted_cmd);
int execute_cmd(int size, int pipes,  char **cmd);
void print_shell(void);
void print_wd(void);
int parent_wait(int children);
void exec_no_pipes(char **cmd);
int count_pipes(int size, int positions[], char **formatted_cmd);
void prepare_cmds(int pipes, int positions[], int cmd_amount, char **formatted_cmd, char ***seperated_cmd);
void save_to_history(void);
void *safe_alloc(int size_of_mem);

int main(int argc, char **argv) {
    clear_terminal();

    vosh_loop();

    // gör cleanup

    return EXIT_OK;
}

void print_shell(void) {
    purple();
    printf("#vosh ");
    reset_color();
    print_wd();
}

void vosh_loop(void) {
    int status = 1;
    while (status) {
        print_shell();

        // Read input
        char cmd_buffer[MAX_CMD_LEN];
        read_cmd(cmd_buffer);
        char **formatted_cmd = safe_alloc(MAX_CMD_LEN * 0.5 * sizeof(char *));

        // Parse input
        int amount_of_cmds = parse_cmd(cmd_buffer, formatted_cmd);
        int positions_of_pipes[(int) ((amount_of_cmds * 0.5) +1)]; // The max amount of well-formed pipes is amount-of_cmds/2
        int pipe_amount = count_pipes(amount_of_cmds, positions_of_pipes, formatted_cmd);

        // Execute
        if(pipe_amount == 0) {
            exec_no_pipes(formatted_cmd);
            status = parent_wait(1);
        } else {
            char ***seperated_cmds = (char ***)safe_alloc(pipe_amount * sizeof(char **));
            prepare_cmds(pipe_amount, positions_of_pipes, amount_of_cmds, formatted_cmd, seperated_cmds);
            /* sätt sista grejen till null (detta ska hända i count_pipes)
            formatted_cmd[amount_of_cmds] = (char *)safe_alloc(sizeof(char));
            formatted_cmd[amount_of_cmds] = (char *)NULL;
            */
            
        }    

        /*
        for(int i = 0 ; i < 3 ; i++) {
            printf("position %d = %d\n", i, positions_of_pipes[i]);
        }
        */

    

        
        

        // fria
    }
}

void read_cmd(char *buf){
    fgets(buf, MAX_CMD_LEN, stdin);
    if (strlen(buf) != 0) { save_to_history(); }
}

void remove_newline(char *arr) {
    arr[strcspn(arr, "\n")] = 0;
    arr[strcspn(arr, "\r")] = 0; // unnecessary? probably but test it
}

void save_to_history(void) {
    // save a command (not passwords)
}

// TODO: måste kolla efter citationstecken? vilka fler speciella tecken måste kollas?
// #, ", ', ><, $?, *, ;, " Tror typ inga av dessa, bara citationstecken

int parse_cmd(char *cmd, char **formatted_cmd) {
    int amount_of_cmds = 0;
    for (int i = 0; i < MAX_CMD_LEN * 0.5; i++) {
        char *tempbuf = strsep(&cmd, " ");
        if (tempbuf == NULL) {
            remove_newline(formatted_cmd[i - 1]); // Remove newline from last command or flag
            break;
        }
        else if (strlen(tempbuf) == 0){ i--; }
        else {
            formatted_cmd[i] = (char *)safe_alloc((strlen(tempbuf) + 1) * sizeof(char)); // +1 for \0
            strcpy(formatted_cmd[i], tempbuf);
        }
        amount_of_cmds = i + 1;
    }

    return amount_of_cmds;
}


// TODO Gör ett generellt fall på liknande sätt som i mexec? 
// Alternativt tänk bara ut hur man gör det här
void prepare_cmds(int pipes, int positions[], int cmd_amount, char **formatted_cmd, char ***seperated_cmd) {
    for(int i = 0 ; i <= pipes ; i++) {
        seperated_cmd[i] = (char **) safe_alloc(cmd_amount * sizeof(char *));
        int k = 0; 
        for(int j = i ; j < positions[i] ; j++) {
            seperated_cmd[i][k] = (char *) safe_alloc(MAX_CMD_LEN * sizeof(char));
            strcpy(seperated_cmd[i][k], formatted_cmd[j]);
            printf("copying %s into position %d %d, copied string is %s\n", formatted_cmd[j], i, k, seperated_cmd[i][k]);
            k++;
        }
    }
}

// size bör vara en array, pipes är storleken, vi behöver också faktiska pipes
int execute_cmd(int size, int pipes, char **cmd) {
    /*
        After parsing, check the list of built-in commands, and if present, execute it. If not,
    After parsing, check the list of built-in commands, and if present, execute it. If not,
        After parsing, check the list of built-in commands, and if present, execute it. If not,
        execute it as a system command. To check for built-in commands, store the commands in
    execute it as a system command. To check for built-in commands, store the commands in
        execute it as a system command. To check for built-in commands, store the commands in
        an array of character pointers, and compare all with strcmp().

        “cd” does not work natively using execvp, so it is a built-in command, executed with
    “cd” does not work natively using execvp, so it is a built-in command, executed with
        “cd” does not work natively using execvp, so it is a built-in command, executed with
        chdir().
    */


    // det här kan vi ha som special case i en egen funk
    int status = 1;
    for (int i = 0; i < pipes; i++) {
       
    }

    return status;
}

// Prints working directory
void print_wd(void) {
    char current_directory[MAX_CMD_LEN];
    getcwd(current_directory, MAX_CMD_LEN * sizeof(char));
    fprintf(stdout, "%s> ", current_directory);
    fflush(stdout);
}

// built in command for printing useful commands when hacking
void print_hack_cmds(char *arg) {}

void *safe_alloc(int size_of_mem) {
    void *v;
    if ((v = malloc(size_of_mem)) == NULL) {
        perror("failed malloc");
        exit(1);
    }
    return v;
}

int count_pipes(int size, int positions[], char **formatted_cmd) {
    int pos = 0;
    int amount = 0;
    for(int i = 0 ; i < size ; i++) {
        if(strcmp(formatted_cmd[i], "|") == 0) {
            amount++;
            positions[pos++] = i;
        }
    }

    return amount;
}

void exec_no_pipes(char **cmd) {
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork failed");
        exit(1);
    }

    if(pid == 0) {
        if (execvp(cmd[0], cmd) < 0) {
            perror(cmd[0]);
            exit(1);
        }
    }
}

int parent_wait(int children) {
    int status;
    for (int i = 0; i < children; i++) {
        if ((wait(&status)) < 0) {
            perror("wait error");
            exit(1);
        } else if (!(WIFEXITED(status))) {
            perror("childprocess terminated unexpectedly");
            exit(1);
        } else if (status != 0) {
            // This case is handled in main
            return -1;
        }
    }

    return 1;
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
