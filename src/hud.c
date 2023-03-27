#include <stdlib.h>

#include <hud.h>

extern int Afficher_Texte_Zone(SDL_Renderer* renderer, TTF_Font* font, const char* text, int y, int x, int w, SDL_Color * textColor)
{
    // Rectangle Destination ( Renderer )
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = 0;
    rect.w = w;
    
    // Surface pour le text
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text, (*textColor), rect.w);
    if ( textSurface == NULL ) {
        printf("Erreur : Echec TTF_RenderText_Blended_Wrapped() dans Afficher_Texte_Zone()\n");
        return 1;
    }

    // Texture depuis la surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if ( textTexture == NULL ) {
        printf("Erreur : Echec TTF_RenderText_Blended_Wrapped() dans Afficher_Texte_Zone()\n");
        return 1;
    }

    // Rectangle de destination
    SDL_Rect destRect = { rect.x, rect.y, textSurface->w, textSurface->h };

    // Ajoute de la texture au renderer
    if (  SDL_RenderCopy(renderer, textTexture, NULL, &destRect) < 0 ) {
        printf("Erreur : Echec SDL_RenderCopy() dans Afficher_Texte_Zone()\n");
        return 1;
    }

    // Netoyage
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);

    return 0;
}

extern inventaire_t * Load_Inventaire(char * cheminInventaire, char * cheminItem, char * cheminSelecteur, char * cheminItemInfo, int invHeight, int invWidth, int equipementHeight, int equipementWidth, SDL_Renderer * renderer) {
    if (invHeight <= 0 || invWidth <= 0) {
        printf("Erreur : Mauvaise dimension d'inventaire dans Load_Inventaire()\n");
        return NULL;
    }

    if (equipementHeight <= 0 || equipementWidth <= 0) {
        printf("Erreur : Mauvaise dimension d'equipement dans Load_Inventaire()\n");
        return NULL;
    }

    inventaire_t* inventaire = malloc(sizeof(inventaire_t));
    if (inventaire == NULL) {
        printf("Erreur : Echec malloc(inventaire) dans Load_Inventaire()\n");
        return NULL;
    }

    inventaire->invHeight = invHeight;
    inventaire->invWidth = invWidth;
    inventaire->equipementHeight = equipementHeight;
    inventaire->equipementWidth = equipementWidth;

    // Allocation de la matrice d'inventaire
    inventaire->inventaire = malloc(inventaire->invHeight * sizeof(int*));
    if (inventaire->inventaire == NULL) {
        printf("Erreur : Echec malloc(inventaire->inventaire) dans Load_Inventaire()\n");
        free(inventaire);
        return NULL;
    }

    inventaire->nbPlaceInventaire = 0;  // initialisation à 0
    for (int i = 0; i < inventaire->invHeight; i++) {
        inventaire->inventaire[i] = malloc(invWidth * sizeof(int));
        if (inventaire->inventaire[i] == NULL) {
            printf("Erreur : Echec malloc(inventaire->inventaire[%d]) dans Load_Inventaire()\n", i);
            for (int j = 0; j < i; j++) {
                if ( inventaire->inventaire[j] != NULL ) {
                    free(inventaire->inventaire[j]);
                }
            }
            free(inventaire->inventaire);
            free(inventaire);
            return NULL;
        }
        for (int j = 0; j < invWidth; j++) {
            inventaire->inventaire[i][j] = -1;  // initialisation à -1
            inventaire->nbPlaceInventaire++;
        }
    }

    // Allocation de la matrice d'equipement
    inventaire->equipement = malloc(inventaire->equipementHeight * sizeof(int*));
    if (inventaire->equipement == NULL) {
        printf("Erreur : Echec malloc(inventaire->equipement) dans Load_Inventaire()\n");
        // Libération de la matrice d'inventaire
        for (int i = 0; i < inventaire->invHeight; i++) {
            free(inventaire->inventaire[i]);
        }
        free(inventaire->inventaire);
        free(inventaire);
        return NULL;
    }

    inventaire->nbPlaceEquipement = 0;  // initialisation à 0
    for (int i = 0; i < inventaire->equipementHeight; i++) {
        inventaire->equipement[i] = malloc(equipementWidth * sizeof(int));
        if (inventaire->equipement[i] == NULL) {
            printf("Erreur : Echec malloc(inventaire->equipement[%d]) dans Load_Inventaire()\n", i);
            // Libération de la matrice d'equipement
            for (int j = 0; j < i; j++) {
                free(inventaire->equipement[j]);
            }
            free(inventaire->equipement);
            // Libération de la matrice d'inventaire
            for (int j = 0; j < inventaire->invHeight; j++) {
                free(inventaire->inventaire[j]);
            }
            free(inventaire->inventaire);
            free(inventaire);
            return NULL;
        }
        for (int j = 0; j < equipementWidth; j++) {
            inventaire->equipement[i][j] = -1;  // initialisation à -1
            inventaire->nbPlaceEquipement++;
        }
    }

    inventaire->textInventaire = IMG_LoadTexture(renderer,cheminInventaire);
    if ( inventaire->textInventaire == NULL ) {
        printf("Erreur : Echec IMG_LoadTexture(textInventaire) dans Load_Inventaire()\n");
        return NULL;
    }

    inventaire->textItem = IMG_LoadTexture(renderer,cheminItem);
    if ( inventaire->textItem == NULL ) {
        printf("Erreur : Echec IMG_LoadTexture(textItem) dans Load_Inventaire()\n");
        return NULL;
    }

    inventaire->textSelecteur = IMG_LoadTexture(renderer,cheminSelecteur);
    if ( inventaire->textSelecteur == NULL ) {
        printf("Erreur : Echec IMG_LoadTexture(textSelecteur) dans Load_Inventaire()\n");
        return NULL;
    }

    inventaire->textItemInfo = IMG_LoadTexture(renderer,cheminItemInfo);
    if ( inventaire->textItemInfo == NULL ) {
        printf("Erreur : Echec IMG_LoadTexture(textItemInfo) dans Load_Inventaire()\n");
        return NULL;
    }

    inventaire->selecteurX = 8;
    inventaire->selecteurY = 2;

    return inventaire;

}

extern void Detruire_Inventaire(inventaire_t** inventaire) {
    // Verification paramètres
    if (inventaire == NULL || *inventaire == NULL) {
        printf("Erreur : Inventaire en paramètre inexistant dans Detruire_Inventaire()\n");
        return;
    }

    if ( (*inventaire)->inventaire == NULL ) {
        printf("Erreur : Inventaire vide dans Detruire_Inventaire()\n");
        return;
    }

    if ( (*inventaire)->equipement == NULL ) {
        printf("Erreur : Inventaire Equipement vide dans Detruire_Inventaire()\n");
        return;
    }

    // Libération de la texture d'inventaire
    if ( (*inventaire)->textInventaire != NULL ) {
        Detruire_Texture(&((*inventaire)->textInventaire));
    }
    
    // Libération de la texture d'item
    if ( (*inventaire)->textItem != NULL ) {
        Detruire_Texture(&((*inventaire)->textItem));
    }
    
    // Libération de la matrice d'inventaire
    for (int i = 0; i < (*inventaire)->invHeight; i++) {
        if ( (*inventaire)->inventaire[i] != NULL ) {
            free((*inventaire)->inventaire[i]);
            (*inventaire)->inventaire[i] = NULL;
        }
        
    }
    free((*inventaire)->inventaire);
    (*inventaire)->inventaire = NULL;

    // Libération de la matrice d'equipement
    for (int i = 0; i < (*inventaire)->equipementHeight; i++) {
        if ( (*inventaire)->equipement[i] != NULL ) {
            free((*inventaire)->equipement[i]);
            (*inventaire)->equipement[i] = NULL;
        }
    }
    free((*inventaire)->equipement);
    (*inventaire)->equipement = NULL;
    
    // Libération de la structure inventaire_t
    free(*inventaire);
    *inventaire = NULL;
}


static int Afficher_Fond_Inventaire(inventaire_t * inventaire, int  dstCoef, int  xBorder, int  yBorder, SDL_Renderer *renderer) {
    // Verification paramètres
    if ( inventaire->textInventaire == NULL ) {
       printf("Erreur : La texture de l'hud d'inventaire n'est pas chargé dans Afficher_Inventaire()\n");
       return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur : Le Renderer n'est pas chargé dans Afficher_Inventaire()\n");
       return 1;
    }

    if ( dstCoef == 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur : Le WinInfo Incorrecte dans Afficher_Inventaire()\n");
        return 1;
    }

    // Affichage des tiles de la carte
    int tileNumber = 0;
    for (int y = 0; y < 11; y++) {
        for (int x = 0; x < 20; x++) {
            SDL_Rect srcrect;
            srcrect.y = 16 * ( (tileNumber) / 20);
            srcrect.x = 16 * ( (tileNumber) % 20);
            srcrect.h = 16;
            srcrect.w = 16;

            // Rectangle Destination ( Renderer )
            SDL_Rect dstrect;
            dstrect.x = ( dstCoef * (16 * x ) ) + xBorder;
            dstrect.y = ( dstCoef * (16 * y ) ) + yBorder; 
            dstrect.h = dstCoef * 16;
            dstrect.w = dstCoef * 16;

            // Affiche La Tile Obtenue Grace Au Rectangle Source Vers Le Rectangle Destination Dans Le Renderer
            if ( SDL_RenderCopy(renderer, inventaire->textInventaire, &srcrect, &dstrect) < 0 ) {
                printf("Erreur : SDL_RenderCopy() à échoué dans Afficher_Inventaire\n");
                return 1;
            }
            tileNumber++;
        }
    }

    return 0;
}

static int Afficher_Item_Inventaire(inventaire_t * inventaire, int  dstCoef, int  xBorder, int  yBorder, SDL_Renderer *renderer) {

    // Verification paramètres
    if ( inventaire->textItem == NULL ) {
       printf("Erreur : La texture des items n'est pas chargé dans Afficher_Item()\n");
       return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur : Le Renderer n'est pas chargé dans Afficher_Item()\n");
       return 1;
    }

    if ( dstCoef == 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur : Le WinInfo Incorrecte dans Afficher_Item()\n");
        return 1;
    }

    int itemID;

    // Affichage item inventaire
    for (int y = 2; y < 8; y++) {
        for (int x = 3; x < 17; x++) {
            // Recuperation de l'id des items dans l'inventaire
            if ( y >= 2 && y < 8 && x >= 8 && x < 17 ) {
                if ( y-2 < 0 || x-8 < 0 ) {
                    printf("Erreur : Erreur inventaire->inventaire[y-2][x-8] dans Afficher_Item()");
                    return 1;
                }
                itemID = inventaire->inventaire[y-2][x-8];
            }
            // Recuperation de l'id des items dans l'inventaire d'equipement
            else if ( y >= 2 && y < 5 && x >= 3 && x < 5 ) {
                if ( y-2 < 0 || x-3 < 0 ) {
                    printf("Erreur : Erreur inventaire->equipement[y-2][x-3] dans Afficher_Item()");
                    return 1;
                }
                itemID = inventaire->equipement[y-2][x-3];
            }
            // Sinon
            else {
                itemID = -1;
            }
            
            if ( itemID != -1) {
                // Rectangle Source ( Texture )
                SDL_Rect srcrect;
                srcrect.y = 16 * ( (itemID) / 6);
                srcrect.x = 16 * ( (itemID) % 6);
                srcrect.h = 16;
                srcrect.w = 16;

                // Rectangle Destination ( Renderer )
                SDL_Rect dstrect;
                dstrect.x = ( dstCoef * (16 * x ) ) + xBorder;
                dstrect.y = ( dstCoef * (16 * y ) ) + yBorder; 
                dstrect.h = dstCoef * 16;
                dstrect.w = dstCoef * 16;

                // Affiche L'Item Correspondant Grace Au Rectangle Source Vers Le Rectangle Destination Dans Le Renderer
                if ( SDL_RenderCopy(renderer, inventaire->textItem, &srcrect, &dstrect) < 0 ) {
                    printf("Erreur : SDL_RenderCopy(inventaire) à échoué dans Afficher_Item()\n");
                    return 1;
                }
            }   
        }
    }

    return 0;
}

static int Afficher_Selecteur_Inventaire(inventaire_t * inventaire, int  dstCoef, int  xBorder, int  yBorder, SDL_Renderer *renderer) {

    // Verification paramètres
    if ( inventaire->textSelecteur == NULL ) {
       printf("Erreur : La texture du selecteur n'est pas chargé dans Afficher_Selecteur_Inventaire()\n");
       return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur : Le Renderer n'est pas chargé dans Afficher_Selecteur_Inventaire()\n");
       return 1;
    }

    if ( inventaire->selecteurX < 3 || inventaire->selecteurX >= 17 ) {
        printf("Erreur : Le inventaire->selecteurX invalide dans Afficher_Selecteur_Inventaire()\n");
        return 1;
    }

    if ( inventaire->selecteurY < 2 || inventaire->selecteurY >= 8 ) {
        printf("Erreur : Le inventaire->selecteurY invalide dans Afficher_Selecteur_Inventaire()\n");
        return 1;
    }

    if ( dstCoef == 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur : Le WinInfo Incorrecte dans Afficher_Selecteur_Inventaire()\n");
        return 1;
    }

    // Rectangle Destination ( Renderer )
    SDL_Rect dstrect;
    dstrect.x = ( dstCoef * (16 * inventaire->selecteurX ) ) + xBorder;
    dstrect.y = ( dstCoef * (16 * inventaire->selecteurY ) ) + yBorder; 
    dstrect.h = dstCoef * 16;
    dstrect.w = dstCoef * 16;

    // Affiche L'Item Correspondant Grace Au Rectangle Source Vers Le Rectangle Destination Dans Le Renderer
    if ( SDL_RenderCopy(renderer, inventaire->textSelecteur, NULL, &dstrect) < 0 ) {
        printf("Erreur : SDL_RenderCopy(inventaire) à échoué dans Afficher_Selecteur_Inventaire()\n");
        return 1;
    }

    return 0;
}

static int Afficher_PersoPreview_Inventaire(sprite_t ** mat, Sprite_Texture_Liste_t *SpriteTextureListe, sprite_type_liste_t * listeType, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder) {
    // Verification paramètres
    if ( SpriteTextureListe == NULL ) {
       printf("Erreur : La SpriteTextureListe n'est pas chargé dans Afficher_PersoPreview()\n");
       return 1;
    }

    if ( mat == NULL ) {
       printf("Erreur : La matrice des sprites de preview n'est pas chargé dans Afficher_PersoPreview()\n");
       return 1;
    }

    if ( listeType == NULL ) {
       printf("Erreur : La listeType n'est pas chargé dans Afficher_PersoPreview()\n");
       return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur : Le Renderer n'est pas chargé dans Afficher_PersoPreview()\n");
       return 1;
    }

    if ( dstCoef < 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur : WinInfo Incorrecte dans Afficher_PersoPreview()\n");
        return 1;
    }
    for (int n = 0; n < 2; n++) {
        sprite_t * sprite = mat[n];

        // Recupération des informations lié au sprite ( pour la lisibilité du code )
        sprite_type_t * spriteType = listeType->typeListe[sprite->spriteTypeId];
        int spriteSize = spriteType->spriteSize;
        int frameNumber = spriteType->frameNumber;
        int spriteLine = spriteType->spriteLine;

        // Recupération de la texture du SpriteSheet
        SDL_Texture * texture = SpriteTextureListe->tab[spriteType->textureId]->spriteSheetTexture;
    
        // Rectangle Source ( SpriteSheet )
        SDL_Rect rectSrc;
        rectSrc.x = spriteSize * ( sprite->frame = sprite->frame % frameNumber);
        rectSrc.y = spriteSize * spriteLine;
        rectSrc.w = spriteSize;
        rectSrc.h = spriteSize;

        // Rectangle Destination ( Renderer )
        SDL_Rect rectDst;
        rectDst.x = ( dstCoef * (spriteSize * 6) ) + xBorder;
        rectDst.y = ( dstCoef * (spriteSize * (2.5 + n) ) ) + yBorder; 
        rectDst.h = dstCoef * spriteSize;
        rectDst.w = dstCoef * spriteSize;

        // Affichage de la frame courante du sprite
        if ( SDL_RenderCopy(renderer, texture, &rectSrc, &rectDst) < 0 ) {
            printf("Erreur : SDL_RenderCopy() à échoué dans Afficher_PersoPreview()\n");
            return 1;
        }
        
    }

    return 0;
}

static int Afficher_Item_Info_Inventaire(inventaire_t * inventaire, liste_objet_t * listeObjets, TTF_Font* font, int  dstCoef, int  xBorder, int  yBorder, SDL_Renderer *renderer) {

    // Verification paramètres
    if ( inventaire == NULL ) {
       printf("Erreur : L'invetaire n'est pas chargé dans Afficher_Item_Info_Inventaire()\n");
       return 1;
    }

    if ( inventaire->textItemInfo == NULL ) {
       printf("Erreur : La texture du selecteur n'est pas chargé dans Afficher_Item_Info_Inventaire()\n");
       return 1;
    }
    
    if ( listeObjets == NULL ) {
       printf("Erreur : La listeObjets n'est pas chargée dans Afficher_Item_Info_Inventaire()\n");
       return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur : Le Renderer n'est pas chargé dans Afficher_Item_Info_Inventaire()\n");
       return 1;
    }

    if ( inventaire->selecteurX < 3 || inventaire->selecteurX >= 17 ) {
        printf("Erreur : Le inventaire->selecteurX invalide dans Afficher_Item_Info_Inventaire()\n");
        return 1;
    }

    if ( inventaire->selecteurY < 2 || inventaire->selecteurY >= 8 ) {
        printf("Erreur : Le inventaire->selecteurY invalide dans Afficher_Item_Info_Inventaire()\n");
        return 1;
    }

    if ( dstCoef == 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur : Le WinInfo Incorrecte dans Afficher_Item_Info_Inventaire()\n");
        return 1;
    }

    // Affichage des stats

    int yItem, xItem, itemID = -1;
    // zone equipement
    if ( inventaire->selecteurX < 8 && inventaire->selecteurY < 5 ) {
        // calcule emplacement matrice
        xItem = inventaire->selecteurX - 3;
        yItem = inventaire->selecteurY - 2;
        itemID = inventaire->equipement[yItem][xItem];
    }
    // zone inventaire
    if ( inventaire->selecteurX >= 8 ) {
        // calcule emplacement matrice
        xItem = inventaire->selecteurX - 8;
        yItem = inventaire->selecteurY - 2;
        itemID = inventaire->inventaire[yItem][xItem];
    }

    if ( itemID == -1 ) {
        return -1;
    }

    if ( itemID >= listeObjets->nbElem ) {
        printf("Erreur : itemID incorecte dans Afficher_Item_Info_Inventaire()\n");
        return 1;
    }

    // Affichage des tiles du hud
    int tileNumber = 0;
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 3; x++) {
            SDL_Rect srcrect;
            srcrect.y = 16 * ( (tileNumber) / 3);
            srcrect.x = 16 * ( (tileNumber) % 3);
            srcrect.h = 16;
            srcrect.w = 16;

            // Rectangle Destination ( Renderer )
            SDL_Rect dstrect;
            dstrect.x = ( dstCoef * (16 * ( x + inventaire->selecteurX - 1 ) ) ) + xBorder;
            dstrect.y = ( dstCoef * (16 * ( y + inventaire->selecteurY - 2 ) ) ) + yBorder; 
            dstrect.h = dstCoef * 16;
            dstrect.w = dstCoef * 16;

            // Affiche La Tile Obtenue Grace Au Rectangle Source Vers Le Rectangle Destination Dans Le Renderer
            if ( SDL_RenderCopy(renderer, inventaire->textItemInfo, &srcrect, &dstrect) < 0 ) {
                printf("Erreur : SDL_RenderCopy() à échoué dans Afficher_Item_Info_Inventaire\n");
                return 1;
            }
            tileNumber++;
        }
    }

    // Affichage des stats du personnage
    SDL_Color marronCLaireInventaire = { 51, 32, 24, 255 };

    char * statLabel[6] = {
        listeObjets->tab[itemID]->nom,
        "lvl",
        "nb",
        "pv",
        "atk",
        "def",
    };

    int * statValue[6] = {
        NULL,
        &(listeObjets->tab[itemID]->niv),
        &(listeObjets->tab[itemID]->nb),
        &(listeObjets->tab[itemID]->stats->pv),
        &(listeObjets->tab[itemID]->stats->atk),
        &(listeObjets->tab[itemID]->stats->def)
    }; 


    int x = ( dstCoef * (16 * ( inventaire->selecteurX - 1 + 0.40 ) ) ) + xBorder;
    int w = dstCoef * 16 * 2.5;
    int y;
    int palierY = (dstCoef * 16 * 1.40 ) / 4;
    for (int i = 0; i < 4; i++) {
        char string[256];
        if (i == 0) {
            // First stat line
            sprintf(string, "%-15s",statLabel[0]);
        }
        if ( i == 1 ) {
            sprintf(string, "%-3s %-1d   %-3s:%-1d",statLabel[1],*statValue[1],statLabel[2],*statValue[2]);
        }
        if ( i == 2 ) {
            sprintf(string, "%-5s %-5s %-5s", statLabel[3], statLabel[4], statLabel[5]);
        }
        if ( i == 3 ) {
            // Subsequent stat lines
            sprintf(string, "%-5d %-5d %-5d", *statValue[3], *statValue[4], *statValue[5]);
        }

        y = ( dstCoef * (16 * ( inventaire->selecteurY - 1.66 ) ) ) + yBorder + (palierY * i);
        if ( Afficher_Texte_Zone(renderer, font, string, y, x, w, &marronCLaireInventaire) ) {
            printf("Erreur : Echec Afficher_Texte_Zone() dans Afficher_Inventaire()\n");
            return 1;
        }
    }

    return 0;
}

extern int Afficher_Inventaire(inventaire_t * inventaire, personnage_t * perso, sprite_t ** matPreviewPerso, Sprite_Texture_Liste_t *SpriteTextureListe, sprite_type_liste_t * listeType, SDL_Rect * view, TTF_Font* font, SDL_Window *window, SDL_Renderer *renderer) {

    int win_width,win_height;
    int dstCoef, xBorder, yBorder;
    
    // Récupération des informations de la fenêtre utile à l'affichage
    getWinInfo(window, &win_width, &win_height, 16, view, &dstCoef, &xBorder, &yBorder);

    if ( Afficher_Fond_Inventaire(inventaire,dstCoef,xBorder,yBorder,renderer) ) {
        printf("Erreur : Echec Afficher_Fond() dans Afficher_Inventaire()\n");
        return 1;
    }

    if ( Afficher_Item_Inventaire(inventaire,dstCoef,xBorder,yBorder,renderer) ) {
        printf("Erreur : Echec Afficher_Fond() dans Afficher_Inventaire()\n");
        return 1;
    }

    if ( Afficher_PersoPreview_Inventaire( matPreviewPerso, SpriteTextureListe, listeType, renderer, dstCoef, xBorder, yBorder) ) {
        printf("Erreur : Echec Afficher_PersoPreview() dans Afficher_Inventaire()\n");
        return 1;
    }

    if ( Afficher_Selecteur_Inventaire(inventaire,dstCoef,xBorder,yBorder,renderer) ) {
        printf("Erreur : Echec Afficher_Selecteur_Inventaire() dans Afficher_Inventaire()\n");
        return 1;
    }

    // calcule des stats du personnage
    caract_t stats = {0, 0, 0, 0};
    if ( calculer_stats_perso(perso,&stats) ) {
        printf("Erreur : Echec calculer_stats_perso() dans Afficher_Inventaire()\n");
        return 1;
    }

    // Affichage des stats du personnage
    SDL_Color marronCLaireInventaire = { 51, 32, 24, 255 };

    char * statLabel[5] = {
        "Niveau",
        "Experience",
        "Points de vie",
        "Attaque",
        "Defense"
    };

    int * statValue[5] = {
        &(perso->niveau),
        &(perso->exp),
        &(perso->caract->pv),
        &(stats.atk),
        &(stats.def)
    }; 

    int * statMaxValue[5] = {
        NULL,
        &(perso->palierExp),
        &(stats.maxPv),
        NULL,
        NULL
    };

    int x = ( dstCoef * 16 * 3.125) + xBorder ;
    int w = dstCoef * 16 * 4.5;
    int y;
    int palierY = (dstCoef * 16 * 2.5 ) / 5;
    for (int i = 0; i < 5; i++) {
        char string[256];
        if ( statMaxValue[i] != NULL ) {
            sprintf(string, "%-14s: %d/%d", statLabel[i], *statValue[i],*statMaxValue[i]);
        }
        else {
            sprintf(string, "%-14s: %d", statLabel[i], *statValue[i]);
        }

        y = (dstCoef * 16 * 5.39 ) + yBorder + (palierY * i);
        if ( Afficher_Texte_Zone(renderer, font, string, y, x, w, &marronCLaireInventaire) ) {
            printf("Erreur : Echec Afficher_Texte_Zone() dans Afficher_Inventaire()\n");
            return 1;
        }
    }
    
    return 0;
}

static int Deplacement_Curseur_Inventaire(inventaire_t * inventaire, char direction ) {
    // Verification paramètres
    if ( inventaire == NULL ) {
       printf("Erreur : L'inventaire n'est pas chargé dans Drop_Item_Inventaire()\n");
       return 1;
    }

    if ( inventaire->selecteurX < 3 || inventaire->selecteurX >= 17 ) {
        printf("Erreur : Le inventaire->selecteurX invalide dans Drop_Item_Inventaire()\n");
        return 1;
    }

    if ( inventaire->selecteurY < 2 || inventaire->selecteurY >= 8 ) {
        printf("Erreur : Le inventaire->selecteurY invalide dans Drop_Item_Inventaire()\n");
        return 1;
    }

    switch ( direction )
    {
    case 'z':
        if ( (inventaire->selecteurY - 1) >= 2 ) {
            inventaire->selecteurY--;
        }
        break;
    case 'q':
        if ( (inventaire->selecteurX - 1) >= 3) {
            if ( inventaire->selecteurX == 8 && inventaire->selecteurY < 5 ) {
                inventaire->selecteurX = 5;
            }
            if ( inventaire->selecteurX == 8 && inventaire->selecteurY >= 5) {
                inventaire->selecteurX = 9;
            }
            inventaire->selecteurX--;
        }
        break;
    case 's':
        if ( (inventaire->selecteurY + 1) < 8) {
            if (inventaire->selecteurX >= 8 || ( inventaire->selecteurX < 8 && inventaire->selecteurY < 4) ) {
                inventaire->selecteurY++;
            }
        }
        break;
    case 'd':
        if ( (inventaire->selecteurX + 1) < 17) {
            if ( inventaire->selecteurX == 4) {
                inventaire->selecteurX = 7;
            }
            inventaire->selecteurX++;
        }
        break;
    default:
        printf("Erreur : direction invalide dans Deplacement_Curseur_Inventaire()\n");
        return 1;
        break;
    }

    return 0;
}

static int Drop_Item_Inventaire(inventaire_t * inventaire, liste_objet_t * listeObjets, int dropMethode ) {
    // Verification paramètres
    if ( inventaire == NULL ) {
       printf("Erreur : L'inventaire n'est pas chargé dans Drop_Item_Inventaire()\n");
       return 1;
    }

    if ( listeObjets == NULL ) {
       printf("Erreur : La listeObjets n'est pas chargé dans Drop_Item_Inventaire()\n");
       return 1;
    }

    if ( dropMethode < 0 || dropMethode > 1 ) {
        printf("Erreur : La methode de drop est incorrect dans Drop_Item_Inventaire()\n");
       return 1;
    }

    if ( inventaire->selecteurX < 3 || inventaire->selecteurX > 17 ) {
        printf("Erreur : Le inventaire->selecteurX invalide dans Drop_Item_Inventaire()\n");
        return 1;
    }

    if ( inventaire->selecteurY < 2 || inventaire->selecteurY > 8 ) {
        printf("Erreur : Le inventaire->selecteurY invalide dans Drop_Item_Inventaire()\n");
        return 1;
    }

    int ** mat = inventaire->inventaire;
    int x = inventaire->selecteurX - 8;
    int y = inventaire->selecteurY - 2;
    if ( inventaire->selecteurX < 8 && inventaire->selecteurY < 5 ) {
        mat = inventaire->equipement;
        x = inventaire->selecteurX - 3;
        y = inventaire->selecteurY - 2;
    }

    if ( mat[y][x] != -1 ) {
        switch ( dropMethode )
        {
        case 0:
            listeObjets->tab[mat[y][x]]->nb--;
            break;
        case 1:
            listeObjets->tab[mat[y][x]]->nb = 0;
            break;
        default:
            printf("Erreur : dropMethode incorrect dans Drop_Item_Inventaire()\n");
            return 1;
            break;
        }

        if ( listeObjets->tab[mat[y][x]]->nb <= 0 ) {
            listeObjets->tab[mat[y][x]]->nb = 0;
            mat[y][x] = -1;
        }
    }

    return 0;
}

extern int Add_Item_Inventaire(inventaire_t * inventaire, liste_objet_t * listeObjets, int itemID) {
    if ( inventaire == NULL ) {
       printf("Erreur : L'inventaire n'est pas chargé dans Add_Item_Inventaire()\n");
       return 1;
    }

    if ( listeObjets == NULL ) {
       printf("Erreur : La listeObjets n'est pas chargée dans Add_Item_Inventaire()\n");
       return 1;
    }

    if ( itemID < 0 || itemID >= listeObjets->nbElem ) {
        printf("Erreur : L'id de l'objet a ajouter est incorrecte dans Add_Item_Inventaire()\n");
        return 1;
    }

    if ( inventaire->invHeight < 0 || inventaire->invHeight > 6 || inventaire->invWidth < 0 || inventaire->invWidth > 9 ) {
        printf("Erreur : dimension inventaire[%d][%d] incorrecte dans Add_Item_Inventaire()\n",inventaire->invHeight,inventaire->invWidth);
        return 1;
    }

    int emptyY = -1,  emptyX = -1;
    int existingY = -1,  existingX = -1;
    for (int y = 0; y < inventaire->invHeight; y++ ) {
        for ( int x = 0; x < inventaire->invWidth; x++) {
            if ( inventaire->inventaire[y][x] == -1 && emptyY == -1 && emptyX == -1) {
                emptyY = y;
                emptyX = x;
            }
            if ( inventaire->inventaire[y][x] == itemID ) {
                existingY = y;
                existingX = x;
            }
        }
    }

    if ( existingY != -1 && existingX != -1 ) {
        listeObjets->tab[inventaire->inventaire[existingY][existingX]]->nb++;
    }
    else {
        listeObjets->tab[itemID]->nb++;
        inventaire->inventaire[emptyY][emptyX] = itemID;
    }

    return 0;
}

extern int Use_Item_Inventaire(inventaire_t * inventaire, liste_objet_t * listeObjets, personnage_t * perso) {
    if ( inventaire == NULL ) {
       printf("Erreur : L'inventaire n'est pas chargé dans Use_Item_Inventaire()\n");
       return 1;
    }

    if ( listeObjets == NULL ) {
       printf("Erreur : La listeObjets n'est pas chargée dans Use_Item_Inventaire()\n");
       return 1;
    }

    if ( perso == NULL ) {
       printf("Erreur : Le perso n'est pas chargée dans Use_Item_Inventaire()\n");
       return 1;
    }

    if ( inventaire->selecteurX < 3 || inventaire->selecteurX > 17 ) {
        printf("Erreur : Le inventaire->selecteurX invalide dans Use_Item_Inventaire()\n");
        return 1;
    }

    if ( inventaire->selecteurY < 2 || inventaire->selecteurY > 8 ) {
        printf("Erreur : Le inventaire->selecteurY invalide dans Use_Item_Inventaire()\n");
        return 1;
    }

    // item a desequiper
    if ( inventaire->selecteurX < 8 && inventaire->selecteurY < 5 ) {
        // calcule emplacement matrice
        int x = inventaire->selecteurX - 3;
        int y = inventaire->selecteurY - 2;
        int itemID = inventaire->equipement[y][x];
        // Si il y a un item a desequiper
        if ( itemID != -1 ) {
            int typeItem = listeObjets->tab[itemID]->typeID;
            // si l'item est un equipement
            if ( typeItem >= 0 && typeItem < 6 ) {
                // reset stats equipement
                perso->equipement[typeItem]->maxPv = 0;
                perso->equipement[typeItem]->atk = 0;
                perso->equipement[typeItem]->def = 0;
                // Cherche emplacement libre dans l'inventaire
                int emptyY = -1,  emptyX = -1;
                for (int y = 0; y < inventaire->invHeight; y++ ) {
                    for ( int x = 0; x < inventaire->invWidth; x++) {
                        if ( inventaire->inventaire[y][x] == -1 && emptyY == -1 && emptyX == -1) {
                            emptyY = y;
                            emptyX = x;
                        }
                    }
                }
                // Deplace l'item desequiper vers l'emplacement libre
                int temp = inventaire->inventaire[emptyY][emptyX];
                inventaire->inventaire[emptyY][emptyX] = itemID;
                inventaire->equipement[y][x] = temp;
            }
            // gestion d'erreur / cas imprevu
            else {
                printf("Erreur : Le type de l'item tab[%d] a desequiper est invalide dans Use_Item_Inventaire()\n",itemID);
                return 1;
            }
        }
    }

    // item a equiper / conssomer
    if ( inventaire->selecteurX >= 8 ) {
        // calcule emplacement matrice
        int x = inventaire->selecteurX - 8;
        int y = inventaire->selecteurY - 2;
        int itemID = inventaire->inventaire[y][x];
        // Si il y a un item a equiper / conssomer
        if ( itemID != -1 ) {
            int typeItem = listeObjets->tab[itemID]->typeID;
            // Si l'item est un conssomalbe
            if  ( typeItem == -1 )  {
                // calcule des stats du personnage
                caract_t stats = {0, 0, 0, 0};
                if ( calculer_stats_perso(perso,&stats) ) {
                    printf("Erreur : Echec calculer_stats_perso() dans Afficher_Inventaire()\n");
                    return 1;
                }
                // detection debordement pv
                perso->caract->pv += listeObjets->tab[itemID]->stats->pv;
                if ( perso->caract->pv > stats.maxPv  ) {
                    perso->caract->pv = stats.maxPv;
                }
                // drop de l'item utilise ( nb-- )
                if (  Drop_Item_Inventaire(inventaire,listeObjets,0) ) {
                    printf("Erreur : Echec Drop_Item_Inventaire() dans Afficher_Inventaire()\n");
                    return 1;
                }
                
            }
            // si l'item est un equipement
            else if ( typeItem >= 0 && typeItem < 6 ) {
                // changement stats equipement
                perso->equipement[typeItem]->maxPv = listeObjets->tab[itemID]->stats->pv;
                perso->equipement[typeItem]->atk = listeObjets->tab[itemID]->stats->atk;
                perso->equipement[typeItem]->def = listeObjets->tab[itemID]->stats->def;
                // deplacement item vers la zone d'equipement
                int temp = itemID;
                int equipementY = typeItem / 2;
                int equipementX = typeItem % 2;
                inventaire->inventaire[y][x] = inventaire->equipement[equipementY][equipementX];
                inventaire->equipement[equipementY][equipementX] = temp;
            }
            // gestion d'erreur / cas imprevu
            else {
                printf("Erreur : Le type de l'item tab[%d] a equiper / conssomer est invalide dans Use_Item_Inventaire()\n",itemID);
                return 1;
            }
        }
    }

    return 0;
}

extern int Inventaire(inventaire_t * inventaire, liste_objet_t * listeObjets, personnage_t * perso, Sprite_Texture_Liste_t *SpriteTextureListe, sprite_type_liste_t * listeType, sprite_liste_t * spritePersoList, SDL_Rect * view, SDL_Window *window, SDL_Texture * background_texture, SDL_Renderer *renderer) {
    /* ------------------ Initialisation variable ------------------ */

    // statut des erreurs
    int erreur = 0;

     // Nombre De FPS A Afficher
    int FRAME_PER_SECONDE = 30;

    // Nombre De Ms Par Frame Produite
    int msPerFrame;

    // Variable Pour Quitter La Boucle Principal
    int quit = SDL_FALSE;

    // Variable qui detecte si une touche est deja été préssé
    int keyPressed = 0;

    // Variable qui detecte si la touche pour afficher les info d'item a été préssé
    int afficherInfoItem = 0;

    // variable 
    int resultAfficherInfoItem;

    // Variable getWinInfo
    int win_width,win_height;
    int dstCoef, xBorder, yBorder;

    // Variable SDL_Event Pour Detecter Les Actions
    SDL_Event event;

    // initialisation des timers
    SDL_timer_t fps;
    SDL_timer_t frameTimer1;
    SDL_timer_t TimerInfoItem;

    // Direction deplacement curseur
    char direction;

    // Sprite Animation Preview Personnage
    sprite_t * mat[2];
    mat[0] = spritePersoList->spriteListe[0];
    mat[1] = spritePersoList->spriteListe[1];

    (void)listeObjets;

    /* ------------------ Initialisation resource jeux ------------------ */

    // initialisation des variables
    TTF_Font* font1 = NULL;
    TTF_Font* font2 = NULL;

    getWinInfo(window, &win_width, &win_height, 0, NULL, NULL, NULL, NULL);

    if ( win_width > 1000 && win_width < 1400 ) {
        // Initalisation Font
        font1 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 20);
        if (font1 == NULL) 
        {
            printf("Erreur : Echec TTF_OpenFont(font24) dans Inventaire()");
            return 1;
        }

        font2 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 16);
        if (font2 == NULL) 
        {
            printf("Erreur : Echec TTF_OpenFont(font20) dans Inventaire()");
            return 1;
        }
    }
    else {
        // Initalisation Font
        font1 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 24);
        if (font1 == NULL) 
        {
            printf("Erreur : Echec TTF_OpenFont(font24) dans Inventaire()");
            return 1;
        }

        font2 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 20);
        if (font2 == NULL) 
        {
            printf("Erreur : Echec TTF_OpenFont(font20) dans Inventaire()");
            return 1;
        }
    }

    if ( inventaire == NULL ) {
        printf("Erreur : inventaire non Initialisé dans Inventaire()");
        return 1;
    }

    // Debut Des Timers De Frame Pour Les Sprites
    Timer_Start( &frameTimer1 );

    /* ------------------ Test ( a supprimer ) ------------------ */

    srand(time(NULL));

    /* ------------------ Boucle Principal ------------------ */

    while( quit == SDL_FALSE && !erreur ) {
        /* --------- Variable Boucle --------- */
        // Lancement timer temps d'execution
        Timer_Start( &fps );

        // Reset keyPressed
        keyPressed = 0;

        /* ------- Detection Evenement -------*/

        //Si on affiche les informations d'un item
        if ( afficherInfoItem ) {
            while (SDL_PollEvent(&event)) {
                // Switch Event
                switch (event.type) {
                    // Evenement QUIT
                    case SDL_QUIT:
                        quit = SDL_TRUE;
                        erreur = -1;
                        break;
                    // Evenement Touche Clavier
                    case SDL_KEYDOWN:
                        if (  !keyPressed ) {
                            switch (event.key.keysym.sym) {
                                case SDLK_i:
                                    afficherInfoItem = 0;
                                    break;
                                default:
                                    break;
                            }
                            keyPressed = 1;
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        // Si on n'affiche pas les informations d'un item
        if ( !afficherInfoItem ) {
            while (SDL_PollEvent(&event)) {
                // Switch Event
                switch (event.type) {
                    // Evenement QUIT
                    case SDL_QUIT:
                        quit = SDL_TRUE;
                        erreur = -1;
                        break;
                    // Evenement Touche Clavier
                    case SDL_KEYDOWN:
                        if (  !keyPressed ) {
                            // Gestion Touche Clavier
                            switch (event.key.keysym.sym) {
                                case SDLK_ESCAPE:
                                    quit = SDL_TRUE;
                                    break;
                                case SDLK_TAB:
                                    quit = SDL_TRUE;
                                    break;
                                case SDLK_z:
                                    direction = 'z';
                                    break;
                                case SDLK_q:
                                    direction = 'q';
                                    break;
                                case SDLK_s:
                                    direction = 's';
                                    break;
                                case SDLK_d: 
                                    direction = 'd';
                                    break;
                                case SDLK_i:
                                    afficherInfoItem = 1;
                                    Timer_Start(&TimerInfoItem);
                                    break;
                                case SDLK_e:
                                    if ( Use_Item_Inventaire(inventaire, listeObjets, perso ) ) {
                                        printf("Erreur : Echec Use_Item_Inventaire() dans Inventaire()\n");
                                        erreur = 1;
                                    }
                                    break;
                                case SDLK_f:
                                    if ( Drop_Item_Inventaire(inventaire, listeObjets, 0 ) ) {
                                        printf("Erreur : Echec Drop_Item_Inventaire() dans Inventaire()\n");
                                        erreur = 1;
                                    }
                                    break;
                                case SDLK_g:
                                    if ( Drop_Item_Inventaire(inventaire, listeObjets, 1 ) ) {
                                        printf("Erreur : Echec Drop_Item_Inventaire() dans Inventaire()\n");
                                        erreur = 1;
                                    }
                                    break;
                                default:
                                    break;
                            }
                            if  ( event.key.keysym.sym == SDLK_z || event.key.keysym.sym == SDLK_q ||
                                event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_d    ) 
                            {
                                if ( Deplacement_Curseur_Inventaire(inventaire,direction) ) {
                                    printf("Erreur : Echec Deplacement_Curseur_Inventaire() dans Inventaire()\n");
                                    erreur = 1;
                                }
                            }
                            keyPressed = 1;
                            
                        }
                        break;
                }
            }
        }
        
        /* --------- Gestion Frame Sprite --------- */ 

        // Gestion Frame Perso
        if ( (int)Timer_Get_Time( &frameTimer1 ) > 200 ) {
            mat[0]->frame++;
            mat[1]->frame++;
            Timer_Start( &frameTimer1 );
        }

        /* --------- Gestion Affichage --------- */

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, background_texture, NULL, NULL);

        // Affichage de l'inventaire
        if ( Afficher_Inventaire(inventaire, perso, mat, SpriteTextureListe, listeType, view, font1, window, renderer) ) {
            printf("Erreur : Echec Afficher_Inventaire() dans Inventaire()\n");
            erreur = 1;
        }

        // Affichage de la boite d'information d'item
        if ( afficherInfoItem == 1 ) {
            // Récupération des informations de la fenêtre utile à l'affichage
            getWinInfo(window, &win_width, &win_height, 16, view, &dstCoef, &xBorder, &yBorder);
            // Affichage Info Item
            resultAfficherInfoItem = Afficher_Item_Info_Inventaire(inventaire,listeObjets,font2,dstCoef,xBorder,yBorder,renderer);
            if ( resultAfficherInfoItem == -1 ) {
                afficherInfoItem = 0;
            }
            if ( resultAfficherInfoItem == 1 ) {
                printf("Erreur : Echec Afficher_Selecteur_Inventaire() dans Afficher_Inventaire()\n");
                return 1;
            }
        }
        if ( afficherInfoItem == 1 && (int)Timer_Get_Time( &TimerInfoItem ) > 3000 ) {
            afficherInfoItem = 0;
        }
        
        // Gestion fps
        if ( ( msPerFrame = (int)Timer_Get_Time( &fps ) ) < (1000 / FRAME_PER_SECONDE) ) {
            SDL_Delay( (1000 / FRAME_PER_SECONDE)  - msPerFrame );
        }

        // mise à jour du renderer ( update affichage)
        SDL_RenderPresent(renderer);
        
    }         
        
    TTF_CloseFont(font2);
    TTF_CloseFont(font1);

    return erreur;
}

extern liste_texture_pnj_dialogue_t * Load_Liste_Texture_Pnj_Dialogue(const char * path, SDL_Renderer * renderer) {
    DIR *dir;
    struct dirent *entry;
    int nbFiles = 0;

    // Ouverture dossier
    dir = opendir(path);
    if (dir == NULL) {
        printf("Erreur : Echec ouverture dossier(%s) dans liste_texture_pnj_dialogue_t()\n",path);
        return NULL;
    }

    // Comptage nombre fichier .png
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".png") != NULL) {
            nbFiles++;
        }
    }

    // Allocation memoire liste_texture_pnj_dialogue_t
    liste_texture_pnj_dialogue_t * liste = (liste_texture_pnj_dialogue_t*) malloc(sizeof(liste_texture_pnj_dialogue_t));
    if ( liste == NULL ) {
        printf("Erreur : Echec malloc(liste) dans liste_texture_pnj_dialogue_t()\n");
        closedir(dir);
        return NULL;
    }

    liste->nbElem = nbFiles;

    // Allocation memoire tabTexture
    liste->tabTexture = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * nbFiles);
    if ( liste->tabTexture == NULL ) {
        printf("Erreur : Echec malloc(tabTexture) dans liste_texture_pnj_dialogue_t()\n");
        free(liste);
        closedir(dir);
        return NULL;
    }

    // Chargement texture fichier .png
    rewinddir(dir);
    int i = 0;
    while ((entry = readdir(dir)) != NULL && i < nbFiles) {
        if (strstr(entry->d_name, ".png") != NULL) {
            char filePath[1024];
            snprintf(filePath, sizeof(filePath), "%s/%s", path, entry->d_name);
            SDL_Texture *texture = IMG_LoadTexture(renderer, filePath);
            if ( texture == NULL ) {
                printf("Erreur : Echec IMG_LoadTexture() dans liste_texture_pnj_dialogue_t()\n");
                for (int j = 0; j < i; j++) {
                    if ( liste->tabTexture[j] != NULL ) {
                        Detruire_Texture( &(liste->tabTexture[j]) );
                    }
                }
                free(liste->tabTexture);
                free(liste);
                closedir(dir);
                return NULL;
            }
            liste->tabTexture[i] = texture;
            i++;
        }
    }

    closedir(dir);

    return liste;
}


static int Afficher_Fond_Boite_Dialogue(SDL_Texture * textDialogue, int  dstCoef, int  xBorder, int  yBorder, SDL_Renderer *renderer) {
    // Verification paramètres
    if (textDialogue == NULL ) {
       printf("Erreur : La texture n'est pas chargé dans Afficher_Inventaire()\n");
       return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur : Le Renderer n'est pas chargé dans Afficher_Inventaire()\n");
       return 1;
    }

    if ( dstCoef == 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur : Le WinInfo Incorrecte dans Afficher_Inventaire()\n");
        return 1;
    }

    // Affichage des tiles de la carte
    int tileNumber = 0;
    for (int y = 0; y < 11; y++) {
        for (int x = 0; x < 20; x++) {
            SDL_Rect srcrect;
            srcrect.y = 16 * ( (tileNumber) / 20);
            srcrect.x = 16 * ( (tileNumber) % 20);
            srcrect.h = 16;
            srcrect.w = 16;

            // Rectangle Destination ( Renderer )
            SDL_Rect dstrect;
            dstrect.x = ( dstCoef * (16 * x ) ) + xBorder;
            dstrect.y = ( dstCoef * (16 * y ) ) + yBorder; 
            dstrect.h = dstCoef * 16;
            dstrect.w = dstCoef * 16;

            // Affiche La Tile Obtenue Grace Au Rectangle Source Vers Le Rectangle Destination Dans Le Renderer
            if ( SDL_RenderCopy(renderer, textDialogue, &srcrect, &dstrect) < 0 ) {
                printf("Erreur : SDL_RenderCopy() à échoué dans Afficher_Inventaire\n");
                return 1;
            }
            tileNumber++;
        }
    }

    return 0;
}

static int Afficher_Pnj_Dialogue(liste_texture_pnj_dialogue_t * listeTextPnj, int pnjID, int  dstCoef, int  xBorder, int  yBorder, SDL_Renderer *renderer) {
    // Verification paramètres
    if (listeTextPnj == NULL) {
       printf("Erreur : La liste en paramètre est incorrecte dans Afficher_Pnj_Dialogue()\n");
       return 1;
    }

    if ( pnjID >= listeTextPnj->nbElem  || listeTextPnj->tabTexture[pnjID] == NULL ) {
       printf("Erreur : La texture du pnj est incorrecte dans Afficher_Pnj_Dialogue()\n");
       return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur : Le Renderer n'est pas chargé dans Afficher_Pnj_Dialogue()\n");
       return 1;
    }

    if ( dstCoef == 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur : Le WinInfo Incorrecte dans Afficher_Pnj_Dialogue()\n");
        return 1;
    }

    // Affichage des tiles de la carte
    int tileNumber = 0;
    for (int y = 0; y < 11; y++) {
        for (int x = 0; x < 20; x++) {
            SDL_Rect srcrect;
            srcrect.y = 16 * ( (tileNumber) / 20);
            srcrect.x = 16 * ( (tileNumber) % 20);
            srcrect.h = 16;
            srcrect.w = 16;

            // Rectangle Destination ( Renderer )
            SDL_Rect dstrect;
            dstrect.x = ( dstCoef * (16 * x ) ) + xBorder;
            dstrect.y = ( dstCoef * (16 * y ) ) + yBorder; 
            dstrect.h = dstCoef * 16;
            dstrect.w = dstCoef * 16;

            // Affiche La Tile Obtenue Grace Au Rectangle Source Vers Le Rectangle Destination Dans Le Renderer
            if ( SDL_RenderCopy(renderer, listeTextPnj->tabTexture[pnjID], &srcrect, &dstrect) < 0 ) {
                printf("Erreur : SDL_RenderCopy() à échoué dans Afficher_Pnj_Dialogue\n");
                return 1;
            }
            tileNumber++;
        }
    }

    return 0;
}

extern int Afficher_Dialogue(SDL_Texture * textDialogue, liste_texture_pnj_dialogue_t * listeTextPnjDialogue, SDL_Rect * view, TTF_Font* font, SDL_Window *window, SDL_Renderer *renderer) {
    int win_width,win_height;
    int dstCoef, xBorder, yBorder;
    
    // Récupération des informations de la fenêtre utile à l'affichage
    getWinInfo(window, &win_width, &win_height, 16, view, &dstCoef, &xBorder, &yBorder);

    if ( Afficher_Fond_Boite_Dialogue(textDialogue,dstCoef,xBorder,yBorder,renderer) ) {
        printf("Erreur : Echec Afficher_Fond_Boite_Dialogue() dans Afficher_Dialogue()\n");
        return 1;
    }

    if ( Afficher_Pnj_Dialogue(listeTextPnjDialogue,0,dstCoef,xBorder,yBorder,renderer) ) {
        printf("Erreur : Echec Afficher_Fond_Boite_Dialogue() dans Afficher_Dialogue()\n");
        return 1;
    }

    (void)font;
    
    return 0;

}

extern int Dialogue(SDL_Texture * textDialogue, liste_texture_pnj_dialogue_t * listeTextPnjDialogue, SDL_Rect * view, SDL_Window *window, SDL_Renderer *renderer) {
    /* ------------------ Initialisation variable ------------------ */

    // statut des erreurs
    int erreur = 0;

     // Nombre De FPS A Afficher
    int FRAME_PER_SECONDE = 30;

    // Nombre De Ms Par Frame Produite
    int msPerFrame;

    // Variable Pour Quitter La Boucle Principal
    int quit = SDL_FALSE;

    // Variable qui detecte si une touche est deja été préssé
    int keyPressed = 0;

    // Variable SDL_Event Pour Detecter Les Actions
    SDL_Event event;

    // initialisation des timers
    SDL_timer_t fps;

    /* ------------------ Initialisation resource jeux ------------------ */

    // initialisation des variables
    /*
    TTF_Font* font = NULL;

    // Initalisation Font
    font = TTF_OpenFont("asset/arial.ttf", 28);
    if (font == NULL) 
    {
        printf("Erreur : Echec TTF_OpenFont dans Inventaire()");
        return 1;
    }
    */

    // Debut Des Timers De Frame Pour Les Sprites

    /* ------------------ Boucle Principal ------------------ */

    while( quit == SDL_FALSE ) {
        /* --------- Variable Boucle --------- */
        // Lancement timer temps d'execution
        Timer_Start( &fps );

        // Reset keyPressed
        keyPressed = 0;

        /* ------- Detection Evenement -------*/
        while (SDL_PollEvent(&event)) {
            // Switch Event
            switch (event.type) {
                // Evenement QUIT
                case SDL_QUIT:
                    quit = SDL_TRUE;
                    erreur = -1;
                    break;
                // Evenement Touche Clavier
                case SDL_KEYDOWN:
                    if (  !keyPressed ) {
                            // Gestion Touche Clavier
                            switch (event.key.keysym.sym) {
                                case SDLK_ESCAPE:
                                    quit = SDL_TRUE;
                                    break;
                                case SDLK_RETURN:
                                    break;
                            }
                    }
                    break;
            }
        }
        
        /* --------- Gestion Frame Sprite --------- */ 


        /* --------- Gestion Affichage --------- */

        int win_width,win_height;
        int dstCoef, xBorder, yBorder;
    
        // Récupération des informations de la fenêtre utile à l'affichage
        getWinInfo(window, &win_width, &win_height, 16, view, &dstCoef, &xBorder, &yBorder);

        if ( Afficher_Dialogue(textDialogue,listeTextPnjDialogue,view,NULL,window,renderer) ) {
            printf("Erreur : Echec Afficher_Dialogue() dans Dialogue()\n");
            return 1;
        }
        
        // Gestion fps
        if ( ( msPerFrame = (int)Timer_Get_Time( &fps ) ) < (1000 / FRAME_PER_SECONDE) ) {
            SDL_Delay( (1000 / FRAME_PER_SECONDE)  - msPerFrame );
        }

        // mise à jour du renderer ( update affichage)
        SDL_RenderPresent(renderer);
        
    }         
        
    //TTF_CloseFont(font);

    return erreur;
}