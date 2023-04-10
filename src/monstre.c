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

/**
 * \fn int afficher_monstre(monstre_t * monstre)
 * \brief Affiche les statistiques d'un monstre
 * \param monstre pointeur sur monstre
 * \return 0 Success || 1 Fail ( statut fonction )
**/
extern
int afficher_monstre(monstre_t * monstre){ 
    if ( monstre == NULL ) {
        printf("Erreur : monstre inexistant dans afficher_monstre()");
        return 1;
    }

    printf("-------------------------------------------------------\n");

    printf("nom: %s\n", monstre->nom);
    printf("lvl: %d\n", monstre->niveau);
    printf("x: %d   y: %d\n",monstre->pos_x,monstre->pos_y);
    printf("Points de vie : %d\n", monstre->caract->pv);
    printf("Attaque : %d\n", monstre->caract->atk);
    printf("Defense : %d\n", monstre->caract->def);

    printf("-------------------------------------------------------\n");

    return 0;
}

/**
 * \fn monstre_t * creer_monstre(char * nom, int niv, int y, int x)
 * \brief Créer un monstre
 * 
 * \param nom nom du monstre
 * \param niv niveau du monstre
 * \param y position verticale du monstre
 * \param x position horizontale du monstre
 * \return pointeur sur monstre_t || NULL Fail ( statut fonction )
**/
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
    monstre->monstreSize = 1;

    monstre->niveau = niv;

    
    if ( niv >=  30) {
        monstre->caract->pv = 1000;
    }
    else {
        monstre->caract->pv =  50 + (niv * 5) ;
    }
    monstre->caract->maxPv = monstre->caract->pv;
    monstre->caract->atk = 5 + (niv * 2) ;
    monstre->caract->def = 5 + (niv * 2) ;

    return monstre;
}

/**
 * \fn void supprimer_monstre(monstre_t ** monstre)
 * \brief Libère l'espace mémoire reservée par monstre
 * 
 * \param monstre pointeur de pointeur sur le monstre_t à supprimer
 * \return Aucun retour
**/
void supprimer_monstre(monstre_t ** monstre) {
    if ( monstre == NULL || (*monstre) == NULL ) {
        printf("Erreur : monstre inexistant dans supprimer_monstre()");
        return;
    }

    if ( (*monstre)->nom != NULL ) {
        free((*monstre)->nom);
        (*monstre)->nom=NULL;
    }

    if ( (*monstre)->caract != NULL ) {
        free((*monstre)->caract);
        (*monstre)->caract=NULL;
    }
    
    free(*monstre);
    (*monstre)=NULL;
    
}