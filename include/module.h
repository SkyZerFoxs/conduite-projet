#define VRAI 1
#define FAUX 0

typedef struct pnj_s{
    char * nom;
    int num_role;
    int nb_quete;
}pnj_t;

typedef struct quete_s{
    int etat;               //etat de la quete : FAUX pour en cours et VRAI pour finis
    pnj_t * pnj;            //pnj qui donne la quete
    char * description;     //court texte descriptif de la quete
    char * recompense;      //modifier apres creation du loot + inventaire ...
}quete_t;



int existe_pnj(pnj_t * pnj);
pnj_t * creer_pnj(char * nom, int num_role);
void supprimer_pnj(pnj_t ** pnj);
void afficher_pnj(pnj_t * pnj);

int existe_quete(quete_t * quete);
quete_t * creer_quete(pnj_t * pnj, char * description, char * recompense);
void supprimer_quete(quete_t ** quete);
quete_t * valider_quete(quete_t * quete);
void afficher_quete(quete_t * quete);