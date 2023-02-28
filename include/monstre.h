typedef struct monstre_s{
    char * nom;
    int niveau;
    int pv;
    int atk;
    int def;
}monstre_t;

monstre_t * creer_monstre(char * nom);
void afficher_monstre(monstre_t * monstre);
void supprimer_monstre(monstre_t ** monstre);
void evolution_monstre(monstre_t * monstre, int niv);