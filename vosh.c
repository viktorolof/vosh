#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <time.h>
#include "vosh.h"
#include "mastermind.h"

#define clear_terminal() printf("\033[H\033[J")
#define purple() printf("\033[1;35m")
#define blue() printf("\033[1;34m")
#define red() printf("\033[1;31m")
#define green() printf("\033[1;32m")
#define yellow() printf("\033[0;33m")
#define cyan() printf("\033[1;36m")
#define white() printf("\033[1;37m")
#define reset_color() printf("\033[0m")
#define EXIT_FAIL 1
#define MAX_CMD_LEN 512
#define READ_END 0
#define WRITE_END 1
#define VOSH_CMD_SIZE 4

char *vosh_cmds[VOSH_CMD_SIZE] = {"cd", "baggen", "help", "mmind"};

int main(void) {
    clear_terminal();

    vosh_loop();

    return EXIT_SUCCESS;
}

void print_shell(void) {
    purple();
    printf("#vosh ");
    blue();
    print_wd();
    reset_color();
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
        int positions_of_pipes[(int) ((amount_of_cmds * 0.5) +1)]; 
        int pipe_amount = count_pipes(amount_of_cmds, positions_of_pipes, formatted_cmd);

        // Execute
        if(pipe_amount == 0) {
            int command;
            if((command = is_built_in_cmd(formatted_cmd[0])) != -1) {
                exec_built_in_cmd(command, formatted_cmd);
                free_formatted_cmds(formatted_cmd, amount_of_cmds);
            } else {
                exec_no_pipes(formatted_cmd, amount_of_cmds);
                status = parent_wait(1);
            }
        } else {
            pipe_and_exec(formatted_cmd, positions_of_pipes, amount_of_cmds, pipe_amount);
            status = parent_wait(pipe_amount + 1);
        }    
    }
}

void pipe_and_exec(char **formatted_cmd, int pipe_positions[], int cmd_amount, int pipe_amount) {
    // Prepare commands for execution
    char ***seperated_cmds = (char ***)safe_alloc((pipe_amount + 1) * sizeof(char **));
    prepare_cmds(pipe_amount, pipe_positions, cmd_amount, formatted_cmd, seperated_cmds);

    // Open pipes 
    int pipes[pipe_amount][2];
    open_pipes(pipes, pipe_amount);
    
    // Fork and execute the processes, close pipes for parent process
    exec_w_pipes(seperated_cmds, pipes, pipe_amount);
    close_pipes(pipes, pipe_amount);

    free_seperated_cmds(seperated_cmds, pipe_amount);
}

void read_cmd(char *buf){
    fgets(buf, MAX_CMD_LEN, stdin);
    if (strlen(buf) != 0) { save_to_history(); }
}

void remove_newline(char *arr) {
    arr[strcspn(arr, "\n")] = 0;
    arr[strcspn(arr, "\r")] = 0;
}

void save_to_history(void) {
    // save a command (not passwords)
}

int parse_cmd(char *cmd, char **formatted_cmd) {
    int amount_of_cmds = 0;
    for (int i = 0; i < MAX_CMD_LEN * 0.5; i++) {
        char *tempbuf = strsep(&cmd, " ");
        if (tempbuf == NULL) {
            remove_newline(formatted_cmd[i - 1]);
            break;
        }
        else if (strlen(tempbuf) == 0){ i--; }
        else {
            formatted_cmd[i] = (char *)safe_alloc((strlen(tempbuf) + 1) * sizeof(char)); 
            strcpy(formatted_cmd[i], tempbuf);
        }
        amount_of_cmds = i + 1;
    }

    return amount_of_cmds;
}


void prepare_cmds(int pipes, int positions[], int cmd_amount, char **formatted_cmd, char ***seperated_cmd) {
    int counter = 0;
    positions[pipes] = -1; // define last position in array to avoid undefined behaviour

    // Loop for breaking up formatted_cmd into seperated, executable commands
    for(int i = 0 ; i <= pipes ; i++) {
        seperated_cmd[i] = (char **) safe_alloc(cmd_amount * sizeof(char *));
        for(int j = 0 ; j < cmd_amount ; j++) {
            if(counter == positions[i] || counter == cmd_amount) {
                // final position in command needs to be NULL for execvp
                seperated_cmd[i][j] = (char *) NULL;
                j = cmd_amount; 
            } else {
                seperated_cmd[i][j] = (char *) safe_alloc(MAX_CMD_LEN * sizeof(char));
                strcpy(seperated_cmd[i][j], formatted_cmd[counter]);
            }
            counter++;
        }
    }

    free_formatted_cmds(formatted_cmd, cmd_amount);
}

void print_wd(void) {
    char current_directory[MAX_CMD_LEN];
    getcwd(current_directory, MAX_CMD_LEN * sizeof(char));
    fprintf(stdout, "%s> ", current_directory);
    fflush(stdout);
}

void *safe_alloc(int size_of_mem) {
    void *v;
    if ((v = malloc(size_of_mem)) == NULL) {
        perror("failed malloc");
        exit(1);
    }
    return v;
}

void free_formatted_cmds(char **cmds, int cmd_amount) {
    for(int i = 0 ; i < cmd_amount ; i++) {
        free(cmds[i]);
    }
    free(cmds);
}

void free_seperated_cmds(char ***cmds, int pipe_amount) {
    int i = 0;
    for(int j = 0 ; j <= pipe_amount ; j++) {
        while(cmds[j][i] != NULL) {
            free(cmds[j][i]);
            i++;
        }
        free(cmds[j][i]);
        free(cmds[j]);
        i = 0;
    }
    free(cmds);
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

void exec_no_pipes(char **cmd, int cmd_amount) {
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork failed");
        exit(1);
    }

    if(pid == 0) {   
        // child case
        if (execvp(cmd[0], cmd) < 0) {
            perror(cmd[0]);
            free_formatted_cmds(cmd, cmd_amount);
            exit(EXIT_FAIL);
        }     
    } else {
        // parent case
        free_formatted_cmds(cmd, cmd_amount);
    }
}

void exec_w_pipes(char ***cmds, int pipes[][2], int pipe_amount) {
    for(int i = 0 ; i <= pipe_amount ; i++) {
        pid_t pid;
        if ((pid = fork()) < 0) {
            perror("fork failed");
            exit(EXIT_FAIL);
        }
    
        if(pid == 0) {
            // child case
            children_pipe_handler(pipes, pipe_amount, i);
            if(execvp(cmds[i][0], cmds[i]) < 0) {
                perror(cmds[i][0]);
                free_seperated_cmds(cmds, pipe_amount);
                exit(EXIT_FAILURE);
            }
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

void open_pipes(int pipes[][2], int pipe_amount) {
    for (int i = 0; i < pipe_amount; i++) {
        if (pipe(pipes[i]) != 0) {
            perror("pipe failed");
        }
    }
}

void children_pipe_handler(int pipes[][2], int pipe_amount, int child_number) {
    if(child_number == 0) {
        close_pipes_first_child(pipes, pipe_amount);
    } else if (child_number == pipe_amount) {
        close_pipes_last_child(pipes, pipe_amount, child_number);
    } else {
        close_pipes_middle_child(pipes, pipe_amount, child_number);
    }
}

void close_pipes_first_child(int arr[][2], int pipe_amount) {
    // The first child will set its output to the first pipes write end
    if (dup2(arr[0][WRITE_END], STDOUT_FILENO) < 0) {
        perror("dup failed first child");
        exit(1);
    }

    close_pipes(arr, pipe_amount);
}

void close_pipes_last_child(int arr[][2], int pipe_amount, int index) {
    // The last child will set its input from the previous pipes read end
    if (dup2(arr[index - 1][READ_END], STDIN_FILENO) < 0) {
        perror("dup failed");
        exit(1);
    }

    close_pipes(arr, pipe_amount);
}

void close_pipes_middle_child(int arr[][2], int pipe_amount, int index) {
    // the middle child will set its input from the previous pipes read end and its output to the next pipes write end.
    if (dup2(arr[index - 1][READ_END], STDIN_FILENO) < 0) {
        perror("dup failed");
        exit(1);
    }
    if (dup2(arr[index][WRITE_END], STDOUT_FILENO) < 0) {
        perror("dup failed");
        exit(1);
    }

    close_pipes(arr, pipe_amount);
}

void close_pipes(int arr[][2], int upper_limit) {
    // closes all pipes
    if ((upper_limit >= 0)) {
        for (int i = 0; i < upper_limit; i++) {
            close(arr[i][READ_END]);
            close(arr[i][WRITE_END]);
        }
    }
}



/***** FUNCTIONS FOR BUILT IN COMMANDS *******/
int is_built_in_cmd(char *cmd) {
    for(int i = 0 ; i < VOSH_CMD_SIZE ; i++) {
        if(strcmp(vosh_cmds[i], cmd) == 0) {
            return i;
        }
    }
    return -1;
}

void print_help(void) {
    purple();
    printf("******** VOSHELL HELP ********\n");
    reset_color();
    printf("List of built in commands:"
        "\n> help - displays help"
        "\n> baggen - baggen rolls in"
        "\n> mmind - play a game of mastermind"
        "\n--------------------------"
        "\nRegular commands for linux-terminals are supported"
        "\nShell can also handle multiple pipes\n\n");
}


void exec_built_in_cmd(int index, char** cmd) {
    switch(index) {
        case 0 :{
            chdir(cmd[1]);
            break;
        }
        case 1: {
            char **arr = safe_alloc(3 * sizeof(char *));
            arr[0] = "cat";
            arr[1] = "baggen";
            arr[2] = NULL;
            exec_no_pipes(arr, 0);
            parent_wait(1);
            break;
        }
        case 2: {
            print_help();
            break;
        }
        case 3: {
            play_mastermind();
        }
    }
    
}