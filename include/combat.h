#ifndef COMBAT_H
#define COMBAT_H

#define M 10
#define N 10

#define H 8
#define D 6
#define B 5
#define G 4

#include <personnage.h>
#include <monstre.h>

extern void combat_monstre(monstre_t * monstre, personnage_t * perso);
extern void combat_joueur(personnage_t * perso, monstre_t * monstre, char atk);

#endif