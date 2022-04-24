/* 
 * Prints the starting screen and guide for mastermind.
 */
void print_mastermind(void);

/*
 * The computer randomly selects 4 colors for the player guess.
 */
void randomize_colors(char *answer);   

/*
 * Compares two strings, returns the amount of letters on the correct position.
 */
int compare_strings(char *computer, char *player);

/*
 * Plays a game of mastermind.
 */
void play_mastermind(void);

/*
 * Prints the win screen.
 */
void print_win(char *res);

/*
 * Prints the win screen.
 */
void print_loss(char *res);

/*
 * Prints the selected colors by the computer.
 */
void print_computer_answer(char *answer);