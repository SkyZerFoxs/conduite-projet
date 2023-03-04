#ifndef _AFFICHAGE_H_
#define _AFFICHAGE_H_

/**
 * \file affichage.h
 * \brief Header Gestion Affichage
 * \author Yamis MANFALOTI
 * \version 4.0
 * \date 04 mars 2023
 *
 * Header Gestion de l'affichage:
 * \n Signature des fonction externe de affichage.c
 */

#include <map.h>
#include <sprite.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

extern int Init_SDL(SDL_Window **window, SDL_Renderer **renderer, int width, int height);
extern void Quit_SDL(SDL_Window *window, SDL_Renderer *renderer);

extern void Detruire_Texture(SDL_Texture *texture);

extern void changeResolution(int indiceResolution, int indiceFullscreen, SDL_Window *window);
extern void getWinInfo(SDL_Window *window, int * width, int * height, int tileSize, SDL_Rect * view, int * dstCoef, int * xBorder, int * yBorder);

extern void Afficher_IMG(char * IMG, SDL_Renderer *renderer, SDL_Texture **texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect );

extern int Afficher_Tile(char * tileSet, int tileSize, int dstCoef, int xBorder, int yBorder, int tileNumber, int colonne, int ligne, SDL_Rect * view, SDL_Renderer *renderer, SDL_Texture **texture);
extern void Afficher_Map(char * tileSet, map_t * map, SDL_Renderer *renderer, SDL_Rect * view, int dstCoef, int xBorder, int yBorder);

extern void Afficher_Sprite(sprite_t * sprite, sprite_type_liste_t *listeType, SDL_Renderer * renderer,  SDL_Rect * view, int dstCoef, int xBorder, int yBorder) ;
extern void Afficher_SpriteMap( sprite_t *** spriteMap, map_t * map, sprite_type_liste_t * listeType, SDL_Rect * view, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder) ;

extern void Affichage_All(char * tileSet, map_t * map, sprite_t *** spriteMap,  sprite_type_liste_t * listeType, SDL_Window * window, SDL_Renderer *renderer, SDL_Rect * view) ;
extern void AddFrame(sprite_t *** spriteMap, map_t * map, SDL_Rect * view) ;

#endif