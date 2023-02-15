#include <caract.h>

typedef struct objet_s{
    char * nom;
    int id;
    caract_t * stats;
    int prix_achat;
    int prix_vente;

}objet_t;

objet_t * creer_objet(char * nom, int id,int pv, int atk,int def,int puissance,int mana,int force, int prix_achat,int prix_vente);


