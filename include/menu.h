#ifndef MENU_H_
#define MENU_H_

/**
 * \file menu.h
 * \brief Header Gestion Menu
 * \author Cody Theard
 * \version 1.0
 * \date 5 avril 2023
 *
 * Header Gestion du menu:
 * \n Signature des fonction externe de menu.c
 */

#include <stdlib.h>
#include <stdbool.h> 

#include <play.h>


/* ------------------------------------------------------ */
/* ------------------ Fonction Externe ------------------ */
/* ------------------------------------------------------ */

extern int menu(SDL_Window *window,SDL_Renderer *renderer);

extern int echap(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture * background_texture, SDL_Texture * tabTextGif[9]) ;

extern void changeResolution(int indiceResolution, int indiceFullscreen, SDL_Window *window);

extern void getWinInfo(SDL_Window *window, int * width, int * height, int tileSize, SDL_Rect * view, int * dstCoef, int * xBorder, int * yBorder);

#endif