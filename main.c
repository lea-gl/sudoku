#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
#include "grille.h"
#include "common.h"

int main(int argc, char** argv)
{    
    if(argc == 1) {
        printf("grille_sujet :");
        sudoku_solver("grille_sujet.txt");
    }
    else {
        printf("%s", argv[1]);
        sudoku_solver(argv[1]);
    }
   
	return 0;
}
