/* dans le fichier texte :
id_quete \n nom \n nom pnj \n description \n id_objet*/

typedef struct quete_s{ 
    int id_quete;           //id quete unique refere a quete.txt
    int etat;               //etat de la quete : 0 pour en cours et 1 pour finis
}quete_t;

int existe_quete(quete_t * quete);
quete_t * creer_quete(int id_quete);
void supprimer_quete(quete_t * quete);
void valider_quete(quete_t ** quete);
void afficher_quete(quete_t * quete);