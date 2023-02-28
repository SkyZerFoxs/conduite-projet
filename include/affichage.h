#ifndef _AFFICHAGE_H_
#define _AFFICHAGE_H_

/**
 * \file affichage.h
 * \brief Header Gestion Affichage
 * \author Yamis MANFALOTI
 * \version 3.0
 * \date 20 février 2023
 *
 * Header Gestion de l'affichage:
 * \n Signature des fonction externe de affichage.c
 */

#include <map.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/**
 * \typedef sprite_s sprite_t Structure Sprite
 * \struct sprite_t
 * \brief Structure des Sprites
 * 
 * Contient:
 * \n x             : Coordonnées x du sprite
 * \n y             : Coordonnées y du sprite
 * \n frame         : Frame courante du sprite
 * \n frameNumber   : Frame finale/maximal du sprite
 * \n spriteSheet   : Chemin du spriteSheet 
 * \n spriteSize    : Taille en pixel du sprite
 * \n spriteLine    : Indice de ligne du sprite dans la spriteSheet
 * 
 */
typedef struct sprite_s {
    //Coordonnées
    int x;
    int y;

    //Sa frame courante
    int frame;

    //Son frameNumber
    int frameNumber;

    // Metadonnées Texture Sprite
    char * spriteSheet;
    int spriteSize;
    int spriteLine;

}sprite_t;

extern int Init_SDL(SDL_Window **window, SDL_Renderer **renderer, int width, int height);
extern void Quit_SDL(SDL_Window *window, SDL_Renderer *renderer);
extern void Afficher_IMG(char * IMG, SDL_Renderer *renderer, SDL_Texture **texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect );
extern void Detruire_Texture(SDL_Texture *texture);
extern void getWinInfo(SDL_Window *window, int * width, int * height, int tileSize, SDL_Rect * view, int * dstCoef, int * xBorder, int * yBorder);
extern void changeResolution(int indiceResolution, int indiceFullscreen, SDL_Window *window);
extern int Afficher_Tile(char * tileSet, int tileSize, int dstCoef, int xBorder, int yBorder, int tileNumber, int colonne, int ligne, SDL_Rect * view, SDL_Renderer *renderer, SDL_Texture **texture);
extern void Afficher_Map(char * tileSet, map_t * map, SDL_Window *window, SDL_Renderer *renderer, SDL_Rect * view);
extern sprite_t * Load_Sprite(int x, int y, int frame, int frameNumber, char * spriteSheet, int spriteSize, int spriteLine);
extern void Afficher_Sprite(sprite_t * sprite, SDL_Renderer * renderer);
extern void Detruire_Sprite( sprite_t ** sprite);


#endif