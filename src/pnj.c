#include <stdio.h>
#include <stdlib.h>

#include <pnj.h>


extern 
liste_type_pnj_t * Load_Liste_Type_Pnj(char * nom_fich) {

    if ( nom_fich == NULL ) {
        printf("Erreur : Fichier liste item invalide dans Load_Liste_Type_Pnj()");
        return NULL;
    }

    FILE * filename = fopen(nom_fich, "r");
    if (!filename) {
        printf("Erreur : Echec ouverture fichier(%s) dans Load_Liste_Type_Pnj()\n", nom_fich);
        return NULL;
    }
	
    // Compter le nombre de lignes dans le fichier
    int nb_lignes = 0;
    int prec = '\n'; // caractère précédent initialisé à un retour à la ligne
    int dernier_caractere = -1; // initialiser à une valeur impossible
    while (!feof(filename)) {
        int c = fgetc(filename);
        if (c == '\n' && prec != '\n') {
            nb_lignes++;
        }
        prec = c;
        dernier_caractere = c;
    }
    if (dernier_caractere != '\n' && prec != '\n') {
        nb_lignes++;
    }
    
    rewind(filename);

    
    // Allouer dynamiquement la structure liste_objet_t et le tableau
    liste_type_pnj_t* liste = malloc(sizeof(liste_type_pnj_t));
    if (liste == NULL) {
        printf("Erreur : Echec malloc(liste) dans Load_Liste_Type_Pnj()\n");
        return NULL;
    }

    liste->nbElem = nb_lignes;
    liste->liste = malloc(sizeof(type_pnj_t*) * nb_lignes);
    if (liste->liste == NULL) {
        printf("Erreur : Echec malloc(liste->liste) dans Load_Liste_Type_Pnj()\n");
        return NULL;
    }
    
    int result;
    for (int i = 0; i < liste->nbElem; i++) {
        // Allocation mémoire
        liste->liste[i] = malloc(sizeof(type_pnj_t));
        if (liste->liste[i] == NULL) {
            printf("Erreur : Echec malloc(liste[%d]) dans Load_Liste_Type_Pnj()\n",i);
            return NULL;
        }
        liste->liste[i]->nom = malloc(sizeof(char) * 25);
        if (liste->liste[i]->nom == NULL) {
            printf("Erreur : Echec malloc(liste[%d]->nom) dans Load_Liste_Type_Pnj()\n",i);
            return NULL;
        }
        liste->liste[i]->imageDialogue = malloc(sizeof(char) * 50);
        if (liste->liste[i]->imageDialogue == NULL) {
            printf("Erreur : Echec malloc(liste[%d]->stats) dans Load_Liste_Type_Pnj()\n",i);
            return NULL;
        }
        liste->liste[i]->dialogue = malloc(sizeof(char) * 250);
        if (liste->liste[i]->imageDialogue == NULL) {
            printf("Erreur : Echec malloc(liste[%d]->stats) dans Load_Liste_Type_Pnj()\n",i);
            return NULL;
        }

        // Récupération données
        result = fscanf(filename, "%[^:]:%[^:]:%[^:]:\n", liste->liste[i]->nom, liste->liste[i]->imageDialogue, liste->liste[i]->dialogue);
        if (result != 3) {
            printf("Erreur : Format invalide dans la ligne %d du fichier %s dans Load_Liste_Type_Pnj()\n", i+1, nom_fich);
            return NULL;
        }
    }
    
    fclose(filename);

    return liste;
}



extern void Detruire_Liste_Type_Pnj( liste_type_pnj_t ** liste_type  ) {
    if ( liste_type == NULL || (*liste_type) == NULL ) {
        printf("Erreur : liste_type_pnj en parametre invalide dans Detruire_Liste_Type_Pnj()\n");
        return; 
    }

    if ( (*liste_type)->nbElem <= 0 ) {
        printf("Erreur : Le nombre d'element de la liste est <= 0 dans Detruire_Liste_Type_Pnj()\n");
        return;
    }

    if ( (*liste_type)->liste == NULL ) {
        printf("Erreur : La liste est vide dans Detruire_Liste_Type_Pnj()\n");
        return;
    }

    for (int i = 0; i < (*liste_type)->nbElem; i++) {
        if ( (*liste_type)->liste[i] != NULL ) {
            if ( (*liste_type)->liste[i]->nom != NULL ) {
                free ( (*liste_type)->liste[i]->nom );
                (*liste_type)->liste[i]->nom = NULL;
            }
            if ( (*liste_type)->liste[i]->imageDialogue != NULL ) {
                free ( (*liste_type)->liste[i]->imageDialogue );
                (*liste_type)->liste[i]->imageDialogue = NULL;
            }
            if ( (*liste_type)->liste[i]->dialogue != NULL ) {
                free ( (*liste_type)->liste[i]->dialogue );
                (*liste_type)->liste[i]->dialogue = NULL;
            }
        }
    }
    
    free((*liste_type)->liste);
    (*liste_type)->liste = NULL;

    free((*liste_type));
    (*liste_type) = NULL;
}


extern int Afficher_Liste_Type_Pnj(liste_type_pnj_t * liste_type) {
    if ( liste_type == NULL ) {
        printf("Erreur : liste_type en parametre invalide dans Afficher_Liste_Type_Pnj()\n");
        return 1;
    }

    printf("-------------------------------------------------\n");

    for (int i = 0; i < liste_type->nbElem; i++) {
        printf("%s\n",liste_type->liste[i]->nom);
        printf("%s\n",liste_type->liste[i]->imageDialogue);
        printf("%s\n\n",liste_type->liste[i]->dialogue);
    }

    printf("-------------------------------------------------\n");

    return 0;
}

extern pnj_t * creer_pnj(int ID, int y, int x, liste_type_pnj_t * liste_type) {

    if ( liste_type == NULL ) {
        printf("Erreur : liste_type_pnj en parametre invalide dans creer_pnj()\n");
        return NULL; 
    }

    if ( ID < 0 || ID >= liste_type->nbElem ) {
        printf("Erreur : ID invalide dans creer_pnj()\n");
        return NULL; 
    }

    if ( y < 0 || x < 0 ) {
        printf("Erreur : coordonees invalide dans creer_pnj()\n");
        return NULL; 
    }

    pnj_t * pnj = malloc( sizeof(pnj_t) );
    if ( pnj == NULL ) {
        printf("Erreur : Echec malloc(pnj) dans creer_pnj()\n");
        return NULL; 
    }

    pnj->pnjTypeID = ID;
    pnj->pos_y = y;
    pnj->pos_x = x;

    return pnj;
}

extern void supprimer_pnj(pnj_t ** pnj) {
    if ( pnj == NULL || (*pnj) == NULL ) {
        printf("Erreur : pnj en parametre invalide dans supprimer_pnj()\n");
        return; 
    }

    (*pnj)->pnjTypeID = -1;
    (*pnj)->pos_y = -1;
    (*pnj)->pos_x = -1;

    free( (*pnj) );
    (*pnj) = NULL;

}

extern void afficher_pnj(pnj_t * pnj) {
    if ( pnj == NULL  ) {
        printf("Erreur : pnj en parametre invalide dans afficher_pnj()\n");
        return; 
    }

    printf("-------------------------------------------\n");
    printf("TypeID: %d\n",pnj->pnjTypeID);
    printf("posY: %d\n",pnj->pos_y);
    printf("posX: %d\n",pnj->pos_x);
    printf("-------------------------------------------\n");
}