#ifndef _AFFICHAGE_H_
#define _AFFICHAGE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <map.h>
#include <sprite.h>
#include <time.h>
#include <string.h>

#define MAX_TEXTURE_NUMBER 100


/**
 * \file affichage.h
 * \brief Header Gestion Affichage
 * \author Yamis MANFALOTI
 * \version 6.0
 * \date 16 mars 2023
 *
 * Header Gestion de l'affichage:
 * \n Signature des fonction externe de affichage.c
 */

/* ------------------------------------------------ */
/* ------------------ Structure  ------------------ */
/* ------------------------------------------------ */

/**
 * \typedef SDL_timer_s SDL_timer_t Structure Timer
 * \struct SDL_timer_t
 * \brief Structure Timer Basé Sur SDL_GetTicks()
 * 
 * Contient:
 * \n  Uint32 start , Le temps quand le timer est lancé
 * \n Uint32 now , Le temps courant au dernier appele des fonctions
*/
typedef struct SDL_timer_s {
    Uint32 start;
    Uint32 now;
}SDL_timer_t;

/**
 * \typedef Sprite_Texture_s Sprite_Texture_t Structure Sprite Texture
 * \struct Sprite_Texture_t
 * \brief Structure Sprite Texture
 * 
 * Contient:
 * \n  char * spriteSheet , Chemin du spriteSheet
 * \n SDL_Texture * spriteSheetTexture , Texture du spriteSheet
*/
typedef struct Sprite_Texture_s {
    char * spriteSheet;
    SDL_Texture * spriteSheetTexture;

}Sprite_Texture_t;

/**
 * \typedef Sprite_Texture_Liste_s Sprite_Texture_Liste_t Structure Liste Sprite Texture
 * \struct Sprite_Texture_Liste_t
 * \brief Structure Liste Sprite Texture
 * 
 * Contient:
 * \n Sprite_Texture_t * tab[MAX_TEXTURE_NUMBER] , tableau texture sprite
 * \n int nbElem , nombre d'element dans le tableau
*/
typedef struct Sprite_Texture_Liste_s {
    Sprite_Texture_t * tab[MAX_TEXTURE_NUMBER];
    int nbElem;
} Sprite_Texture_Liste_t;


/* ------------------------------------------------------ */
/* ------------------ Fonction Externe ------------------ */
/* ------------------------------------------------------ */

/* ------------------ Gestion SDL------------------ */
extern int Init_SDL(SDL_Window **window, SDL_Renderer **renderer, int width, int height);
extern void Quit_SDL(SDL_Window *window, SDL_Renderer *renderer);

/* ------------------ Destruction Texture ------------------ */
extern void Detruire_Texture(SDL_Texture ** texture) ;

/* ------------------ Gestion Fenetre & Affichage  ------------------ */
extern void getWinInfo(SDL_Window *window, int * width, int * height, int tileSize, SDL_Rect * view, int * dstCoef, int * xBorder, int * yBorder);
extern void changeResolution(int indiceResolution, int indiceFullscreen, SDL_Window *window);

/* ------------------ Afficher Image ( avec ou sans texture ) ------------------ */
extern int Afficher_IMG(char * IMG, SDL_Renderer *renderer, SDL_Texture **texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect ) ;

/* ------------------ Gestion Texture Sprite ------------------ */
extern Sprite_Texture_Liste_t * Init_Sprite_Texture_Liste() ;
extern int Chercher_Texture(Sprite_Texture_Liste_t *liste, char *spriteSheet) ;
extern int Ajouter_Texture(Sprite_Texture_Liste_t *liste, char *spriteSheet, SDL_Texture *texture) ;
extern int Load_Sprite_Texture_Liste(Sprite_Texture_Liste_t *SpriteTexteListe, sprite_type_liste_t * listeType, SDL_Renderer *renderer ) ;
extern void Detruire_Sprite_Texture_Liste(Sprite_Texture_Liste_t **liste) ;

extern int Affichage_All(personnage_t * perso, int tabSkill[3], SDL_Texture * textSkillBar[4], SDL_Texture * texture, map_t * map, Sprite_Texture_Liste_t *SpriteTextureListe, sprite_t **** spriteMap, sprite_type_liste_t * listeType, SDL_Window * window, TTF_Font * font, SDL_Renderer *renderer, SDL_Rect * view) ;
/* ------------------ Gestion Des Frames Des Sprites ------------------ */
extern void AddFrame(sprite_t **** spriteMap, int frameCat, sprite_type_liste_t * listeType, map_t * map, SDL_Rect * view) ;

/* ------------------ Gestion Des Timers ------------------ */
extern void Timer_Start( SDL_timer_t * timer ) ;
extern void Timer_Update( SDL_timer_t * timer ) ; 
extern Uint32 Timer_Get_Time( SDL_timer_t * timer ) ;

/* ------------------ Gestion Deplacement Attaque ------------------ */
extern int Deplacement_PersoSprite(sprite_t **** spriteMap, map_t * map, sprite_liste_t * spritePersoList, SDL_Rect * view, char Action ) ;

/* ------------------ Gestion Animation Attaque ------------------ */
extern int Attack_PersoSprite(sprite_t **** spriteMap, map_t * map, sprite_liste_t * spritePersoList , SDL_Rect * view, char Action ) ;
extern int Special_PersoSprite(sprite_t **** spriteMap, map_t * map, sprite_liste_t * spritePersoList , SDL_Rect * view, char Action ) ;
extern int Ultime_PersoSprite(sprite_t **** spriteMap, map_t * map, sprite_liste_t * spritePersoList , SDL_Rect * view, char Action ) ;

/* ------------------ Affichage Texte ------------------ */
extern int Afficher_Texte_Zone(SDL_Renderer* renderer, TTF_Font* font, const char* text, int y, int x, int w, SDL_Color * textColor) ;

extern void drawLine(SDL_Renderer *renderer, int y1, int x1, int y2, int x2, int thickness, SDL_Color color) ;

#endif 