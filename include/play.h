
#ifndef PLAY_H_
#define PLAY_H_

#include <time.h>

#include <affichage.h>
#include <hud.h>
#include <data.h>
#include <save.h>

// Fonction Pour Lancer Le Jeu
int play(SDL_Window *window, SDL_Renderer *renderer, int charger, SDL_Texture *tabTextGif[9]) ;

#endif