#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void error_file()
{
	printf("Erreur : Fichier non  Trouvé\n");
	exit(1);
}



void error_format()
{
    printf("Erreur : Fichier mal formaté\n");
	exit(1);
}

