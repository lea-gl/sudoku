#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sudoku.h"
#include "grille.h"
#include "common.h"


void init_sudoku(grille* g){
    /* Initialisation des possibilites du sudoku */
    
    // On parcourt la grille
    for (int i=0; i<g->size; i++){
        for(int j=0; j<g->size; j++){
        
            // En mettant a jour les possibilites des
            // cases impactees par une valeur connue
            if(poids(g->possibilities[i][j])==1){
                update_new_val(g, i, j);
            }
        }
    }
}



int exclusivite(grille* g, int* list_cases, int ind_i, int ind_j, int val){
    /* Met a jour les possibilites d'un bloc lorsque la valeur val d'une case ij a ete trouvee,
    en l'enlevant des autres cases du bloc */
    
    int modified = 0;
    int size = g->size;
    
    // On parcourt la liste des cases donnees
    for(int case_no=0; case_no<size; case_no++){
        int case_i = list_cases[case_no]/size;
        int case_j = list_cases[case_no]%size;
        
        if(case_i != ind_i || case_j != ind_j){
            int valeur_case = g->possibilities[case_i][case_j];
            
            // Si on n'est pas sur la case i j ou la valeur a ete trouvee
            if(valeur_case & val){
                g->possibilities[case_i][case_j] &= (~val);
                modified |= 1;
                
                // Si son poids passe a 1
                if(poids(g->possibilities[case_i][case_j])==1){
                    modified |= update_new_val(g, case_i, case_j);
                }
                    
                 // S'il n'y a plus aucune possibilite la grille est incorrecte
                else if(poids(g->possibilities[case_i][case_j])==0){
                    return 2;
                }
            } 
        }
    }
    
    return modified;
}



int unicite(grille * g, int* indices){
    /* Met a jour un bloc si une valeur n'est possible que dans une case en la lui affectant. */

    int modified = 0;
    
    int size = g->size;
    // On initialise des variables permettant de relever
    // la redondance de certaines possibilites ou non
    int seen = g->possibilities[indices[0]/size][indices[0]%size];
    int atleast_twice = 0;
        
    // On parcourt chaque case sauf la 1ere
    for(int case_no=1; case_no<size; case_no++){

        int i = indices[case_no]/size;
        int j = indices[case_no]%size;
        
        // En mettant a jour les possibilite vues et vues au moins 2 fois 
        atleast_twice |= (seen & g->possibilities[i][j]);
        seen |= g->possibilities[i][j];
    }
    int seen_once = atleast_twice ^= seen;
        
    // Si au moins une valeur n'est possible que dans une case
    if ( ~seen_once != 0){
    
        // On parcourt la liste de cases a nouveau
        // pour lui affecter cette valeur
        for(int case_no=0; case_no<size; case_no++){
            int i = indices[case_no]/size;
            int j = indices[case_no]%size;
            if(seen_once & g->possibilities[i][j]){
                if(poids(g->possibilities[i][j]) != 1){
                    modified = 1;
                    g->possibilities[i][j] = seen_once & g->possibilities[i][j];
                    modified |= update_new_val(g, i, j);
                }
            }
        }
    }
    
    return modified;
}



int parite(grille* g, int* indices){
    /* Met a jour un bloc si 2 cases contiennent exactement le meme couple
    de valeurs et retirant ces 2 possibilites des autres cases du bloc */
    
    int modified = 0;
    
    // On parcourt les cases a la recherche d'un couple possibilites
    int size = g->size;
    for(int case_nb=0; case_nb<size-1; case_nb++){
        int i = indices[case_nb]/size;
        int j = indices[case_nb]%size;
        
        // si on en trouve une on regarde si cette case se repete 2 fois
        int couple = g->possibilities[i][j];
        if(poids(couple) == 2){
            for(int case_nb2=case_nb+1; case_nb2<size; case_nb2++){
                int i2 = indices[case_nb2]/size;
                int j2 = indices[case_nb2]%size;
                
                if(couple == g->possibilities[i2][j2]){
                    
                    for(int case_nb3=0; case_nb3<size; case_nb3++){
                        int i3 = indices[case_nb3]/size;
                        int j3 = indices[case_nb3]%size;
                        int val_case = g->possibilities[i3][j3];
                        
                        
                        // On enleve le couple des possibilites des autres cases
                        if((i3 != i || j3 != j) && (i3 != i2 || j3 != j2)){
                            // Si possibilites en commun on modifie
                            if(couple & val_case){
                                g->possibilities[i3][j3] &= ~couple;
                                int new_weight = poids(val_case);
                                
                                // Verifions qu'il y a toujours des possibilites
                                if (new_weight == 0){
                                    return 2;
                                }
                                
                                if (new_weight == 1){
                                    modified |= update_new_val(g, i3, j3);
                                }
                                modified |= 1;
                            }
                        }
                
                    }
                }
            }
        }
    }
    
    return modified;

}



int update_new_val(grille* g, int i, int j){
    /* Met a jour les possibilites des cases impactees
    par la decouverte d'une valeur en case[i][j] 
    Et renvoie modified pour determiner si la matrice a
    ete modifiee ou non ou si elle est devenue incoherente */
    
    int modified = 0;
    int val = g->possibilities[i][j];
    
    // On met a jour sur les lignes
    int* indices = MemeLigne(g, i, j);
    modified |= exclusivite(g, indices, i, j, val);
    free(indices);
    
    // On met a jour sur les colonnes
    indices = MemeColonne(g, i, j);
    modified |= exclusivite(g, indices, i, j, val);
    free(indices);
    
    // On met a jour sur les regions
    indices = MemeRegion(g, i, j);
    modified |= exclusivite(g, indices, i, j, val);
    free(indices);
    
    return modified;
}



int bouclerRegles(grille* g){
    /* Appelle en boucle les regles EUP jusqu'a 
    resolution de la grille si possible */
    
    int modified = 1;
    int size = g->size;
    int reg_size = g->reg_size;
    
    while(modified == 1){
        modified = 0;
    
        // On parcourt toutes les lignes en appliquant unicite parite
        for(int i=0; i<size; i++){
            int* indices = MemeLigne(g, i, 0);
            modified |= unicite(g, indices);
            modified |= parite(g, indices);
            modified |= parite_complexe(g, indices, reg_size);
            free(indices);
        }
        
        // On parcourt toutes les colonnes en appliquant unicite parite
        for(int j=0; j<size; j++){
            int* indices = MemeColonne(g, 0, j);
            modified |= unicite(g, indices);
            modified |= parite(g, indices);
            free(indices);
        }
        
        // On parcourt toutes les regions en appliquant unicite parite
        for(int i=0; i<reg_size; i++){
            for(int j=0; j<reg_size; j++){
                int* indices = MemeRegion(g, i, j);
                modified |= unicite(g, indices);
                modified |= parite(g, indices);
                free(indices);
            }
        }
    }
    
    // Si la grille n'est pas incoherente on cherche
    // la case ou il y a le moins d'options possibles
    if(modified == 0){
        int best_opt = -1;
        int min = size;
        
        for(int i=0; i<size; i++){
            for(int j=0; j<size; j++){
                
                int p = poids(g->possibilities[i][j]);
                if (p > 1 && p < min){
                    best_opt = i*size + j;
                    min = p;
                }
            }
        }
        // Si la grille est resolue on renvoie -1
        //check_coherence(g)
        return best_opt;
    }
    
    // Sinon si la grille est invalide on renvoie 2
    else{
        return -2;
    }
}



int sudoku(grille* g){
    /* Resous une grille de sudoku */
    
    int pos_case;
    
    int size = g->size;
    
    pos_case = bouclerRegles(g);
    
    if (pos_case >= 0){
        int val_case = g->possibilities[pos_case/size][pos_case%size];
        
        while(val_case > 0){
            int tmp = val_case & (val_case -1);
            int poss = tmp ^ val_case;
            val_case = tmp;
            
            grille* g_copy = grille_copy(g);
            int i = pos_case/size;
            int j = pos_case%size;
            g->possibilities[i][j] = poss;
            
            int solved = 0;
            
            if(update_new_val(g, i, j) <= 1)
                solved = sudoku(g);
            
            if (solved){
                free_grille(g_copy);
                return 1;
            }
            
            else{
                // Keep trying possibilities
                int ** tmp_mat = g->possibilities;
                g->possibilities = g_copy->possibilities;
                g_copy->possibilities = tmp_mat;
                free_grille(g_copy);
            }          
        }
    }
    
    // Si la grille est resolue
    else if (pos_case == -1){
        return 1;
    }
    
    // Si la grille est ainsi insolvable
    // On retourne en arriere
    return 0;
}



int sudoku_solver(char * filename){
    /* Resous une grille de sudoku a charger d'un fichier 
    en l'affichant avant resolution et apres */
    
    grille* g = load_grille(filename);
    init_sudoku(g);
	Afficher(g);
	
	clock_t t1, t2;
	t1 = clock();
    int solved = sudoku(g);
    t2 = clock();
    printf("temps: %f\n", (float)(t2-t1)/CLOCKS_PER_SEC);
    Afficher(g);
    free_grille(g);
    return solved;
}





int nuplet(grille* g, int* indices, int nb){
    int modified = 0;
    
    // On parcourt les cases a la recherche de cases ayant nb possibilites
    int size = g->size;
    for(int case_nb=0; case_nb<size; case_nb++){
        int i = indices[case_nb]/size;
        int j = indices[case_nb]%size;
        
        // si on en trouve une on regarde si cette case se repete nb fois
        int cpt = nb;
        int nuplet = g->possibilities[i][j];
        if(poids(nuplet) == nb){
            for(int case_nb2=case_nb+1; case_nb2<size; case_nb2++){
                int i2 = indices[case_nb2]/size;
                int j2 = indices[case_nb2]%size;
                
                if(nuplet == g->possibilities[i2][j2]){
                    cpt--;
                    
                    // Si c'est le cas
                    if(cpt==0){
                        for(int case_nb3=0; case_nb3<size; case_nb3++){
                            int i3 = indices[case_nb3]/size;
                            int j3 = indices[case_nb3]%size;
                            int val_case = g->possibilities[i3][j3];
                            
                            // On enleve le nuplet des possibilites des autres cases
                            if((nuplet & val_case) && (nuplet != val_case)){
                            
                                g->possibilities[i3][j3] &= ~nuplet;
                                
                                int new_weight = poids(g->possibilities[i3][j3]);
                            
                                // Verifions qu'il y a toujours des possibilites
                                if (new_weight == 0){
                                    return 2;
                                }
                                
                                // Si on decouvre la valeur de la case on met a jour celles que ca impacte
                                if (new_weight == 1){
                                    modified |= update_new_val(g, i3, j3);
                                }
                                modified |= 1;
                            }
                        }                    
                    }
                }
            }
        }
    }
    
    return modified;
}



int parite_complexe(grille * g, int* indices, int length_max){ // length_max = reg_size ?

    /* Fonction permettant d'analyser un bloc en reperant
    les couples de valeurs n'aparaissant que dans 2 cases
    et supprimant les autres possibilites de ces cases */
    int modified = 0;
    int size = g->size;
    
    // Verifions la presence de memes couples
    // possibles parmi les cases parmi des nuplet ayant
    // au maximum length_max possibilites en commun
    for(int case_nb=0; case_nb<g->size; case_nb++){
        int i = indices[case_nb]/size;
        int j = indices[case_nb]%size;
        
        // Si la case a au moins 2 possibilites
        int weightij = poids(g->possibilities[i][j]);
        if (weightij >= 2){
            int caseij = g->possibilities[i][j];
            
            // On regarde si les autres cases du groupe ont
            // un couple de possibilites en commun
            for(int case_nb2=case_nb+1; case_nb2<size; case_nb2++){
                int i2 = indices[case_nb2]/size;
                int j2 = indices[case_nb2]%size;
                
                int caseij2 = g->possibilities[i2][j2];
                
                // Si les cases ont entre 2 et length_max possibilites en commun
                // Et qu'une des cases a au moins 3 possibilites
                int commun = caseij2 & caseij;
                int weightij2 = poids(caseij2);
                int weight_comm = poids(commun);
                if ((weight_comm >= 2 && weight_comm <= length_max) && (weightij > 2 || weightij2 >2)){
                    int commun_tmp = commun;
                    
                    // On extraie les couples
                    while(poids(commun) > 1){
                        
                        // On recupere le bit a 1 de poids le plus faible
                        commun_tmp = commun -1;
                        commun_tmp &= commun;
                        int poss1 = commun ^ commun_tmp;
                        commun = commun_tmp;
                        
                        int commun_tmp2 = commun;
                        while(commun_tmp2 <= 1UL){
                            commun_tmp = commun_tmp2 - 1;
                            commun_tmp &= commun_tmp2;
                            int poss2 = commun_tmp ^ commun_tmp2;
                            commun_tmp2 = commun_tmp;
                            
                            // Testons un couple qu'on suppose present 2 fois
                            int couple = poss1 | poss2;
                            int twice = 1;
                            
                            // Parcourons le groupe pour voir si les valeurs
                            // de ce couple sont presentes ailleurs
                            for(int case_nb3=0; case_nb3<size; case_nb3++){
                            //for (int case_j = 0; case_j<g->size; case_j++){
                                int i3 = indices[case_nb3]/size;
                                int j3 = indices[case_nb3]%size;
                            
                                // si ce n'est pas la case[i][j] ou [i2][j2]
                                if((i3 != i || j3 != j) && (i3 != i2 || j3 != j2)){
                                
                                    // et que des valeurs du couples sont dedans
                                    if((couple & g->possibilities[i3][j3]) != 0){
                                        // alors ce couple a d'autres options de placement
                                        // on passe donc a la suite
                                        twice = 0;
                                        break;
                                    }
                                }
                            } // Fin du test presence du couple 
                            
                            // Si le couple n'est bien present
                            // que 2 fois dans la ligne
                            if (twice){
                            
                                // On supprime les autres possibilites
                                // que ce couple des cases [i,j] et [i2,j2]
                                // si elles en avaient d'autres
                                // et on modifie donc la grille
                                modified |= 1;
                                
                                if(weightij > 2) {
                                    g->possibilities[i][j] ^= couple;
                                    
                                }
                                if(weightij2 >2){
                                    g->possibilities[i2][j2] ^= couple;
                                }
                                
                            } // Fin update grille si le couple n'a que 2 placements possibles
                        }
                    } // Fin extraction et test des couples
                }
            }
        }
    }
    return modified;
}


