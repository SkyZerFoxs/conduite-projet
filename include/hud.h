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

/* ------------------------------------------------ */
/* ------------------ Structure  ------------------ */
/* ------------------------------------------------ */

/**
 * \typedef inventaire_s inventaire_t Structure inventaire
 * \struct inventaire_t
 * \brief Structure de l'inventaire du personnage ( Data & Affichage )
 * 
* Contient:
* \n int **inventaire, un pointeur de pointeur vers un tableau 2D d'entiers représentant l'inventaire
* \n int nbPlaceInventaire, le nombre total de places dans l'inventaire
* \n int invHeight, la hauteur de l'inventaire
* \n int invWidth, la largeur de l'inventaire
* \n int **equipement, un pointeur de pointeur vers un tableau 2D d'entiers représentant l'équipement
* \n int nbPlaceEquipement, le nombre total de places dans l'équipement
* \n int equipementHeight, la hauteur de l'inventaire d'équipement
* \n int equipementWidth, la largeur de l'inventaire d'équipement
* \n SDL_Texture * textInventaire, un pointeur vers une texture SDL représentant la texture de l'inteface d'inventaire
* \n SDL_Texture * textItem, un pointeur vers une texture SDL représentant la texture des objets
* \n SDL_Texture * textSelecteur, un pointeur vers une texture SDL représentant la texture du sélecteur
* \n SDL_Texture * textItemInfo, un pointeur vers une texture SDL représentant la texture de l'inteface des informations d'un objet
* \n int selecteurX, la position X du sélecteur
* \n int selecteurY, la position Y du sélecteur
*/
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

/**
 * \typedef liste_texture_pnj_dialogue_s liste_texture_pnj_dialogue_t Structure liste texture pnj dialogue
 * \struct liste_texture_pnj_dialogue_t
 * \brief Structure de la liste des textures des images de pnj lors du dialogue ( Affichage )
 * 
 * Contient:
 * \n SDL_Texture ** tabTexture, tableau / liste des différentes textures
 * \n int nbElem, nombre d'élements dans le tableau
*/
typedef struct liste_texture_pnj_dialogue_s {
    SDL_Texture ** tabTexture;
    int nbElem;
}liste_texture_pnj_dialogue_t;


/* ------------------------------------------------------ */
/* ------------------ Fonction Externe ------------------ */
/* ------------------------------------------------------ */

/* ------------------ Gestion HUD Inventaire ------------------ */
// Gestion Item
extern inventaire_t * Load_Inventaire(char * cheminInventaire, char * cheminItem, char * cheminSelecteur, char * cheminItemInfo, int invHeight, int invWidth, int equipementHeight, int equipementWidth, SDL_Renderer * renderer) ;
extern void Detruire_Inventaire(inventaire_t ** inventaire) ;
extern int Update_Equipement_Stat(inventaire_t * inventaire, liste_objet_t * listeObjets, personnage_t * perso) ;

// Gestion Utilité Item
extern int Add_Item_Inventaire(inventaire_t * inventaire, liste_objet_t * listeObjets, int itemID) ;
extern int Use_Item_Inventaire(inventaire_t * inventaire, liste_objet_t * listeObjets, personnage_t * perso) ;

//Fonction Boucle Inventaire
extern int Inventaire(inventaire_t * inventaire, liste_objet_t * listeObjets, personnage_t * perso, Sprite_Texture_Liste_t *SpriteTextureListe, sprite_type_liste_t * listeType, sprite_liste_t * spritePersoList, SDL_Rect * view, SDL_Window *window, SDL_Texture * background_texture, SDL_Renderer *renderer) ;

/* ------------------ Gestion HUD Dialogue ------------------ */

// Gestopn texture Pnj Dialogue
extern liste_texture_pnj_dialogue_t * Load_Liste_Texture_Pnj_Dialogue(liste_type_pnj_t * liste_type, SDL_Renderer * renderer) ;
extern void Detruire_Liste_Texture_Pnj_Dialogue(liste_texture_pnj_dialogue_t ** liste_texture_pnj) ;

// Fonction Boucle Dialogue
extern int Dialogue(SDL_Texture * textDialogue, liste_texture_pnj_dialogue_t * ListeTextPnjDialogue, pnj_t * pnj, liste_type_pnj_t * listeTypePnj, SDL_Rect * view, SDL_Window *window, SDL_Renderer *renderer) ;

/* ------------------ Gestion HUD Level UP ------------------ */

// Fonction Boucle Dialogue
extern int Level_UP(SDL_Texture * textFondLevelUP, SDL_Texture * background_texture, personnage_t * perso, SDL_Rect * view, SDL_Window *window, SDL_Renderer *renderer) ;

/* ------------------ Gestion Mort Joueur  ------------------ */
extern int Mort_Joueur( personnage_t * perso, inventaire_t * inventaire, liste_objet_t *listeObjets ) ;

extern int Death(SDL_Texture * textFondDeath, SDL_Texture * background_texture, SDL_Rect * view, SDL_Window *window, SDL_Renderer *renderer) ;

/* ------------------ Gestion Introduction  ------------------ */
extern int Introduction(SDL_Window * window, SDL_Renderer *renderer, SDL_Rect * view ) ;

/* ------------------ Gestion Ending  ------------------ */
extern int Ending(SDL_Window * window, SDL_Renderer *renderer, SDL_Rect * view ) ;

#endif

