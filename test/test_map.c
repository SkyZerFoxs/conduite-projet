#include <stdio.h>
#include <map.h>

/**
 * \file test_map.c
 * \brief Test des fonctionnalité des map
 * \author Yamis MANFALOTI
 * \version 2.0
 * \date 14 février 2023
 *
 * Test initialiser_map: 
 * \n Création structure map_t
 * \n Chargement des map par le fichier sélectionné
 * \n Affichage des informations stockées dans la structure map_t
 */

/**
 * \brief Fonction principale du test
 * 
 * \param void Aucun paramètre en entrée 
 * \return Int qui caractérise la du main
 */
int main() {
    map_t * continent = Initialiser_Map("asset/map/map.txt");
    
    if ( continent != NULL) {
        printf("----------------------  Metadonnees  ----------------------\n");
        printf("map height : %d\n",continent->height);
        printf("map width  : %d\n",continent->width);
        printf("tileSize   : %d\n",continent->tileSize);
        printf("layers     : %d\n",continent->layer);

        for (int n = 0; n < continent->layer; n++) {
            printf("---------------------- Map layer %d ----------------------\n",n);
            for (int i = 0; i < continent->height; i++) {
                for (int j = 0; j < continent->width; j++) {
                    printf("%d,",continent->matrice[n][i][j]);
                    
                }
                printf("\n");
            }
            printf("\n");
        }
        
        Detruire_Map(&continent);
    }

    return 0;
}
