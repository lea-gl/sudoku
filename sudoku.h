#ifndef SUDOKU_H
#define SUDOKU_H
#include "grille.h"

void init_sudoku(grille* g);

int exclusivite(grille* g, int* list_cases, int ind_i, int ind_j, int val);
int unicite(grille * g, int* indices);
int parite(grille* g, int* indices);

int update_new_val(grille* g, int i, int j);

int bouclerRegles(grille* g);

int sudoku(grille* g);

int sudoku_solver(char * filename);

int parite_complexe(grille * g, int* indices, int length_max);
int nuplet(grille* g, int* indices, int nb);


#endif
