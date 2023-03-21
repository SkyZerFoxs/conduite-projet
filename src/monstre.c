#include <stdio.h>
#include <stdlib.h>

#include <monstre.h>

/**
 *	\file monstre.c
 *  \brief Fonction pour gérer les monstres
 *  \author Raccouard Nathan
 *  \version 1.0
 *  \date 9/02/2023
**/

extern
void afficher_monstre(monstre_t * monstre){ 
    if ( monstre == NULL ) {
        printf("Erreur : monstre inexistant dans afficher_monstre()");
        return;
    }

    printf("nom: %s\n", monstre->nom);
    printf("lvl: %d\n", monstre->niveau);
    printf("x: %d   y: %d\n",monstre->pos_x,monstre->pos_y);
    /*
    printf("PV: %d\n", monstre->pv);
    printf("Attaque : %d\n", monstre->atk);
    printf("Defense : %d\n", monstre->def);
    */
    printf("Points de vie : %d\n", monstre->caract->pv);
    printf("Attaque : %d\n", monstre->caract->atk);
    printf("Defense : %d\n", monstre->caract->def);
    /*
    printf("Mana : %d\n", monstre->caract->mana);
    printf("Puissance : %d\n", monstre->caract->puissance);
    */

}

extern
monstre_t * creer_monstre(char * nom, int niv, int y, int x) {
    if ( nom == NULL ) {
        printf("Erreur : Nom inexistant dans creer_monstre()");
        return NULL;
    }

    if ( niv < 0 ) {
        printf("Erreur : Niveau incorrecte dans creer_monstre()");
        return NULL;
    }

    monstre_t * monstre=malloc(sizeof(monstre_t));
    if ( monstre == NULL ) {
        printf("Erreur : Echec malloc(monstre) dans creer_monstre()");
        return NULL;
    }

    monstre->nom=malloc(sizeof(char) * strlen(nom));
    if ( monstre->nom == NULL ) {
        printf("Erreur : Echec malloc(monstre->nom) dans creer_monstre()");
        return NULL;
    }
    strcpy(monstre->nom,nom);

    monstre->caract=malloc(sizeof(caract_t));
    if ( monstre->caract == NULL ) {
        printf("Erreur : Echec malloc( monstre->caract) dans creer_monstre()");
        return NULL;
    }

    monstre->pos_x=x;
    monstre->pos_y=y;

    monstre->niveau = niv;

    monstre->caract->pv = 50 + ( niv - 1 ) * 10;
    monstre->caract->atk = 5 + ( niv - 1 ) * 5;
    monstre->caract->def = 2 + ( niv - 1 ) * 2;
    monstre->caract->mana = 0;

    return monstre;
}

void supprimer_monstre(monstre_t ** monstre) {
    if ( monstre == NULL || (*monstre) == NULL ) {
        printf("Erreur : monstre inexistant dans supprimer_monstre()");
        return;
    }

    free((*monstre)->nom);
    (*monstre)->nom=NULL;

    free((*monstre)->caract);
    (*monstre)->caract=NULL;

    free(*monstre);
    (*monstre)=NULL;
}