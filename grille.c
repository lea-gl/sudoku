#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "grille.h"
#include "common.h"



grille* init_grille(int size){
    /* Alloue l'espace memoire d'une grille en 
    fonction de son nombre de cases par ligne 
    et initialise ses parametres statiques */
    
	grille* g;
	g = malloc(sizeof(grille));
	
	g->size = size;
	g->reg_size = sqrt(size);
	g->nb_cases = size * size;
	
	g->possibilities = malloc(sizeof(int*) * size);
	for( int i = 0; i < size; i++)
		g->possibilities[i] = calloc(size, sizeof(int));
		
	return g;
}



grille* load_grille(const char* nomfichier){
    /* Renvoie la grille correspondant a un fichier texte */

    int size_g = size_count(nomfichier);
    
    grille* g;
	char c;
	
	g = init_grille(size_g);
	
	int i = 0;
	int j = 0;
	
	char prev_c = ' ';
	int nombre = 0;
	
	FILE* file = fopen(nomfichier, "r");
	
	c = fgetc(file);
	while(prev_c != EOF){
	    // si le caractère précédent est un nombre
	    if (prev_c != ' ' && prev_c != '\n'){
	        nombre = nombre * 10 + (prev_c - '0');
	        
	        // si le nombre est fini
	        if (c == ' ' || c == '\n' || c == EOF){
	            if(nombre != 0) 
	                g->possibilities[i][j] = 1<<nombre;
	            else
	                g->possibilities[i][j] = (1<<(g->size+1))-2;
	            
	            if (i == g->size)
	                error_format();
	            if (j == g->size - 1) {
	                j = 0;
	                i++;
                }
	            else
	                j++;
	            nombre = 0;
	        }
	    }

	    prev_c = c;
	    c = fgetc(file);
	}
	
	fclose(file);
	
	return g;
}



int size_count(const char* nomfichier){
    /* Renvoie le nombre de cases par ligne 
    d'une grille notee dans un fichier texte */
    
    FILE* file = fopen(nomfichier, "r");
	if (!file) error_file();
	
    int c = '0', count;

    count = 1;
    while( count < SIZE_MAX )
    {
        c = fgetc( file );
        if(c == ' '){
            while( (c = fgetc( file )) == ' ');
            ++count;
            if(c == '\n')
                count--;
        }
        if( c == EOF || c == '\n' )
            break;
    }

    fclose(file);
    
    return count;
}



grille* grille_copy(grille* g){
    /* Renvoie une copie d'une grille g */
    
    int size = g->size;
    grille* g_cop = init_grille(size);
    
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            g_cop->possibilities[i][j] = g->possibilities[i][j];
        }
    }
    return g_cop;
}



void free_grille(grille* g) {
    /* Libere l'espace memoire d'une grille */
    
    for (int i=0; i<g->size; i++)
        free(g->possibilities[i]);
    free(g->possibilities);
    free(g);
}



void Afficher(grille* G) {
    /* Affiche une grille */
    
    unsigned char i,j,bit;
    unsigned int valeur;
    int n = G->size;
    int reg_size = G->reg_size;
    for (i = 0; i < n; i++) {
        if (i % reg_size == 0) 
            printf("\n");      
        printf("%2u",i + 1); 
        for (j = 0; j < n; j++) {
            if (j % reg_size == 0)
                printf("  ");
            valeur = G->possibilities[i][j];
            bit = 0;
            if (poids(valeur) == 1) {
                while (valeur > 1UL) {
                    valeur = valeur >> 1;
                    bit++;
                }
                printf("\033[1;33m\033[40m%02u \033[0m",bit);
            }
            else
                printf("\033[0;36m\033[40m%02u \033[0m",poids(G->possibilities[i][j]));
        }
      printf("\n");
    }
}



int* MemeLigne(grille* g, int case_i, int case_j){
    /* Renvoie une liste d'indices correspondant aux
    cases de la ligne de la case case_i, case_j */
    
    int* index_cases = malloc(g->size*sizeof(int));
    int position = case_i*g->size;
    for (int j=0; j<g->size;j++){
        index_cases [j] = position+j;
    }
    return index_cases;
}


int* MemeColonne(grille* g, int case_i, int case_j){
    /* Renvoie une liste d'indices correspondant aux
    cases de la colonne de la case case_i, case_j */
    
    int* index_cases = malloc(g->size*sizeof(int));
    int position = case_j;
    int size = g->size;
    for (int i=0; i<size; i++){
        index_cases [i] = i*size + position;
    }
    return index_cases;
}


int* MemeRegion(grille* g, int case_i, int case_j){
    /* Renvoie une liste d'indices correspondant aux
    cases de la region de la case case_i, case_j */
    
    int size = g->size;
    int* index_cases = malloc(size*sizeof(int));
    int cpt = 0;
    int reg_size = g->reg_size;
    int deb_regi = case_i - (case_i % reg_size);
    int deb_regj = case_j - (case_j % reg_size);
    for(int i=deb_regi; i<deb_regi+reg_size; i++){
        for (int j=deb_regj; j<deb_regj+reg_size; j++){
            index_cases [cpt] = i*size + j;
            cpt++;
        }
    }
    return index_cases;
}


