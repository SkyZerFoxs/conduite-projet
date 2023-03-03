//Taille de la Matrice
#define M 30
#define N 20

//Valeur des directions pour deplacement
#define H 8
#define D 6
#define B 5
#define G 4

extern void init_matrice (int collision[N][M]);
extern void affiche_matrice (int collision[N][M], int x, int y);
extern int deplacement_perso (int collision[N][M], int * x, int * y, int dir);
