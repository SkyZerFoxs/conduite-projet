/* dans le fichier texte :
id_quete \n nom_quete \n nom_pnj \n description \n nom_objet*/

//Maximum de characteres lus dans une ligne dans les txt
#define MAX_CHAR 50 

typedef struct quete_s{ 
    int id_quete;           //id quete unique refere a quete.txt
    int etat;               //etat de la quete : 0 pour en cours et 1 pour finis
}quete_t;

int existe_quete(quete_t * quete);
quete_t * creer_quete(int id_quete);
void supprimer_quete(quete_t ** quete);
void valider_quete(quete_t ** quete);
void afficher_quete(quete_t * quete);