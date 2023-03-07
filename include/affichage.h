#ifndef _AFFICHAGE_H_
#define _AFFICHAGE_H_

/**
 * \file affichage.h
 * \brief Header Gestion Affichage
 * \author Yamis MANFALOTI
 * \version 4.5
 * \date 07 mars 2023
 *
 * Header Gestion de l'affichage:
 * \n Signature des fonction externe de affichage.c
 */

#include <map.h>
#include <sprite.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct SDL_timer_s {
    Uint32 start;
    Uint32 now;
}SDL_timer_t;

extern int Init_SDL(SDL_Window **window, SDL_Renderer **renderer, int width, int height);
extern void Quit_SDL(SDL_Window *window, SDL_Renderer *renderer);

extern void Detruire_Texture(SDL_Texture *texture);

extern void getWinInfo(SDL_Window *window, int * width, int * height, int tileSize, SDL_Rect * view, int * dstCoef, int * xBorder, int * yBorder);
extern void changeResolution(int indiceResolution, int indiceFullscreen, SDL_Window *window);

extern int Afficher_IMG(char * IMG, SDL_Renderer *renderer, SDL_Texture **texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect ) ;

extern int Afficher_Map(SDL_Texture * texture, map_t * map, SDL_Rect * view, SDL_Renderer *renderer, int dstCoef, int xBorder, int yBorder ) ;

extern void Afficher_Sprite(sprite_t * sprite, sprite_type_liste_t *listeType, SDL_Renderer * renderer,  SDL_Rect * view, int dstCoef, int xBorder, int yBorder) ;
extern void Afficher_SpriteMap( sprite_t *** spriteMap, map_t * map, sprite_type_liste_t * listeType, SDL_Rect * view, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder) ;

extern int Affichage_All(SDL_Texture * texture, map_t * map, sprite_t *** spriteMap, sprite_type_liste_t * listeType, SDL_Window * window, SDL_Renderer *renderer, SDL_Rect * view) ;

extern void AddFrame(sprite_t *** spriteMap, int frameCat, sprite_type_liste_t * listeType, map_t * map, SDL_Rect * view) ;

extern void Timer_Start( SDL_timer_t * timer ) ;
extern void Timer_Update( SDL_timer_t * timer ) ; 
extern Uint32 Timer_Get_Time( SDL_timer_t * timer ) ;

#endif