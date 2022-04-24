#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "mastermind.h"

/* Colors */
#define purple() printf("\033[1;35m")
#define blue() printf("\033[1;34m")
#define red() printf("\033[1;31m")
#define green() printf("\033[1;32m")
#define yellow() printf("\033[0;33m")
#define cyan() printf("\033[1;36m")
#define white() printf("\033[1;37m")
#define reset_color() printf("\033[0m")

void print_mastermind(void) {
    printf("*********** MASTER MIND **********\n");
    printf("The computer has chosen 4 unknown\n");
    printf(" * symbols in different colors.  \n");
    printf("There are seven different colors:\n");
    red();
    printf(" R");
    green();
    printf(" G");
    yellow();
    printf(" Y");
    blue();
    printf(" B");
    purple();
    printf(" P");
    cyan();
    printf(" C");
    white();
    printf(" W\n\n");
    printf("You have 10 rounds to guess what\n");
    printf("colors were chosen. Each round you\n");
    printf("will pick 4 colors and their positions.\n");
    printf("Format your guesses without any spaces \nlike this:\n");
    printf("RYBC (Red, Yellow, Blue, Cyan)\n\n");
    printf("Each round you will know how many\n");
    printf("guesses were correct and in the\n");
    printf("correct position.\n\n");
}

void randomize_colors(char *answer) {
    char buf[5];
    srand(time(NULL));
    
    for(int i = 0 ; i < 4 ; i++) {
        int x = rand() % 7;

        switch(x) {
            case 0: {
                buf[i] = 'R';
                break;
            }
            case 1: {
                buf[i] = 'G';
                break;
            }
            case 2: {
                buf[i] = 'Y';
                break;
            }
            case 3: {
                buf[i] = 'B';
                break;                
            }
            case 4: {
                buf[i] = 'P';
                break;
            }
            case 5: {
                buf[i] = 'C';
                break;
            }
             case 6: {
                buf[i] = 'W';
                break;
            }
        }
    }
    buf[4] = '\0';
    strcpy(answer, buf);
}

int compare_strings(char *computer, char *player) {
    int green_indexes[] = {-1, -1, -1, -1};
    int yellow_indexes[] = {-1, -1, -1, -1};
    
    for(int i = 0 ; i < 4 ; i++) {
        if(player[i] == computer[i]) {
            // if correct color is at correct position save the position
            green_indexes[i] = i;
            
            // if there was a character of the same sort found already, overwrite it
            for(int k = 0 ; k < 4 ; k++) {
                if(yellow_indexes[k] == i) {
                    yellow_indexes[k] = -1;
                }
            }

            if(yellow_indexes[i] == i) {
                yellow_indexes[i] = -1;
            }
        } else {
            for(int j = 0 ; j < 4 ; j++) {
                // check if it is a correct color but wrong position
                if(player[i] == computer[j] && green_indexes[j] == -1) {
                    // if the computer character is already used ~ if(yellow.contains)
                    bool not_used = true;
                    for(int k = 0 ; k < 4 ; k++) {
                        if(yellow_indexes[k] == j) {
                            not_used = false;
                        }
                    }

                    if(not_used) {
                        yellow_indexes[i] = j;
                    }
                }
            }
        }
    }

    int g_res = 0;
    int y_res = 0;
    for(int i = 0 ; i < 4 ; i++) {
        if(green_indexes[i] != -1) {
            g_res++;
        } else if(yellow_indexes[i] != -1) {
            y_res++;
        } 
    }

    if(g_res > 0) {
        green();
        printf("%d correct and correct position\n", g_res);
    }
    if(y_res > 0) {
        yellow();
        printf("%d correct but wrong position\n", y_res);

    }
    if(g_res == 0 && y_res == 0) {
        white();
        printf("0 correct guesses\n");
    }
    reset_color();

    return g_res;
}

void play_mastermind(void) {
    print_mastermind();
    char *answer = calloc(4,  sizeof(char));
    randomize_colors(answer);

    bool won = false;
    for(int i = 0 ; i < 10 ; i++) {
        printf("Round %d\n", i + 1);
        char buf[10];
        fgets(buf, 10, stdin);
        if(compare_strings(answer, buf) == 4) {
            won = true;
            break;
        }
    }

    (won) ? (print_win(answer)) : (print_loss(answer));

    free(answer);
}

void print_win(char *res) {
    green();
    printf("Congrats, you won.\n");
    printf("The answer was: ");
    print_computer_answer(res);
    reset_color();
    printf("**********************************\n");

}

void print_loss(char *res) {
    red();
    printf("Smh my head... you lost..\n");
    printf("The answer was: ");
    print_computer_answer(res);
    reset_color();
    printf("**********************************\n");
}

void print_computer_answer(char *answer) {
    for(int i = 0 ; i < 4 ; i++) {
        char c = answer[i];
        switch(c) {
            case 'R': {
                red();
                break;
            }
            case 'G': {
                green();
                break;
            }
            case 'B': {
                blue();
                break;
            }
            case 'C': {
                cyan();
                break;
            }
            case 'Y': {
                yellow();
                break;
            }
            case 'P': {
                purple();
                break;
            }
            case 'W': {
                white();
                break;
            }
        }
        printf("%c", c);
    }
    printf("\n");
}