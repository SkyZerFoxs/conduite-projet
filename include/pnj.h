/* dans le fichier texte :
id_pnj \n nom \n role */

#define MAX_CHAR 50

typedef struct pnj_s{
    int id_pnj;
}pnj_t;

int existe_pnj(pnj_t * pnj);
pnj_t * creer_pnj(int id_pnj);
void supprimer_pnj(pnj_t ** pnj);
void afficher_pnj(pnj_t * pnj);
