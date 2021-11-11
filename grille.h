#ifndef GRILLE_H
#define GRILLE_H

#define SIZE_MAX 256

typedef struct s_grille
{
    int ** possibilities; // entier de n bits. bits a 1 pour les possibilites
    int size;
    int reg_size;
    int nb_cases;
} grille ;


grille* init_grille(int size);

grille* load_grille(const char* nomfichier);
int size_count(const char* nomfichier);

grille* grille_copy(grille* g);

void free_grille(grille* g);

void Afficher(grille* G);



int* MemeLigne(grille* g, int case_i, int case_j);
int* MemeColonne(grille* g, int case_i, int case_j);
int* MemeRegion(grille* g, int case_i, int case_j);

#endif
