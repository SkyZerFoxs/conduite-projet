#include <stdio.h>
#include <map.h>

/**
 * \file test_map.c
 * \brief Test des fonctionnalité des map
 * \author Yamis MANFALOTI
 * \version 2.0
 * \date 03 février 2023
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
 * \return Int qui caractérise la réussite de la fonction
 */
int main() {
    map_t * continent = initialiser_map("asset/map/mapTest.txt");
    
    printf("----------------------  Metadonnees  ----------------------\n");
    printf("map height : %d\n",continent->height);
    printf("map width  : %d\n",continent->width);
    printf("tileSize   : %d\n",continent->tileSize);

    printf("---------------------- Map Principal ----------------------\n");
    for (int i = 0; i < continent->height; i++) {
        for (int j = 0; j < continent->width; j++) {
            printf("%d ", continent->matriceMap[i][j]);
        }
        printf("\n");
    }
    printf("----------------------   Map Decor   ----------------------\n");
    for (int i = 0; i < continent->height; i++) {
        for (int j = 0; j < continent->width; j++) {
            printf("%d ", continent->matriceDecor[i][j]);
        }
        printf("\n");
    }
    
    detruire_map(continent);

    return 0;
}
