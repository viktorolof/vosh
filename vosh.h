#ifndef VOSH_H
#define VOSH_H

#include <stdbool.h>

/*
 *
 */
void vosh_loop(void);

/*
 *
 */
void read_cmd(char *buf);

/*
 *
 */
int parse_cmd(char *cmd, char **formatted_cmd);

/*
 *
 */
void print_shell(void);

/*
 *
 */
void print_wd(void);

/*
 *
 */
int parent_wait(int children);

/*
 *
 */
void exec_no_pipes(char **cmd, int cmd_amount);

/*
 *
 */
void exec_w_pipes(char ***cmds, int pipes[][2], int pipe_amount);

/*
 *
 */
void pipe_and_exec(char **formatted_cmd, int pipe_positions[], int cmd_amount,
                                        int pipe_amount);

/*
 *
 */
void children_pipe_handler(int pipes[][2], int pipe_amount, int child_number);

/*
 *
 */
int count_pipes(int size, int positions[], char **formatted_cmd);

/*
 *
 */
void prepare_cmds(int pipes, int positions[], int cmd_amount,
                     char **formatted_cmd, char ***seperated_cmd);

/*
 *
 */
void save_to_history(void);

/*
 *
 */
void *safe_alloc(int size_of_mem);

void free_formatted_cmds(char **cmds, int cmd_amount);

void free_seperated_cmds(char ***cmds, int pipe_amount);

void free_all_mem(char **formatted_cmd, char ***seperated_cmd, int pipe_amount, int cmd_amount);

/*
 *
 */
void open_pipes(int pipes[][2], int pipe_amount);

/*
 *
 */
void close_child_pipes(int pipes[][2], int pipe_amount);

/*
 *
 */
void close_pipes_first_child(int arr[][2], int pipe_amount);

/*
 *
 */
void close_pipes_last_child(int arr[][2], int pipe_amount, int index);

/*
 *
 */
void close_pipes_middle_child(int arr[][2], int pipe_amount, int index);

/*
 *
 */
void close_pipes(int arr[][2], int upper_limit);

/*
 *
 */
int is_built_in_cmd(char *cmd);

/*
 *
 */
void exec_built_in_cmd(int index, char** cmd);

void print_help(void);


#endif