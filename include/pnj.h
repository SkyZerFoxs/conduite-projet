/* dans le fichier texte :
id_pnj \n nom \n role */

#define MAX_CHAR 50

typedef struct pnj_s{
    int id_pnj;
}pnj_t;

extern int existe_pnj(pnj_t * pnj);
extern pnj_t * creer_pnj(int id_pnj);
extern void supprimer_pnj(pnj_t ** pnj);
extern void afficher_pnj(pnj_t * pnj);
