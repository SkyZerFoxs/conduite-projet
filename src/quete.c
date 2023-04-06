#include <stdio.h>
#include <stdlib.h>

#include <quete.h>

extern quete_t * Load_Quete(const char * nom_fichier) {
    // initialisation des variables
    FILE *fichier;
    char ligne[256];
    int nb_elements = 0;

    // Ouverture du fichier en mode lecture
    fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        printf("Erreur : Echec ouverture fichier dans Load_Quete()\n");
        return NULL;
    }

    // Comptage du nombre de lignes du fichier
    while (fgets(ligne, sizeof(ligne), fichier)) {
        nb_elements++;
    }

    // Retour au début du fichier
    rewind(fichier);

    // Remplissage de la liste de sprites
    int result;
    int i = 0;
    quete_t *first_quete = NULL;
    quete_t *previous_quete = NULL;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        // Allocation de la quete
        quete_t * quete = malloc( sizeof(quete_t) );
        if (quete == NULL) {
            printf("Erreur : Echec malloc(quete) dans Load_Quete() \n");
            fclose(fichier);
            return NULL;
        }

        // Ajout id quete
        quete->id = i;

        // Récupération données
        result = sscanf(ligne, "%[^:]:%[^:]:\n", quete->objectifQuete,quete->dialogueQuetePnj);
        if (result != 2) {
            printf("Erreur : Format invalide dans la ligne %d du fichier %s dans Load_Quete()\n", i+1, nom_fichier);
            return NULL;
        }

        // Ajout des pointeurs avant et après
        quete->avant = previous_quete;
        quete->apres = NULL;

        // Mise à jour des pointeurs pour l'élément précédent
        if (previous_quete != NULL) {
            previous_quete->apres = quete;
        } else {
            // Si c'est le premier élément, on le garde en mémoire
            first_quete = quete;
        }

        // Mise à jour de l'élément précédent
        previous_quete = quete;

        i++;
    }

    // Fermeture du fichier
    fclose(fichier);

    return first_quete;
}

extern void afficher_quetes(quete_t * premiere_quete) {
    quete_t * quete_courante = premiere_quete;
    while (quete_courante != NULL) {
        printf("ID : %d\n", quete_courante->id);
        printf("Objectif : %s\n", quete_courante->objectifQuete);
        printf("Dialogue du PNJ : %s\n", quete_courante->dialogueQuetePnj);
        quete_courante = quete_courante->apres;
    }
}
