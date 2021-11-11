Projet solveur de sudoku réalisé dans le cadre du module I231 Outils algotithmiques
du Master 1 Développement et Ingénierie des Données de l'université de Toulon en 2021.
par Léa Glandus.


Le programme se lance à l'aide de la commande :
                ./sudoku
                ou ./sudoku < filename >
Par défaut la grille utilisée est celle du sujet grille_sujet.txt mais une nouvelle grille peut être fournie.

Pour donner une nouvelle grille il faut la placer dans le dossier du code et suivre une structure :
    Le fichier doit contenir la grille représentée par les nombres de chaque case
    séparés par un ou plusieurs espaces et avec un saut de ligne à chaque nouvelle ligne.
    De haut en bas et de gauche à droite.

    Les grilles valides solvables par ce programme doivent avoir une taille allant de 4 à 5
    car les possibilites de chaque case sont représentées par un int soit par 32 bits.
    (9 étant la taille classique d'un sudoku).


Le programme affiche le nom du fichier de la grille, la grille originale puis la grille avec son temps de resolution.


Informations sur le code:
	Le projet se compose de 7 fichiers sources:

		- common.h, common.c:
		    definition de popcount en poids
		    fonctions de gestion d'erreur

		- grille.h, grille.c : 
			définition d'une grille
			fonctions d'allocation memoire, de chargement, copie, libération mémoire, affichage d'une grille
			fonctions MemeLigne/MemeColonne/MemeRegion
			
		- sudoku.c/sudoku.h:
		    fonction d'initialisation du sudoku
			fonctions exclusivité, unicité, parité
			fonctions de resolution du sudoku appliquant les regles EUP precedentes en boucle et en appliquant du backtracking
			fonctions parite_complexe et nuplet implémentées au début du sujet mais non utilisées decoulant d'idees de regles applicables efficacement à la main mais dont le temps de calcul a semblé moins efficace que l'appel au backtracking et qui ne sont pas demandées dans le sujet.
	    
	    - main.c :
	        fonction d'ecécution du programme main


Ce programme a été testé avec valgrind et la commande make permet de le vérifier tout en lançant le programme.

	
Quelques exemples de fichiers de grilles sont joint à ce projet.


Pour plus d'informations consulter le sujet disponible sur ce lien :
	    http://zanotti.univ-tln.fr/ALGO/I51/Sudoku.html
