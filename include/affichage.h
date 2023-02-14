#ifndef _AFFICHAGE_H_
#define _AFFICHAGE_H_

/**
 * \file affichage.h
 * \brief Header Gestion Affichage
 * \author Yamis MANFALOTI
 * \version 2.0
 * \date 14 février 2023
 *
 * Header Gestion de l'affichage:
 * \n Signature des fonction externe de affichage.c
 */

#include <map.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

extern int Init_SDL(SDL_Window **window, SDL_Renderer **renderer, int width, int height);
extern void Quit_SDL(SDL_Window *window, SDL_Renderer *renderer);
extern void Afficher_IMG(char * IMG, SDL_Renderer *renderer, SDL_Texture **texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect );
extern void Detruire_Texture(SDL_Texture *texture);
extern int Afficher_Tile(char * tileSet, int tileSize, int dstCoef, int xBorder, int yBorder, int tileNumber, int colonne, int ligne, SDL_Rect * view, SDL_Renderer *renderer, SDL_Texture **texture);
extern void getWinInfo(SDL_Window *window, map_t * map, SDL_Rect * view, int * width, int * height, int * dstCoef, int * xBorder, int * yBorder);
extern void Afficher_Map(char * tileSet, map_t * map, SDL_Window *window, SDL_Renderer *renderer, SDL_Rect * view);

#endif