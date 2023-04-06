#include <stdlib.h>

#include <hud.h>

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
        "niv",
        "nb",
        "vie",
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
            sprintf(string, "%-3s %-1d   %-2s:%-3d",statLabel[1],*statValue[1],statLabel[2],*statValue[2]);
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

static int Afficher_Inventaire(inventaire_t * inventaire, personnage_t * perso, sprite_t ** matPreviewPerso, Sprite_Texture_Liste_t *SpriteTextureListe, sprite_type_liste_t * listeType, SDL_Rect * view, TTF_Font* font, SDL_Window *window, SDL_Renderer *renderer) {

    // Initialisation variable getWinInfo
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
    
    /* ------------------ Detection Erreur Parametre ------------------ */

    // Vérification de la variable inventaire
    if ( inventaire == NULL ) {
        printf("Erreur : inventaire non Initialisé dans Inventaire()");
        return 1;
    }

    // Vérification de la variable listeObjets
    if (listeObjets == NULL) {
        printf("Erreur : listeObjets non initialisée dans Inventaire()");
        return 1;
    }

    // Vérification de la variable perso
    if (perso == NULL) {
        printf("Erreur : perso non initialisé dans Inventaire()");
        return 1;
    }

    // Vérification de la variable SpriteTextureListe
    if (SpriteTextureListe == NULL) {
        printf("Erreur : SpriteTextureListe non initialisée dans Inventaire()");
        return 1;
    }

    // Vérification de la variable listeType
    if (listeType == NULL) {
        printf("Erreur : listeType non initialisée dans Inventaire()");
        return 1;
    }

    // Vérification de la variable spritePersoList
    if (spritePersoList == NULL) {
        printf("Erreur : spritePersoList non initialisée dans Inventaire()");
        return 1;
    }

    // Vérification de la variable view
    if (view == NULL) {
        printf("Erreur : view non initialisée dans Inventaire()");
        return 1;
    }

    // Vérification de la variable window
    if (window == NULL) {
        printf("Erreur : window non initialisée dans Inventaire()");
        return 1;
    }

    // Vérification de la variable background_texture
    if (background_texture == NULL) {
        printf("Erreur : background_texture non initialisée dans Inventaire()");
        return 1;
    }

    // Vérification de la variable renderer
    if (renderer == NULL) {
        printf("Erreur : renderer non initialisé dans Inventaire()");
        return 1;
    }

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
    sprite_t * mat[2] = {
        spritePersoList->spriteListe[0],
        spritePersoList->spriteListe[1],
    };

    /* ------------------ Initialisation resource jeux ------------------ */

    // initialisation des variables
    TTF_Font* font1 = NULL;
    TTF_Font* font2 = NULL;

    getWinInfo(window, &win_width, &win_height, 0, NULL, NULL, NULL, NULL);

    // Gestion font 1280 x 720
    if ( win_width > 1000 && win_width < 1400 ) {
        // Initalisation Font
        font1 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 20);
        if (font1 == NULL) {
            printf("Erreur : Echec TTF_OpenFont(font20) dans Inventaire()");
            return 1;
        }

        font2 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 16);
        if (font2 == NULL) {
            printf("Erreur : Echec TTF_OpenFont(font16) dans Inventaire()");
            return 1;
        }
    }
    // Gestion font 1600 x 900 || 1920 x 1080
    else {
        // Initalisation Font
        font1 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 24);
        if (font1 == NULL) {
            printf("Erreur : Echec TTF_OpenFont(font24) dans Inventaire()");
            return 1;
        }

        font2 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 20);
        if (font2 == NULL) {
            printf("Erreur : Echec TTF_OpenFont(font20) dans Inventaire()");
            return 1;
        }
    }

   

    // Debut Des Timers De Frame Pour Les Sprites
    Timer_Start( &frameTimer1 );

    /* ------------------ Test ( a supprimer ) ------------------ */

    

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
                printf("Erreur : Echec Afficher_Item_Info_Inventaire() dans Afficher_Inventaire()\n");
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

        // mise à jour du renderer ( update affichage )
        SDL_RenderPresent(renderer);
        
    }         
        
    // Reset matrice preview perso
    if ( mat[0] != NULL ) {
        mat[0] = NULL;
    }
    if ( mat[1] != NULL ) {
        mat[1] = NULL;
    }

    // Fermeture Font
    if ( font2 != NULL ) {
        TTF_CloseFont(font2);
    }
    if ( font1 != NULL ) {
        TTF_CloseFont(font1);
    }
    
    // Reset background_texture
    if ( background_texture != NULL ) {
        Detruire_Texture(&background_texture);
    }

    return erreur;
}

extern liste_texture_pnj_dialogue_t * Load_Liste_Texture_Pnj_Dialogue(liste_type_pnj_t * liste_type, SDL_Renderer * renderer) {
    if ( liste_type == NULL ) {
        printf("Erreur : liste_type_pnj en parametre invalide dans Load_Liste_Texture_Pnj_Dialogue()\n");
        return NULL;
    }

    if ( renderer == NULL ) {
        printf("Erreur : renderer en parametre invalide dans Load_Liste_Texture_Pnj_Dialogue()\n");
        return NULL;
    }

    if ( liste_type->nbElem <= 0 ) {
        printf("Erreur : liste_type->nbElem invalide dans Load_Liste_Texture_Pnj_Dialogue()\n");
        return NULL;
    }

    // Allocation memoire liste_texture_pnj_dialogue_t
    liste_texture_pnj_dialogue_t * liste = (liste_texture_pnj_dialogue_t*) malloc(sizeof(liste_texture_pnj_dialogue_t));
    if ( liste == NULL ) {
        printf("Erreur : Echec malloc(liste) dans Load_Liste_Texture_Pnj_Dialogue()\n");
        return NULL;
    }

    liste->nbElem = liste_type->nbElem;

    // Allocation memoire tabTexture
    liste->tabTexture = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * liste->nbElem);
    if ( liste->tabTexture == NULL ) {
        printf("Erreur : Echec malloc(tabTexture) dans Load_Liste_Texture_Pnj_Dialogue()\n");
        free(liste);
        return NULL;
    }

    for (int i = 0; i < liste_type->nbElem; i++) {
        SDL_Texture *texture = IMG_LoadTexture(renderer, liste_type->liste[i]->imageDialogue);
        if ( texture == NULL ) {
            printf("Erreur : Echec IMG_LoadTexture(%s) dans Load_Liste_Texture_Pnj_Dialogue()\n",liste_type->liste[i]->imageDialogue);
            for (int j = 0; j < i; j++) {
                if ( liste->tabTexture[j] != NULL ) {
                    Detruire_Texture( &(liste->tabTexture[j]) );
                }
            }
            free(liste->tabTexture);
            free(liste);
            return NULL;
        }
        liste->tabTexture[i] = texture;
    }

    return liste;
}

extern void Detruire_Liste_Texture_Pnj_Dialogue(liste_texture_pnj_dialogue_t ** liste_texture_pnj) {
    if (liste_texture_pnj == NULL || (*liste_texture_pnj) == NULL) {
        printf("Erreur : liste_texture_pnj en parametre invalide dans Detruire_Liste_Texture_Pnj_Dialogue()\n");
        return;
    }

    if ((*liste_texture_pnj)->tabTexture == NULL) {
        printf("Erreur : Liste de type de pnj vide dans Detruire_Liste_Texture_Pnj_Dialogue()\n");
        return;
    }

    if ( (*liste_texture_pnj)->nbElem < 0 ) {
        printf("Erreur : Nombre d'element invalide dans Detruire_Liste_Texture_Pnj_Dialogue()\n");
        return;
    }

    // Suppression des type de pnj dans le tableau
    for (int i = 0; i < (*liste_texture_pnj)->nbElem; i++) {
        if ((*liste_texture_pnj)->tabTexture[i] != NULL) {
            Detruire_Texture( &((*liste_texture_pnj)->tabTexture[i]) );
            if ( (*liste_texture_pnj)->tabTexture[i] != NULL) {
                printf("Erreur : Impossible de detruire la texture de tabTexture[%d] dans Detruire_Liste_Texture_Pnj_Dialogue()\n", i);
            }
        }
    }

    // free tab texture pnj
    if ( (*liste_texture_pnj)->tabTexture != NULL ) {
        free((*liste_texture_pnj)->tabTexture);
        (*liste_texture_pnj)->tabTexture = NULL;
    }

    // clear nb elem
    (*liste_texture_pnj)->nbElem = -1;

    // Suppression de la structure liste_texture_pnj
    free( (*liste_texture_pnj) );
    (*liste_texture_pnj) = NULL;
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

static int Afficher_Pnj_Dialogue(liste_texture_pnj_dialogue_t * listeTextPnj, pnj_t * pnj, int  dstCoef, int  xBorder, int  yBorder, SDL_Renderer *renderer) {
    // Verification paramètres
    if (listeTextPnj == NULL) {
       printf("Erreur : La liste en paramètre est incorrecte dans Afficher_Pnj_Dialogue()\n");
       return 1;
    }

    if ( pnj == NULL) {
       printf("Erreur : Le pnj en paramètre est incorrecte dans Afficher_Pnj_Dialogue()\n");
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

    // Recuperation id type pnj
    int pnjID = pnj->pnjTypeID;

    if ( pnjID >= listeTextPnj->nbElem  || listeTextPnj->tabTexture[pnjID] == NULL ) {
       printf("Erreur : La texture du pnj (pnjID) est incorrecte dans Afficher_Pnj_Dialogue()\n");
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

static int Afficher_Bouton_Dialogue(SDL_Texture ** mat, int frameButton, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder) {
    // Verification paramètres

    if ( mat == NULL ) {
       printf("Erreur : La matrice des texture de boutton en paramètre est invalide dans Afficher_Bouton_Dialogue()\n");
       return 1;
    }

    if ( mat[0] == NULL || mat[1] == NULL ) {
        printf("Erreur : La matrice des texture de boutton est invalide dans Afficher_Bouton_Dialogue()\n");
        return 1;
    }

    if ( frameButton < 0 || frameButton > 1 ) {
        printf("Erreur : frameButton invalide dans Afficher_Bouton_Dialogue()\n");
        return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur : Le Renderer n'est pas chargé dans Afficher_Bouton_Dialogue()\n");
       return 1;
    }

    if ( dstCoef < 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur : WinInfo Incorrecte dans Afficher_Bouton_Dialogue()\n");
        return 1;
    }
    
    SDL_Texture * text = mat[frameButton];

    // Rectangle Destination ( Renderer )
    SDL_Rect rectDst;
    rectDst.x = ( dstCoef * 16 * 15 ) + xBorder;
    rectDst.y = ( dstCoef * 16 * 10 ) + yBorder; 
    rectDst.h = dstCoef * 16;
    rectDst.w = dstCoef * 16 * 3;

    // Affichage de la frame courante du sprite
    if ( SDL_RenderCopy(renderer, text, NULL, &rectDst) < 0 ) {
        printf("Erreur : SDL_RenderCopy() à échoué dans Afficher_Bouton_Dialogue()\n");
        return 1;
    }

    return 0;
}

static int Afficher_Dialogue(SDL_Texture * textDialogue, SDL_Texture ** matTextButton, int frameButton, liste_texture_pnj_dialogue_t * listeTextPnjDialogue, pnj_t * pnj, liste_type_pnj_t * listeTypePnj, SDL_Rect * view, TTF_Font* font, SDL_Window *window, SDL_Renderer *renderer) {
    // Verification paramètre
    if ( textDialogue == NULL ) {
        printf("Erreur : textDialogue en parametre invalide dans Afficher_Dialogue()\n");
        return 1;
    }

    if ( listeTextPnjDialogue == NULL ) {
        printf("Erreur : listeTextPnjDialogue en parametre invalide dans Afficher_Dialogue()\n");
        return 1;
    }

    if ( pnj == NULL ) {
        printf("Erreur : pnj en parametre invalide dans Afficher_Dialogue()\n");
        return 1;
    }

    if ( listeTypePnj == NULL ) {
        printf("Erreur : listeTypePnj en parametre invalide dans Afficher_Dialogue()\n");
        return 1;
    }

    if ( listeTypePnj->liste == NULL || listeTypePnj->nbElem <= 0 ) {
        printf("Erreur : la liste les listes des types de pnj est mal initialisé dans Afficher_Dialogue()\n");
        return 1;
    }

    if ( view == NULL ) {
        printf("Erreur : view en parametre invalide dans Afficher_Dialogue()\n");
        return 1;
    }

    if ( font == NULL ) {
        printf("Erreur : font en parametre invalide dans Afficher_Dialogue()\n");
        return 1;
    }

    if ( window == NULL ) {
        printf("Erreur : window en parametre invalide dans Afficher_Dialogue()\n");
        return 1;
    }

    if ( renderer == NULL ) {
        printf("Erreur : renderer en parametre invalide dans Afficher_Dialogue()\n");
        return 1;
    }

    if ( frameButton < 0 || frameButton > 1 ) {
        printf("Erreur : frameButton invalide dans Afficher_Dialogue()\n");
        return 1;
    }

    // initialisatio variable
    int win_width,win_height;
    int dstCoef, xBorder, yBorder;
    
    // Récupération des informations de la fenêtre utile à l'affichage
    getWinInfo(window, &win_width, &win_height, 16, view, &dstCoef, &xBorder, &yBorder);

    if ( Afficher_Fond_Boite_Dialogue(textDialogue,dstCoef,xBorder,yBorder,renderer) ) {
        printf("Erreur : Echec Afficher_Fond_Boite_Dialogue() dans Afficher_Dialogue()\n");
        return 1;
    }

    if ( Afficher_Pnj_Dialogue(listeTextPnjDialogue,pnj,dstCoef,xBorder,yBorder,renderer) ) {
        printf("Erreur : Echec Afficher_Fond_Boite_Dialogue() dans Afficher_Dialogue()\n");
        return 1;
    }

    

    if ( Afficher_Bouton_Dialogue(matTextButton,frameButton,renderer,dstCoef,xBorder,yBorder) ) {
        printf("Erreur : Echec Afficher_Bouton_Dialogue() dans Afficher_Dialogue()\n");
        return 1;
    }

    // Affichage des stats du personnage
    SDL_Color marronCLaireInventaire = { 51, 32, 24, 255 };

    int x = ( dstCoef * 16 * 6 ) + xBorder;
    int w = dstCoef * 16 * 11;
    int y = ( dstCoef * 16 * 8.45 ) + yBorder;

    if ( Afficher_Texte_Zone(renderer, font, listeTypePnj->liste[pnj->pnjTypeID]->dialogue, y, x, w, &marronCLaireInventaire) ) {
        printf("Erreur : Echec Afficher_Texte_Zone() dans Afficher_Inventaire()\n");
        return 1;
    }

    return 0;

}

extern int Dialogue(SDL_Texture * textDialogue, liste_texture_pnj_dialogue_t * listeTextPnjDialogue, pnj_t * pnj, liste_type_pnj_t * listeTypePnj, SDL_Rect * view, SDL_Window *window, SDL_Renderer *renderer) {
    /* ------------------ Detection Erreur Parametre ------------------ */

    // Vérification de la variable textDialogue
    if ( textDialogue == NULL ) {
        printf("Erreur : textDialogue non Initialisé dans Dialogue()");
        return 1;
    }

    // Vérification de la variable listeTextPnjDialogue
    if ( listeTextPnjDialogue == NULL) {
        printf("Erreur : listeTextPnjDialogue non initialisée dans Dialogue()");
        return 1;
    }

    // Vérification de la variable pnj
    if ( pnj == NULL ) {
        printf("Erreur : pnj non initialisée dans Dialogue()");
        return 1;
    }

    // Vérification de la variable listeTypePnj
    if ( listeTypePnj == NULL ) {
        printf("Erreur : view non initialisée dans Dialogue()");
        return 1;
    }
    // Verification de la liste de type de pnj
    if ( listeTypePnj->liste == NULL || listeTypePnj->nbElem <= 0 ) {
        printf("Erreur : la liste les listes des types de pnj est mal initialisé dans Dialogue()\n");
        return 1;
    }

    // Vérification de la variable view
    if ( view == NULL) {
        printf("Erreur : view non initialisée dans Dialogue()");
        return 1;
    }

    // Vérification de la variable window
    if ( window == NULL) {
        printf("Erreur : window non initialisée dans Dialogue()");
        return 1;
    }

    // Vérification de la variable renderer
    if ( renderer == NULL) {
        printf("Erreur : renderer non initialisé dans Dialogue()");
        return 1;
    }
    
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
    SDL_timer_t timerFrameButton;

    // Variable frame button
    int frameButton = 0;

    // Variable getWinInfo
    int win_width;
    int win_height;
    int dstCoef;
    int xBorder;
    int yBorder;

    /* ------------------ Initialisation resource jeux ------------------ */

    // initialisation des variables
    TTF_Font* font1 = NULL;
    SDL_Texture * matTextButton[2] = { NULL, NULL};
    char * cheminTextButton[2] = { "asset/hud/dialogue/button.png","asset/hud/dialogue/buttonPressed.png"};

    // Récupération des informations de la fenêtre utile à l'affichage
    getWinInfo(window, &win_width, &win_height, 0, NULL, NULL, NULL, NULL);

    // Gestion font 1280 x 720
    if ( win_width > 1000 && win_width < 1400 ) {
        // Initalisation Font
        font1 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 28);
        if (font1 == NULL) {
            printf("Erreur : Echec TTF_OpenFont(font1) dans Dialogue()");
            return 1;
        }
    }
    // Gestion font 1600 x 900 || 1920 x 1080
    else {
        // Initalisation Font
        font1 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 36);
        if (font1 == NULL) {
            printf("Erreur : Echec TTF_OpenFont(font1) dans Dialogue()");
            return 1;
        }
    }

    // Chargement texture bouton
    for (int i = 0; i < 2; i++) {
        matTextButton[i] = IMG_LoadTexture(renderer,cheminTextButton[i]);
        if ( matTextButton[i] == NULL ) {
            printf("Erreur : Echec IMG_Load(matTextButton[%d]) dans Dialogue()",i);
            return 1;
        }
    }


    // Debut Des Timers De Frame
    Timer_Start(&timerFrameButton);
    timerFrameButton.start -= 700;

    /* ------------------ Boucle Principal ------------------ */

    while( quit == SDL_FALSE && erreur == 0 ) {
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
                                case SDLK_RETURN:
                                    quit = SDL_TRUE;
                                    break;
                            }
                    }
                    break;
            }
        }
        

        /* --------- Gestion Affichage --------- */
    
        // Récupération des informations de la fenêtre utile à l'affichage
        getWinInfo(window, &win_width, &win_height, 16, view, &dstCoef, &xBorder, &yBorder);

        if ( Afficher_Dialogue(textDialogue,matTextButton,frameButton,listeTextPnjDialogue,pnj,listeTypePnj,view,font1,window,renderer) ) {
            printf("Erreur : Echec Afficher_Dialogue() dans Dialogue()\n");
            return 1;
        }

        // Augmentation frame boutton
        if ( (int)Timer_Get_Time( &timerFrameButton ) > 700 ) {
            frameButton = ( frameButton + 1 ) % 2;
            Timer_Start( &timerFrameButton );
        }
        
        // Gestion fps
        if ( ( msPerFrame = (int)Timer_Get_Time( &fps ) ) < (1000 / FRAME_PER_SECONDE) ) {
            SDL_Delay( (1000 / FRAME_PER_SECONDE)  - msPerFrame );
        }

        // mise à jour du renderer ( update affichage)
        SDL_RenderPresent(renderer);
        
    }        

    // Destruction texture button
    for (int i = 0; i < 2; i++) {
        Detruire_Texture( &(matTextButton[i]) );
        if ( matTextButton[i] != NULL ) {
            printf("Erreur : Echec Detruire_Texture(matTextButton[%d]) dans Dialogue()",i);
            return 1;
        }
    }
        
    if ( font1 != NULL ) {
        TTF_CloseFont(font1);
    }

    return erreur;
}

static int Afficher_Fond_Level_UP(SDL_Texture * textFondLvlUP, int  dstCoef, int  xBorder, int  yBorder, SDL_Renderer *renderer) {
    // Verification paramètres
    if (textFondLvlUP == NULL ) {
       printf("Erreur : La texture n'est pas chargé dans Afficher_Fond_Level_UP()\n");
       return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur : Le Renderer n'est pas chargé dans Afficher_Fond_Level_UP()\n");
       return 1;
    }

    if ( dstCoef == 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur : Le WinInfo Incorrecte dans Afficher_Fond_Level_UP()\n");
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
            if ( SDL_RenderCopy(renderer, textFondLvlUP, &srcrect, &dstrect) < 0 ) {
                printf("Erreur : SDL_RenderCopy() à échoué dans Afficher_Fond_Level_UP\n");
                return 1;
            }
            tileNumber++;
        }
    }

    return 0;
}

static int Afficher_Bouton_Level_UP(SDL_Texture ** mat, int selectButton, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder) {
    // Verification paramètres
    if ( mat == NULL ) {
       printf("Erreur : La matrice des texture de boutton en paramètre est invalide dans Afficher_Bouton_Level_UP()\n");
       return 1;
    }

    if ( mat[0] == NULL || mat[1] == NULL ) {
        printf("Erreur : La matrice des texture de boutton est invalide dans Afficher_Bouton_Level_UP()\n");
        return 1;
    }

    if ( selectButton < 0 || selectButton > 2 ) {
        printf("Erreur : selectButton invalide dans Afficher_Bouton_Level_UP()\n");
        return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur : Le Renderer n'est pas chargé dans Afficher_Bouton_Level_UP()\n");
       return 1;
    }

    if ( dstCoef < 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur : WinInfo Incorrecte dans Afficher_Bouton_Level_UP()\n");
        return 1;
    }

    int tabTextButtonID[3] = { 0, 0, 0};

    tabTextButtonID[selectButton]++;

    for (int n = 0; n < 3; n++) {
        SDL_Texture * text = mat[ tabTextButtonID[n] ];

        // Rectangle Destination ( Renderer )
        SDL_Rect rectDst;
        rectDst.x = ( dstCoef * 16 * 12 ) + xBorder;
        rectDst.y = ( dstCoef * 16 * (5 + n) ) + yBorder; 
        rectDst.h = dstCoef * 16;
        rectDst.w = dstCoef * 16 * 1;

        // Affichage des bouton
        if ( SDL_RenderCopy(renderer, text, NULL, &rectDst) < 0 ) {
            printf("Erreur : SDL_RenderCopy() à échoué dans Afficher_Bouton_Level_UP()\n");
            return 1;
        }
    }
    return 0;
}

static int Afficher_Level_UP(SDL_Texture * textFondLevelUP, SDL_Texture ** matTextButton, int selectButton, personnage_t * perso, SDL_Rect * view, TTF_Font ** tabFont, SDL_Window *window, SDL_Renderer *renderer) {
    // Verification paramètre
    if ( textFondLevelUP == NULL ) {
        printf("Erreur : textFondLevelUP en parametre invalide dans Afficher_Lvl_UP()\n");
        return 1;
    }

    if ( perso == NULL ) {
        printf("Erreur : perso en parametre invalide dans Afficher_Lvl_UP()\n");
        return 1;
    }

    if ( view == NULL ) {
        printf("Erreur : view en parametre invalide dans Afficher_Lvl_UP()\n");
        return 1;
    }

    if ( tabFont == NULL ) {
        printf("Erreur : tabFont en parametre invalide dans Afficher_Lvl_UP()\n");
        return 1;
    }

    if ( tabFont[0] == NULL || tabFont[1] == NULL || tabFont[2] == NULL ) {
        printf("Erreur : Tableau de police d'ecriture invalide dans Afficher_Lvl_UP()\n");
        return 1;
    }

    if ( window == NULL ) {
        printf("Erreur : window en parametre invalide dans Afficher_Lvl_UP()\n");
        return 1;
    }

    if ( renderer == NULL ) {
        printf("Erreur : renderer en parametre invalide dans Afficher_Lvl_UP()\n");
        return 1;
    }

    if ( selectButton < 0 || selectButton > 2 ) {
        printf("Erreur : selectButton invalide dans Afficher_Lvl_UP()\n");
        return 1;
    }

    // initialisatio variable
    int win_width,win_height;
    int dstCoef, xBorder, yBorder;
    
    // Récupération des informations de la fenêtre utile à l'affichage
    getWinInfo(window, &win_width, &win_height, 16, view, &dstCoef, &xBorder, &yBorder);

    if ( Afficher_Fond_Level_UP(textFondLevelUP,dstCoef,xBorder,yBorder,renderer) ) {
        printf("Erreur : Echec Afficher_Fond_Level_UP() dans Afficher_Lvl_UP()\n");
        return 1;
    }

    if ( Afficher_Bouton_Level_UP(matTextButton,selectButton,renderer,dstCoef,xBorder,yBorder) ) {
        printf("Erreur : Echec Afficher_Bouton_Level_UP() dans Afficher_Lvl_UP()\n");
        return 1;
    }

    // Affichage du texte LEVEL UP
    int x,y,w;
    
    SDL_Color orange = { 255, 114, 42, 255 };

    w = dstCoef * 16 * 3;
    for (int n = 0; n < 2; n++) {
        char string[150];
        if ( n == 0 ) {
            x = ( dstCoef * 16 * 8.8 ) + xBorder;
            y = ( dstCoef * 16 * 2.2 ) + yBorder;
            strcpy(string,"Level");
        }
        if ( n == 1 ) {
            x = ( dstCoef * 16 * 9.3 ) + xBorder;
            y = ( dstCoef * 16 * ( 3.3) ) + yBorder;
            strcpy(string,"UP");
        }
        
        if ( Afficher_Texte_Zone(renderer, tabFont[n], string, y, x, w, &orange) ) {
            printf("Erreur : Echec Afficher_Texte_Zone('level up logo') dans Afficher_Lvl_UP()\n");
            return 1;
        }
    }

    // Affichage nombre pts_upgrade
    
    SDL_Color marronCLaire = { 51, 32, 24, 255 };

    char string[150];
    w = dstCoef * 16 * 5;
    x = ( dstCoef * 16 * 7.5 ) + xBorder;
    y = ( dstCoef * 16 * ( 4.2) ) + yBorder;
    sprintf(string,"Points Upgrade : %-3d",perso->pts_upgrade);
    if ( Afficher_Texte_Zone(renderer, tabFont[2], string, y, x, w, &marronCLaire) ) {
        printf("Erreur : Echec Afficher_Texte_Zone('pts_upgrade') dans Afficher_Lvl_UP()\n");
        return 1;
    }

    // Affichage des stats du personnage

    char * statLabel[3] = {
        "Points de vie",
        "Attaque",
        "Defense"
    };

    int * statValue[5] = {
        &(perso->caract->maxPv),
        &(perso->caract->atk),
        &(perso->caract->def)
    }; 

    int upgradeValue[3] = {
        UPGRADE_MAX_PV,
        UPGRADE_ATK,
        UPGRADE_DEF
    };

    x = ( dstCoef * 16 * 6.1 ) + xBorder;
    w = dstCoef * 16 * 6;
    for (int n = 0; n < 3; n++) {
        y = ( dstCoef * 16 * ( 5.2 + n) ) + yBorder;

        char string[150];
        sprintf(string,"%-13s: %-4d + %-2d",statLabel[n],*statValue[n],upgradeValue[n]);

        if ( Afficher_Texte_Zone(renderer, tabFont[2], string, y, x, w, &marronCLaire) ) {
            printf("Erreur : Echec Afficher_Texte_Zone('perso stats & upgrade') dans Afficher_Lvl_UP()\n");
            return 1;
        }
    }

    return 0;

}

extern int Level_UP(SDL_Texture * textFondLevelUP, SDL_Texture * background_texture, personnage_t * perso, SDL_Rect * view, SDL_Window *window, SDL_Renderer *renderer) {
    /* ------------------ Detection Erreur Parametre ------------------ */

    // Vérification de la variable textFondLvlUP
    if ( textFondLevelUP == NULL ) {
        printf("Erreur : textFondLvlUP non Initialisé dans Level_UP()\n");
        return 1;
    }

    // Vérification de la variable background_texture
    if ( background_texture == NULL ) {
        printf("Erreur : background_texture non Initialisé dans Level_UP()\n");
        return 1;
    }

    // Vérification de la variable perso
    if ( perso == NULL ) {
        printf("Erreur : perso non initialisée dans Level_UP()\n");
        return 1;
    }


    // Vérification de la variable view
    if ( view == NULL) {
        printf("Erreur : view non initialisée dans Level_UP()\n");
        return 1;
    }

    // Vérification de la variable window
    if ( window == NULL) {
        printf("Erreur : window non initialisée dans Level_UP()\n");
        return 1;
    }

    // Vérification de la variable renderer
    if ( renderer == NULL) {
        printf("Erreur : renderer non initialisé dans Level_UP()\n");
        return 1;
    }
    
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

    // Select button
    int selectButton = 0;

    // Variable getWinInfo
    int win_width;
    int win_height;
    int dstCoef;
    int xBorder;
    int yBorder;

    /* ------------------ Initialisation resource jeux ------------------ */

    // initialisation des variables
    TTF_Font * tabFont[3] = { NULL, NULL, NULL };
    TTF_Font * font1 = NULL;
    TTF_Font * font2 = NULL;
    TTF_Font * font3 = NULL;
    SDL_Texture * matTextButton[2] = { NULL, NULL};
    char * cheminTextButton[2] = { "asset/hud/lvl_up/boutonOFF.png", "asset/hud/lvl_up/boutonON.png"};

    // Récupération des informations de la fenêtre utile à l'affichage
    getWinInfo(window, &win_width, &win_height, 0, NULL, NULL, NULL, NULL);

    // Gestion font 1280 x 720
    if ( win_width > 1000 && win_width < 1400 ) {
        // Initalisation Font Logo Big Texte
        font1  = TTF_OpenFont("asset/font/Minecraft.ttf", 46);
        if (font1  == NULL) {
            printf("Erreur : Echec TTF_OpenFont(font1) dans Level_UP()\n");
            return 1;
        }
        // Initalisation Font Logo Small Texte
        font2 = TTF_OpenFont("asset/font/Minecraft.ttf", 50);
        if (font2  == NULL) {
            printf("Erreur : Echec TTF_OpenFont(font2) dans Level_UP()\n");
            return 1;
        }
        // Initalisation Font Stats Texte
        font3 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 26);
        if (font3  == NULL) {
            printf("Erreur : Echec TTF_OpenFont(font3) dans Level_UP()\n");
            return 1;
        }
    }
    // Gestion font 1600 x 900 || 1920 x 1080
    else {
        // Initalisation Font Logo Big Texte
        font1  = TTF_OpenFont("asset/font/Minecraft.ttf", 64);
        if (font1  == NULL) {
            printf("Erreur : Echec TTF_OpenFont(font1) dans Level_UP()\n");
            return 1;
        }
        // Initalisation Font Logo Small Texte
        font2 = TTF_OpenFont("asset/font/Minecraft.ttf", 68);
        if (font2  == NULL) {
            printf("Erreur : Echec TTF_OpenFont(font2) dans Level_UP()\n");
            return 1;
        }
        // Initalisation Font Stats Texte
        font3 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 32);
        if (font3  == NULL) {
            printf("Erreur : Echec TTF_OpenFont(font3) dans Level_UP()\n");
            return 1;
        }
    }

    tabFont[0] = font1;
    tabFont[1] = font2;
    tabFont[2] = font3;

    // Chargement texture bouton
    for (int i = 0; i < 2; i++) {
        matTextButton[i] = IMG_LoadTexture(renderer,cheminTextButton[i]);
        if ( matTextButton[i] == NULL ) {
            printf("Erreur : Echec IMG_Load(matTextButton[%d]) dans Level_UP()",i);
            return 1;
        }
    }


    // Debut Des Timers

    /* ------------------ Boucle Principal ------------------ */

    while( quit == SDL_FALSE && erreur == 0) {
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
                            case SDLK_UP:
                                selectButton--;
                                break;
                            case SDLK_DOWN:
                                selectButton++;
                                break;
                            case SDLK_RETURN:
                                if ( perso->pts_upgrade > 0 ) {
                                    upgrade_perso(perso,selectButton);
                                    perso->pts_upgrade--;
                                }
                                break;
                            case SDLK_ESCAPE:
                                quit = SDL_TRUE;
                                break;
                            default:
                                break;
                        }
                        if ( selectButton < 0 ) {
                            selectButton = 0;
                        }
                        if ( selectButton > 2 ) {
                            selectButton = 2;
                        }
                    }
                    break;
            }
        }
        

        /* --------- Gestion Affichage --------- */
    
        // Récupération des informations de la fenêtre utile à l'affichage
        getWinInfo(window, &win_width, &win_height, 16, view, &dstCoef, &xBorder, &yBorder);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,background_texture,NULL,NULL);

        if ( Afficher_Level_UP(textFondLevelUP,matTextButton,selectButton,perso,view,tabFont,window,renderer) ) {
            printf("Erreur : Echec Afficher_Level_UP() dans Level_UP()\n");
            return 1;
        }
        
        // Gestion fps
        if ( ( msPerFrame = (int)Timer_Get_Time( &fps ) ) < (1000 / FRAME_PER_SECONDE) ) {
            SDL_Delay( (1000 / FRAME_PER_SECONDE)  - msPerFrame );
        }

        // mise à jour du renderer ( update affichage)
        SDL_RenderPresent(renderer);
        
    }        

    // Destruction texture button
    for (int i = 0; i < 2; i++) {
        Detruire_Texture( &(matTextButton[i]) );
        if ( matTextButton[i] != NULL ) {
            printf("Erreur : Echec Detruire_Texture(matTextButton[%d]) dans Level_UP()\n",i);
            return 1;
        }
    }

    if ( font1 != NULL ) {
        TTF_CloseFont( font1 );
    }

    if ( font2 != NULL ) {
        TTF_CloseFont( font2 );
    }

    if ( font3 != NULL ) {
        TTF_CloseFont( font3 );
    }
    

    return erreur;
}

extern int Mort_Joueur( personnage_t * perso, inventaire_t * inventaire, liste_objet_t *listeObjets ) { 

    if ( perso == NULL ) {
        printf("Erreur : perso en parametre invalide dans Mort_Joueur()\n");
        return 1;
    }

    if ( inventaire == NULL ) {
        printf("Erreur : inventaire en parametre invalide dans Mort_Joueur()\n");
        return 1;
    }

    if ( listeObjets == NULL ) {
        printf("Erreur : listeObjets en parametre invalide dans Mort_Joueur()\n");
        return 1;
    }

    // Drop Aléatoire D'un Equipement
    int rdmDroppedEquipement = rand() % 6;
    int y = rdmDroppedEquipement / 2;
    int x = rdmDroppedEquipement % 2;
    int itemId = inventaire->equipement[y][x];
    if ( itemId != -1 ) {
        listeObjets->tab[itemId]->nb = 0;
        inventaire->equipement[y][x] = -1;
    }
    
    // Reset PV
    perso->caract->pv = perso->caract->maxPv;
    if ( perso->niveau == 1 ) {
        perso->exp = 0;
    }
    else {
        perso->exp = 100 + ( (perso->niveau - 1 ) * 50);
    }


    return 0;
}

extern int Introduction(SDL_Window * window, SDL_Renderer *renderer, SDL_Rect * view ) {
    /* ------------------ Detection Erreur Parametre ------------------ */
    
    // Vérification de la variable window
    if ( window == NULL) {
        printf("Erreur : window non initialisé dans Introduction()");
        return 1;
    }

    // Vérification de la variable renderer
    if ( renderer == NULL) {
        printf("Erreur : renderer non initialisé dans Introduction()");
        return 1;
    }
    
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

    // Variable frame img
    int frame = 0;

    // Variable getWinInfo
    int win_width;
    int win_height;
    int dstCoef;
    int xBorder;
    int yBorder;

    /* ------------------ Initialisation resource jeux ------------------ */

    char * cheminTextImgIntro[4] = { 
        "asset/hud/introduction/Frame1.png",
        "asset/hud/introduction/Frame2.png",
        "asset/hud/introduction/Frame3.png",
        "asset/hud/introduction/Frame4.png"
    };

    SDL_Texture * matTextImgIntro[4];

    // Chargement texture bouton
    for (int i = 0; i < 4; i++) {
        matTextImgIntro[i] = IMG_LoadTexture(renderer,cheminTextImgIntro[i]);
        if ( matTextImgIntro[i] == NULL ) {
            printf("Erreur : Echec IMG_Load(matTextImgIntro[%d]) dans Introduction()",i);
            return 1;
        }
    }


    /* ------------------ Boucle Principal ------------------ */

    while( quit == SDL_FALSE && erreur == 0 ) {
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
                                case SDLK_RETURN:
                                    frame++;
                                    break;
                            }
                    }
                    break;
            }
        }

        if ( frame > 3 ) {
            quit = SDL_TRUE;
            break;
        }
        

        /* --------- Gestion Affichage --------- */
    
        // Récupération des informations de la fenêtre utile à l'affichage
        getWinInfo(window, &win_width, &win_height, 16, view, &dstCoef, &xBorder, &yBorder );

        // Rect Destination ( renderer )
        SDL_Rect dest;
        dest.h = dstCoef * 16 * 11;
        dest.w = dstCoef * 16 * 20;
        dest.x = xBorder;
        dest.y = yBorder;

        // Affichage HUD Skill Bar
        SDL_RenderCopy(renderer, matTextImgIntro[frame], NULL, &dest);
        
        // Gestion fps
        if ( ( msPerFrame = (int)Timer_Get_Time( &fps ) ) < (1000 / FRAME_PER_SECONDE) ) {
            SDL_Delay( (1000 / FRAME_PER_SECONDE)  - msPerFrame );
        }

        // mise à jour du renderer ( update affichage)
        SDL_RenderPresent(renderer);
        
    }        

    // Destruction texture button
    for (int i = 0; i < 4; i++) {
        if ( matTextImgIntro[i] != NULL ) {
            Detruire_Texture( &matTextImgIntro[i] );
        }
    }

    return erreur;
}