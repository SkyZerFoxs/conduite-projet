#include <stdio.h>

#include <objets.h>

/**
 *	\file objets.c
 *  \brief fonction pour gérer les objets
 *  \author Raccouard Nathan
 *  \version 1.0
 *  \date 10/02/2023
**/

extern 
liste_objet_t * load_liste_objets(char * nom_fich) {

    if ( nom_fich == NULL ) {
        printf("Erreur : Fichier liste item invalide dans load_objets()");
        return NULL;
    }

    FILE * filename = fopen(nom_fich, "r");
    if (!filename) {
        printf("Erreur : Echec ouverture fichier(%s) dans load_objets()\n", nom_fich);
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
    liste_objet_t* liste = malloc(sizeof(liste_objet_t));
    if (liste == NULL) {
        printf("Erreur : Echec malloc(liste) dans load_objets()\n");
        return NULL;
    }

    liste->nbElem = nb_lignes;
    liste->tab = malloc(sizeof(objet_t*) * nb_lignes);
    if (liste->tab == NULL) {
        printf("Erreur : Echec malloc(liste->tab) dans load_objets()\n");
        return NULL;
    }
    
    int result;
    for (int i = 0; i < liste->nbElem; i++) {
        // Allocation mémoire
        liste->tab[i] = malloc(sizeof(objet_t));
        if (liste->tab[i] == NULL) {
            printf("Erreur : Echec malloc(tab[%d]) dans load_objets()\n",i);
            return NULL;
        }
        liste->tab[i]->nom = malloc(sizeof(char) * 25);
        if (liste->tab[i]->nom == NULL) {
            printf("Erreur : Echec malloc(tab[%d]->nom) dans load_objets()\n",i);
            return NULL;
        }
        liste->tab[i]->stats = malloc(sizeof(caract_t));
        if (liste->tab[i]->stats == NULL) {
            printf("Erreur : Echec malloc(tab[%d]->stats) dans load_objets()\n",i);
            return NULL;
        }
        // Récupération données
        result = fscanf(filename, "%[^:]:%d:%d:%d:%d:%d:%d:%d:\n", 
            liste->tab[i]->nom, &(liste->tab[i]->niv), &(liste->tab[i]->typeID), &(liste->tab[i]->stats->pv),
            &(liste->tab[i]->stats->atk), &(liste->tab[i]->stats->def), &(liste->tab[i]->prix_achat), &(liste->tab[i]->prix_vente));
        if (result != 8) {
            printf("Erreur : Format invalide dans la ligne %d du fichier %s dans load_objets()\n", i+1, nom_fich);
            return NULL;
        }
        liste->tab[i]->id = i;
        liste->tab[i]->nb=0;
    }

    fclose(filename);

    return liste;
}


extern
int afficher_liste_objet(liste_objet_t * liste) { 
    if ( liste == NULL ) {
        printf("Erreur : La liste d'objet en paramètre est invalide dans afficher_objet()\n");
        return 1;
    }

    if ( liste->tab == NULL || liste->nbElem <= 0 ) {
        printf("Erreur : La liste d'objet est inexistante dans afficher_objet()\n");
        return 1;
    }

    printf("-------------------------------------------------------\n");
    printf("nb objets: %d\n",liste->nbElem);
    printf("-------------------------------------------------------\n");
    for (int i=0; i<liste->nbElem;i++){
        if ( liste->tab[i]->nb > 0 ) {
            printf("Id : %d\n", liste->tab[i]->id);
            printf("Nom : %s\n",liste->tab[i]->nom);
            printf("Type : %d\n", liste->tab[i]->typeID);
            printf("Niveau : %d\n", liste->tab[i]->niv);
            printf("PV : %d\n", liste->tab[i]->stats->pv);
            printf("Defense : %d\n", liste->tab[i]->stats->def);
            printf("Attaque : %d\n", liste->tab[i]->stats->atk);
            printf("Prix achat: %d\n", liste->tab[i]->prix_achat);
            printf("Prix vente : %d\n", liste->tab[i]->prix_vente);
            printf("Nombre : %d\n", liste->tab[i]->nb);
            printf("\n");
        }
    }
    printf("-------------------------------------------------------\n");

    return 0;
}

extern
void detruire_objet(objet_t ** objet){
    if ( objet == NULL || (*objet) == NULL ) {
        printf("Erreur : Objet en paramètre invalide dans detruire_objet()\n");
    }

    if ( (*objet)->nom != NULL ) {
        free((*objet)->nom);
        (*objet)->nom = NULL;
    }

    if ( (*objet)->stats != NULL ) {
        free((*objet)->stats);
        (*objet)->stats = NULL;
    }

    free(*objet);
    (*objet) = NULL;
}

extern
void detruire_liste_objet( liste_objet_t ** liste) {

    if ( liste == NULL || (*liste) == NULL ) {
        printf("Erreur : La liste d'objet en paramètre est invalide dans detruire_liste_objet()\n");
        return;
    }

    if ( (*liste)->tab == NULL || (*liste)->nbElem < 0 ) {
        printf("Erreur : La liste d'objet est inexistante dans detruire_liste_objet()\n");
        return;
    }

    for (int i = 0; i < (*liste)->nbElem; i++) {
        if ( (*liste)->tab[i] != NULL ) {
            detruire_objet( &( (*liste)->tab[i] ) );
        }
    }

    free((*liste)->tab);
    (*liste)->tab = NULL;

    (*liste)->nbElem = -1;

    free((*liste));
    (*liste) = NULL;
}

extern int loot_monstre(int niv_monstre) {
    int drop = rand() % 3; 
    if( drop==1 ) {
        int item_drop = rand() % 8;
        int niv_obj= rand() % 100;
        if(niv_monstre>=1 && niv_monstre<=10){
            if(niv_obj>=0 && niv_obj<=4){
                return item_drop*3+2;
            }
            else if(item_drop>=5 && item_drop<=15){
                return item_drop*3+1;
            }
            else{
                return item_drop*3;
            }
        }
        else if(niv_monstre>=11 && niv_monstre<=20){
            if(niv_obj>=0 && niv_obj<=9){
                return item_drop*3+2;
            }
            else if(item_drop>=10 && item_drop<=70){
                return item_drop*3+1;
            }
            else{
                return item_drop*3;
            }
        }
        else if(niv_monstre>=21 && niv_monstre<=25){
            if(niv_obj>=0 && niv_obj<=49){
                return item_drop*3+2;
            }
            else if(item_drop>=50 && item_drop<=80){
                return item_drop*3+1;
            }
            else{
                return item_drop*3;
            }
        }
    }

    return -1;
}

extern int loot_coffre(int niv_coffre) {
    int item_drop = rand() % 8;
    return item_drop*3+niv_coffre;

}