#include <stdio.h>
#include <stdlib.h>

#include <personnage.h>

/**
 *	\file personnage.c
 *  \brief fonction pour gérer les personnages
 *  \author Raccouard Nathan
 *  \version 2.0
 *  \date 27/02/2023
**/

/**
 * \fn personnage_t * creer_personnage(char * nom)
 * \brief création d'un personnage
 * 
 * \param nom du personnage
 * \return personnage_t perso
 * \return pointeur sur le personnage_t || NULL Fail ( statut fonction )
**/
extern
personnage_t * creer_personnage(char * nom){
    personnage_t * perso=malloc(sizeof(personnage_t));
    if ( perso == NULL ) {
        printf("Erreur : Echec malloc(perso) dans creer_personnage()");
        return NULL;
    }

    perso->nom=malloc(sizeof(char)*strlen(nom));
    if ( perso->nom == NULL ) {
        printf("Erreur : Echec malloc(perso->nom) dans creer_personnage()");
        return NULL;
    }
    strcpy(perso->nom,nom);

    perso->caract=malloc(sizeof(caract_t));
    if ( perso->caract == NULL ) {
        printf("Erreur : Echec malloc(perso->caract) dans creer_personnage()");
        return NULL;
    }

    perso->niveau=1;
    perso->pts_upgrade=0;

    perso->exp = 0;
    perso->palierExp = 0;

    perso->caract->pv = 100 ;
    perso->caract->maxPv = perso->caract->pv;
    perso->caract->atk = 10 ;
    perso->caract->def =  5 ;

    for (int i = 0; i < 6; i++) {
        perso->equipement[i] = malloc( sizeof(caract_t) );
        if ( perso->equipement[i] == NULL ) {
            printf("Erreur : Echec malloc(perso->equipement[%d]) dans creer_personnage()",i);
            return NULL;
        }
        perso->equipement[i]->pv = 0;
        perso->equipement[i]->maxPv = 0;
        perso->equipement[i]->atk = 0;
        perso->equipement[i]->def = 0;
    }

    return perso;
}

/**
 * \fn void afficher_perso(personnage_t * perso)
 * \brief afficher statistiques du personnage
 * 
 * \param personnage_t * perso
 * \return Aucun retour
**/
extern
void afficher_perso(personnage_t * perso){ 
    if ( perso == NULL ) {
        printf("Erreur : perso inexistant dans afficher_perso()");
        return;
    }

    printf("-------------------------------------------------------\n");

    printf("Nom : %s\n",perso->nom);
    printf("lvl : %d\n", perso->niveau);
    printf("Experience : %04d/%04d\n",perso->exp, perso->palierExp);
    printf("Points de vie : %04d/%04d\n", perso->caract->pv, perso->caract->maxPv);
    printf("Attaque : %d\n", perso->caract->atk);
    printf("Defense : %d\n", perso->caract->def);

    char * label[6] = {
        "Casque",
        "Pantalon",
        "Plastron",
        "Bottes",
        "Arme",
        "Accessoire"
    };

    for (int i = 0; i < 6; i++) {
        printf("-------- Stats %s --------\n",label[i]);
        printf("pv: %d, maxPv: %d, atk: %d, def: %d\n",
        perso->equipement[i]->pv,
        perso->equipement[i]->maxPv,
        perso->equipement[i]->atk,
        perso->equipement[i]->def );
    }

    printf("-------------------------------------------------------\n");
}

/**
 * \fn int calculer_stats_perso(personnage_t * perso, caract_t * caractSortie )
 * \brief Calcul des statistiques du personnage avec objets
 * 
 * \param perso stats personnage 
 * \param caractSortie stats personnage avec objets
 * \return 0 Success || 1 Fail ( statut fonction )
**/
extern int calculer_stats_perso(personnage_t * perso, caract_t * caractSortie ) {
    if ( perso == NULL ) {
        printf("Erreur : perso inexistant dans calculer_stats_perso()");
        return 1;
    }

    if ( caractSortie == NULL ) {
        printf("Erreur : caractSortie inexistant dans calculer_stats_perso()");
        return 1;
    }

    if ( caractSortie->pv != 0 && caractSortie->maxPv != 0 && caractSortie->atk != 0 && caractSortie->def != 0 ) {
        printf("Erreur : les stats de caractSortie ne sont pas a 0 dans calculer_stats_perso()\n");
        return 1;
    }

    caractSortie->maxPv = perso->caract->maxPv;
    caractSortie->atk = perso->caract->atk;
    caractSortie->def = perso->caract->def;

    for (int i = 0; i < 6; i++) {
        caractSortie->maxPv += perso->equipement[i]->maxPv;
        caractSortie->atk += perso->equipement[i]->atk;
        caractSortie->def += perso->equipement[i]->def;
    }

    if ( perso->caract->pv > caractSortie->maxPv ) {
        perso->caract->pv = caractSortie->maxPv;
    }

    return 0;

}

/**
 * \fn void supprimer_perso(personnage_t ** perso)
 * \brief Libère l'espace mémoire réservée par personnage_t
 * 
 * \param perso à supprimer
 * \return Aucun retour
**/
extern
void supprimer_perso(personnage_t ** perso){
    if ( perso == NULL || (*perso) == NULL ) {
        printf("Erreur : perso inexistant dans supprimer_perso()\n");
        return;
    }

    if ((*perso)->nom != NULL) {
        free((*perso)->nom);
        (*perso)->nom = NULL;
    }

    if ((*perso)->caract != NULL) {
        free((*perso)->caract);
        (*perso)->caract = NULL;
    }

    for (int i = 0; i < 6; i++) {
        if ( (*perso)->equipement[i] != NULL ) {
            free( (*perso)->equipement[i] );
            (*perso)->equipement[i] = NULL;
        }   
    }

    free(*perso);
    (*perso) = NULL;  
    
}


/**
 * \fn int upgrade_perso(personnage_t * perso, int stats)
 * \brief amélioration d'un personnage
 * \param personnage pointeur sur la structure du personnage qu'on améliore
 * \param stats indice de la statistique a améliorer
 * \return 0 Success || 1 Fail ( statut fonction )
**/
extern
int upgrade_perso(personnage_t * perso, int stats) { 
    
    switch ( stats ) {
        case 0:
            perso->caract->maxPv += UPGRADE_MAX_PV;
            break;
        case 1:
            perso->caract->atk += UPGRADE_ATK;
            break;
        case 2:
            perso->caract->def += UPGRADE_DEF;
            break;
        default:
            printf("Erreur : Mauvaise stats augmenté dans upgrade_perso()\n");
            return 1;
            break;
    }

    return 0;
}



