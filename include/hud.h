#ifndef HUD_H
#define HUD_H

#include <affichage.h>
#include <data.h>
#include <dirent.h>

/**
 *	\file hud.h
 *  \brief Header pour l'hud
 *  \author Yamis MANFALOTI
 *  \version 1.0
 *  \date 24 mars 2023
**/

typedef struct inventaire_s {
    // Données inventaire
    int **inventaire;
    int nbPlaceInventaire;
    int invHeight;
    int invWidth;

    // Données inventaire equipement
    int **equipement;
    int nbPlaceEquipement;
    int equipementHeight;
    int equipementWidth;

    // Texture
    SDL_Texture * textInventaire;
    SDL_Texture * textItem;
    SDL_Texture * textSelecteur;
    SDL_Texture * textItemInfo;

    // Coordonée selecteur
    int selecteurX;
    int selecteurY;
    
}inventaire_t;

typedef struct liste_texture_pnj_dialogue_s {
    SDL_Texture ** tabTexture;
    int nbElem;
}liste_texture_pnj_dialogue_t;


/* ------------------ Affichage Texte ------------------ */
extern int Afficher_Texte_Zone(SDL_Renderer* renderer, TTF_Font* font, const char* text, int y, int x, int w, SDL_Color * textColor) ;

/* ------------------ Gestion Inventaire ------------------ */
extern inventaire_t * Load_Inventaire(char * cheminInventaire, char * cheminItem, char * cheminSelecteur, char * cheminItemInfo, int invHeight, int invWidth, int equipementHeight, int equipementWidth, SDL_Renderer * renderer) ;
extern void Detruire_Inventaire(inventaire_t ** inventaire) ;

extern int Add_Item_Inventaire(inventaire_t * inventaire, liste_objet_t * listeObjets, int itemID) ;
extern int Use_Item_Inventaire(inventaire_t * inventaire, liste_objet_t * listeObjets, personnage_t * perso) ;

extern int Afficher_Inventaire(inventaire_t * inventaire, personnage_t * perso, sprite_t ** matPreviewPerso, Sprite_Texture_Liste_t *SpriteTextureListe, sprite_type_liste_t * listeType, SDL_Rect * view, TTF_Font* font, SDL_Window *window, SDL_Renderer *renderer) ;

//extern int Inventaire(inventaire_t * inventaire, liste_objet_t * listeObjets, personnage_t * perso, Sprite_Texture_Liste_t *SpriteTextureListe, sprite_type_liste_t * listeType, sprite_liste_t * spritePersoList, SDL_Rect * view, SDL_Window *window, SDL_Renderer *renderer) ;
extern int Inventaire(inventaire_t * inventaire, liste_objet_t * listeObjets, personnage_t * perso, Sprite_Texture_Liste_t *SpriteTextureListe, sprite_type_liste_t * listeType, sprite_liste_t * spritePersoList, SDL_Rect * view, SDL_Window *window, SDL_Texture * background_texture, SDL_Renderer *renderer) ;

/* ------------------ Gestion Dialogue ------------------ */
extern liste_texture_pnj_dialogue_t * Load_Liste_Texture_Pnj_Dialogue(const char * path, SDL_Renderer * renderer) ;

extern int Dialogue(SDL_Texture * textDialogue, liste_texture_pnj_dialogue_t * ListeTextPnjDialogue, SDL_Rect * view, SDL_Window *window, SDL_Renderer *renderer) ;

#endif

