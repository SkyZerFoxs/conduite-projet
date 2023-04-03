#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char * argv[] ) {

    int mat[10][10] = {
        { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
        { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
        { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
        { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
        { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
        {10,10,10,10,10,10,10,10,10,10},
        {10,10,10,10,10,10,10,10,10,10},
        {10,10,10,10,10,10,10,10,10,10},
        {10,10,10,10,10,10,10,10,10,10},
        {10,10,10,10,10,10,10,10,10,10}
    };

    int monstre[10][10] = {
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
    };

    srand(time(NULL));

    int zoneLevel, randomLevel;
    for ( int y = 0; y < 10; y++ ) {
        for (int x = 0; x < 10; x++ ) {
            if ( monstre[y][x] != -1 && 1 ) {
                zoneLevel = mat[y][x];
                if ( zoneLevel > 0 && zoneLevel % 5 == 0 ) {
                    randomLevel = zoneLevel - ( rand() % 5 ) ;
                    printf("Monstre crée au lvl %d en y=%d et x=%d\n",randomLevel,y,x);
                }
                else {
                    printf("Erreur : Level de zone incorecte dans mat[%d][%d] dans fonction()\n",y,x);
                    return 1;
                }
            }
        }
        printf("\n");
    }

    return 05;
}