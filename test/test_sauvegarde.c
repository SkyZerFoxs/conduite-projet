#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <personnage.h>
#include <sauvegarde.h>

int main(){
    personnage_t * perso=creer_personnage("jean");
    save_game(1,2,perso);
}