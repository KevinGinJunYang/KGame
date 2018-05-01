#include <stdlib.h>
#include "kgame.h"


void kgame_init(kgame_t *game)
{
    if(!game) return;
    
    for(int i=0; i<KGAME_SIDES; i++) 
        for(int j=0; j<KGAME_SIDES; j++)
            game->board[i][j] = ' ';
    game->score = 0;
}

void kgame_add_random_tile(kgame_t *game)
{
	int row, col;
	
	if(!game) return;

	// find random, but empty tile
	// FIXME: will go to infinite loop if no empty tile
	do {
		row = rand() % 4;
		col = rand() % 4;
	} while(game->board[row][col] != ' ');

	// place to the random position 'A' or 'B' tile
	game->board[row][col] = 'A' + (rand() % 2);
}


void kgame_render(char *output_buffer, const kgame_t *game)
{
    // FIXME: Implement correctly (task 1)
}


bool kgame_is_won(const kgame_t *game)
{
    // FIXME: Implement correctly (task 2)
    return false;
}


bool kgame_is_move_possible(const kgame_t *game)
{
    // FIXME: Implement correctly (task 3)
    return true;
}


bool kgame_update(kgame_t *game, dir_t direction)
{
    // FIXME: Implement correctly (task 4)
    return true;    
}


void kgame_save(const kgame_t *game)
{
    // FIXME: Implement correctly (task 5)
    char ch;
    FILE *fp;
    fp = fopen("KGame.txt", "w");
    
    for(int i = 0; i < boardSize; i++){
     for(int j = 0; j < boardSize; j++){
      ch = game->board[i][j];
      putc(ch,fp);
     }
    }
    fprintf(fp, "%d", game->score);
    fclose(fp);
    printf("Saved");
}


bool kgame_load(kgame_t *game)
{
    // FIXME: Implement correctly (task 6)
    FILE *fl;
    int score;
    
    fl = fopen("KGame.txt", "r");
    if(fl == NULL){
     printf("That is invalid");
     return false;
    }
    
    for(int i = 0; i < boardSize; i++){
     for(int j = 0; j < boardSize; j++){
         game->board[i][j] = fgetc(fl);
         
     }
    }
    fscanf(fl, "%d",&score);
    game->score = score;
    fclose(fl);
    
}


