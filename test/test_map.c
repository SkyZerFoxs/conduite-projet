#include <stdio.h>
#include <map.h>

int main() {
    map_t * continent = initialiser_map("asset/map/mapTest.txt");

    for (int i = 0; i < continent->height; i++) {
        for (int j = 0; j < continent->width; j++) {
            printf("%d ", continent->matriceMap[i][j]);
        }
        printf("\n");
    }
    
    detruire_map(continent);

}
