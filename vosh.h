#ifndef VOSH_H
#define VOSH_H

#include <stdbool.h>

/*
 * The eternal shell loop.
 */
void vosh_loop(void);

/*
 * Reads a line from stdin.
 */
void read_cmd(char *buf);

/*
 * Parses a command.
 */
int parse_cmd(char *cmd, char **formatted_cmd);

/*
 * Prints the shell line.
 */
void print_shell(void);

/*
 * Prints the working directory.
 */
void print_wd(void);

/*
 * Waits for children processes to terminate.
 */
int parent_wait(int children);

/*
 * Executes a command without piping.
 */
void exec_no_pipes(char **cmd, int cmd_amount);

/*
 * Forks and executes commands.
 */
void exec_w_pipes(char ***cmds, int pipes[][2], int pipe_amount);

/*
 * Handles the execution of piped commands.
 */
void pipe_and_exec(char **formatted_cmd, int pipe_positions[], int cmd_amount,
                                        int pipe_amount);

/*
 * Depending on the process, pipes are duped and closed accordingly.
 */
void children_pipe_handler(int pipes[][2], int pipe_amount, int child_number);

/*
 * Counts the amount of pipes and saves their positions.
 */
int count_pipes(int size, int positions[], char **formatted_cmd);

/*
 * Prepares commands for execution with pipes. Seperates the previously formatted
 * commands where the pipes where found.
 */
void prepare_cmds(int pipes, int positions[], int cmd_amount,
                     char **formatted_cmd, char ***seperated_cmd);

/*
 * Not implemented yet...
 */
void save_to_history(void);

/*
 * Allocates memory safely.
 */
void *safe_alloc(int size_of_mem);

/*
 * Functions for freeing memory.
 */
void free_formatted_cmds(char **cmds, int cmd_amount);
void free_seperated_cmds(char ***cmds, int pipe_amount);

/*
 * Opens pipes.
 */
void open_pipes(int pipes[][2], int pipe_amount);

/*
 * Dupes the necessary filedescriptors and closes pipes. Different cases for different children.
 */
void close_pipes_first_child(int arr[][2], int pipe_amount);
void close_pipes_last_child(int arr[][2], int pipe_amount, int index);
void close_pipes_middle_child(int arr[][2], int pipe_amount, int index);

/*
 * Closes all pipes in the array from arr[0] to arr[upper_limit]
 */
void close_pipes(int arr[][2], int upper_limit);

/*
 * Checks if a given command is built in or not.
 */
int is_built_in_cmd(char *cmd);

/*
 * Executes a built in command.
 */
void exec_built_in_cmd(int index, char** cmd);

/*
 * Prints some help to the user.
 */
void print_help(void);

/*
 * Prints the rules of the mastermind game.
 */
void print_mastermind(void);

void play_mastermind(void);

void randomize_colors(char *answer);

int compare_strings(char *computer, char *player);

void print_win(void);

void print_loss(void);



#endif