#include <stdlib.h>
#include "kgame.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#define boardSize 4

int get_score(char value);


void kgame_init(kgame_t *game)
{
	if(!game) return;

	for(int i=0; i<KGAME_SIDES; i++) 
		for(int j=0; j<KGAME_SIDES; j++)
			game->board[i][j] = ' ';
			game->score = 0;
}

/**
 * Adds Random tiles to game 
 * 
 */
void kgame_add_random_tile(kgame_t *game)
{
	int row, col;
	srand(time(NULL));
	if(!game) return;
	int count = 0; 
	for(int i = 0; i < boardSize; i++){ // checks through board for non taken spaces
		for(int j = 0; j < boardSize; j++){
			if(game->board[i][j]!= ' '){
				count++; // 
			}
		}
	}
	if(count == 16) return; // if board is full return so game doesn't infinite loop (4x4)
	// find random, but empty tile
	// FIXME: will go to infinite loop if no empty tile
	do {
		row = rand() % 4;
		col = rand() % 4;
	} while(game->board[row][col] != ' ');

	// place to the random position 'A' or 'B' tile
	game->board[row][col] = 'A' + (rand() % 2);
}

/**
 * Renders the board 
 * 
 */

void kgame_render(char *output_buffer, const kgame_t *game)
{

	for(int z = 0; z < boardSize; z++){ // loops top half of game 
		sprintf(output_buffer++, "+");
		sprintf(output_buffer++, "-");
	}
	sprintf(output_buffer++, "+");
	sprintf(output_buffer++, "\n");

	//get string of game board
	for(int row = 0; row < boardSize; row++){

		//get character and store it
		sprintf(output_buffer++, "|"); // prints side and character if taken 
		for(int col = 0; col < boardSize; col++){
			sprintf(output_buffer++, "%c", game -> board[row][col]);
			sprintf(output_buffer++, "|");

		}
		sprintf(output_buffer++, "\n");
		for(int z = 0; z < boardSize; z++){ //prints bottom half 
			sprintf(output_buffer++, "+");
			sprintf(output_buffer++, "-");
		}
		sprintf(output_buffer++, "+");
		sprintf(output_buffer++, "\n");

		sprintf(output_buffer, "\nScore = %d", game->score); // print score 

	}

}

/**
 * Checks if game is won 
 * 
 */
bool kgame_is_won(const kgame_t *game)
{
	for(int i = 0; i < boardSize; i++){ // loops through until letter 'K' is found 
		for(int j = 0; j < boardSize; j++){
			if(game->board[i][j] == 'K'){
				return true;   
			}
		}
	}
	return false;
}


/**
 * checks valid moves
 * 
 */
bool kgame_is_move_possible(const kgame_t *game)
{
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			// checks vertical and horizonal if board is full 
			if(j<3){
				if(game->board[i][j] == game->board[i][j+1] && game->board[i][j] != ' '){ 
					return true;   
				}
			}
			if(i<3){
				if(game->board[i][j] == game->board[i+1][j] && game->board[i][j] != ' '){
					return true;   
				}

			}
			if(game->board[i][j] == ' ')
                return true;
		}
	}
	return false; 
}

/**
 *  rotates the board so that you can just turn and move instead of going through each iteration  
 */
void rotate(kgame_t *game){
	char temp[boardSize][boardSize]; 
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			temp[i][j] = game->board[i][j]; // creates and temp board and set to current board 
		}
	}

	for(int z = 0; z < boardSize; z++){
		for(int x = 0; x < boardSize; x++){
			game->board[z][x] = temp[boardSize - 1 - x][z];    // rotates board clockwise 
		}
	}   

}

/**
 *  movement of the pieces from shifting to merging 
 * 
 */

void move(kgame_t *game){ 

	bool validMove = kgame_is_move_possible(game); 

	if(validMove){ // checks if valid 
		// shifts the board to the right 
		for(int i = 0; i <boardSize; i++ ){
			for(int j= 0; j < boardSize-1; j++){
				for(int b = 0; b < boardSize -1; b++){
					if(game->board[i][b + 1] == ' '){ // checks if empty spaces and shifts it 
						char temp[boardSize][boardSize];
						temp[i][b] = game->board[i][b +1];
						game->board[i][b+1] = game->board[i][b];   
						game->board[i][b] = temp[i][b];
					}
				}
			}
		}
		// merge the pieces
		for(int i = 0; i < boardSize; i++){
			for(int j = boardSize-1; j > 0; j--){
				if(game->board[i][j-1] == game->board[i][j] && game->board[i][j - 1]!= ' ' && game->board[i][j]!= ' '){ // checks if two pieces are can be merged 
					char temp[boardSize][boardSize];
					game->board[i][j]++; // gets next character 
					game->score += get_score(game->board[i][j]); // gets score 
					game->board[i][j-1] = ' '; // makes one of the pieces empty so it 'merges' 

				}

			}
		}
		//shifts the board to right again so that non-empty spaces can be moved into spaces created from merging 
		for(int i = 0; i <boardSize; i++ ){
			for(int j= 0; j < boardSize-1; j++){
				for(int b = 0; b < boardSize -1; b++){
					if(game->board[i][b + 1] == ' '){
						char temp[boardSize][boardSize];
						temp[i][b] = game->board[i][b +1];
						game->board[i][b+1] = game->board[i][b];   
						game->board[i][b] = temp[i][b];
					}
				}
			}
		}
        kgame_add_random_tile(game);
	}
}

/**
 * Updates gane
 */
bool kgame_update(kgame_t *game, dir_t direction)
{
	switch(direction){
	//rotation is set to right so simply get it to move
	case RIGHT:
		move(game);
        return true;

	case LEFT:
		// rotates twice so right -> left = 2 rotations 
		rotate(game);
		rotate(game);
		move(game);
		rotate(game);
		rotate(game);
		return true;

	case UP:
		//rotates till UP from right 
		rotate(game);
		move(game);
		rotate(game);
		rotate(game);
		rotate(game);
		return true;

	case DOWN:
		//rotates till DOWN from right	
		rotate(game);
		rotate(game);
		rotate(game);
		move(game);
		rotate(game);
		return true;
	}

	return false;
}


/**
 *  gets score values and adds it to game score when returned 
 * 
 */
int get_score(char value){
	switch(value){
	case 'A' : return 2;
	case 'B' : return 4;
	case 'C' : return 8;
	case 'D' : return 16;
	case 'E' : return 32;
	case 'F' : return 64;
	case 'G' : return 128;
	case 'H' : return 256;
	case 'I' : return 512;
	case 'J' : return 1024;
	default : return 0;


	}
}

void kgame_save(const kgame_t *game)
{
	char ch;
	FILE *fp;
	fp = fopen("KGame.txt", "w"); // creates a file and writes to it 
	// goes through board and puts writes the characters and score 
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			ch = game->board[i][j]; 
			fputc(ch,fp); 
		}
	}
	fprintf(fp, "%d", game->score); 
	fclose(fp);

}


bool kgame_load(kgame_t *game) // i put "render = true" in main method in case LOAD so that it renders upon pressing 'l'
{	
	FILE *fl;
	int score;

	fl = fopen("KGame.txt", "r"); // opens saved file and reads 
	if(fl == NULL){ // checks if valid file

		return false;
	}
	// goes through the board and returns the values in the file to the game board 
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			game->board[i][j] = fgetc(fl);
            }

		}
	
	fscanf(fl, "%d",&score);
	game->score = score;
    

	fclose(fl);
    return true;
    
}


