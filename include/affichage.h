#ifndef _AFFICHAGE_H_
#define _AFFICHAGE_H_

/**
 * \file affichage.h
 * \brief Header Gestion Affichage
 * \author Yamis MANFALOTI
 * \version 1.0
 * \date 11 février 2023
 *
 * Header Gestion de l'affichage:
 * \n Signature des fonction externe de affichage.c
 */

#include <map.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

extern void Init_SDL(SDL_Window **window, SDL_Renderer **renderer, int width, int height);
extern void Quit_SDL(SDL_Window *window, SDL_Renderer *renderer);
extern void Afficher_IMG(char * IMG, SDL_Renderer *renderer, SDL_Texture **texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect );
extern void Detruire_Texture(SDL_Texture *texture);
extern int Afficher_Tile(char * tileSet, int tileSize, float dstCoef, int xBorder, int tileNumber, int colonne, int ligne, SDL_Renderer *renderer, SDL_Texture **texture);
extern void getWinInfo(SDL_Window *window, map_t * map, int * width, int * height, float * dstCoef, int * xBorder );
extern void Afficher_Map(char * tileSet, map_t * map, SDL_Window *window, SDL_Renderer *renderer);

#endif