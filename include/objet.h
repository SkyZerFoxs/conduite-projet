#ifndef CARACT_H
#define CARACT_H

#include <caract.h>

#define MAX_CHAR 50

typedef struct objet_s{
    int id;
    caract_t * stats;
    int prix_achat;
    int prix_vente;

}objet_t;

objet_t * creer_objet(int id,int pv, int atk,int def,int puissance,int mana,int force, int prix_achat,int prix_vente);
void afficher_objet(objet_t * objet);
void supprimer_objet(objet_t ** objet);

#endif