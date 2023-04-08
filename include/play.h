
#ifndef PLAY_H_
#define PLAY_H_


#include <time.h>

#include <affichage.h>
#include <hud.h>
#include <data.h>
#include <save.h>

/**
 * \file play.h
 * \brief Header Fonction Play
 * \author MANFALOTI Yamis
 * \version 1.5
 * \date 5 avril 2023
 *
 * Header Fonction Play:
 * \n Signature des fonction externe de play.c
 */


/* ------------------------------------------------------ */
/* ------------------ Fonction Externe ------------------ */
/* ------------------------------------------------------ */

// Fonction Pour Lancer Le Jeu
int play(SDL_Window *window, SDL_Renderer *renderer, int charger, SDL_Texture *tabTextGif[9]) ;

#endif