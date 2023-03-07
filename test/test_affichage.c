#include <stdlib.h>
#include <affichage.h>


/**
 * \file test_affichage.c
 * \brief Test des fonctionnalité d'affichage
 * \author Yamis MANFALOTI
 * \version 4.0
 * \date 04 mars 2023
 *
 * Test des fonctionnalités d'affichage: 
 * \n Obtention des informations de la fenetres
 * \n Chargement et Affichage d'une image
 * \n Affichage d'une map selon la vue du joueur
 * \n Affichage de l'animation d'un sprite
 */

/**
 * \brief Fonction principale du test
 * 
 * \param void Aucun paramètre en entrée 
 * \return Int qui caractérise la réussite de la fonction
 */
int main(void) {
	// initalisation variable et pointeur
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;
    
	// initalisation de SDL
    if ( Init_SDL(&window,&renderer, 1600, 900) ) {
        Quit_SDL(window,renderer);
        return 1;
    }

    int fullscreen = 0;
    if ( fullscreen ) {
        SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
    }

    // initialisation de la map continent
    map_t * continent = Initialiser_Map( "asset/map/map.txt");
    if ( continent == NULL ) {
        printf("Erreur: Initialiser_Map() à échoué\n");
    }

    // initialisation vue du joueur
    SDL_Rect vue;
    vue.x = 0;
    vue.y = 0;
    vue.w = 20;
    vue.h = 11;

    // initialisation de la liste de types des sprites
    sprite_type_liste_t *listeType = Load_Sprite_Type("asset/sprite/spriteType.txt");
    if ( listeType == NULL ) {
        printf("Erreur: Load_Sprite_Type() à échoué\n");
    }

    // initialisation de la spriteMap
    sprite_t ***spriteMap = Load_SpriteMap(listeType,continent);
    if ( spriteMap == NULL ) {
        printf("Erreur: Load_SpriteMap() à échoué\n");
    }

    //init map Texture
    SDL_Texture * mapTexture;
    mapTexture = IMG_LoadTexture(renderer, "asset/tileset.png");
    if ( mapTexture == NULL ) {
        printf("Erreur: IMG_LoadTexture() à échoué\n");
    }

    /* variable utile à la boucle principal */
    int FRAME_PER_SECONDE = 25;
    // Affichage de la map seule
    int testAffichageMap = 0;
    // Deplacement de la caméra
    int testMouvementCamera = 0;
    int down = 1;
    // Affichage de d'un sprite seule
    int testAffichageSprite = 0;
    sprite_t * slime = NULL;
    if ( testAffichageSprite) {
        // initialisation du sprite slime
        slime = Load_Sprite(0,0,0,0,listeType,continent);
    }
    // Affichage complet
    int testAffichageAll = 1;
    if ( testAffichageAll ) {
        testAffichageMap = 0;
        testAffichageSprite = 0;
    }
    

    // boucle qui s'arrete a la fermeture de la fenetre 
    int quit = SDL_FALSE;
    while( quit == SDL_FALSE ) {

        //Tant qu'il y a un événement
        while( SDL_PollEvent( &event ) != 0 ) {
            //L'utilisateur demande la fermeture de la fenètres
            if( event.type == SDL_QUIT )
            {
                quit = SDL_TRUE;
                break;
            }
        }   

        // remise à 0 du renderer ( fond noir )
        SDL_RenderClear(renderer);

        
        // affichage de la map
        if ( testAffichageMap ) {
            int win_width,win_height;
            int dstCoef, xBorder, yBorder;
            // Récupération des informations de la fenêtre utile à l'affichage
            getWinInfo(window, &win_width, &win_height, continent->tileSize, &vue, &dstCoef, &xBorder, &yBorder);
            Afficher_Map(mapTexture,continent,&vue, renderer, dstCoef, xBorder, yBorder);
        }

        // test affiche sprite
        if ( testAffichageSprite ) {
            int win_width,win_height;
            int dstCoef, xBorder, yBorder;
            
            // Récupération des informations de la fenêtre utile à l'affichage
            int size = listeType->typeListe[slime->spriteTypeId]->spriteSize;
            getWinInfo(window, &win_width, &win_height, size, &vue, &dstCoef, &xBorder, &yBorder);

            // gestion frame
            slime->frame += 1;

            // affichage du sprite
            Afficher_Sprite(slime, listeType, renderer, &vue, dstCoef, xBorder, yBorder);

            // gestion frame
            SDL_Delay(200);
            
        }
        

        if ( testAffichageAll ) {
            AddFrame(spriteMap,0,listeType,continent,&vue);
            AddFrame(spriteMap,1,listeType,continent,&vue);
            Affichage_All(mapTexture,continent, spriteMap, listeType, window, renderer,&vue);
            SDL_Delay(300);
        }

        // test du mouvement de la caméra
        if ( testMouvementCamera ) {
            // scrolling de haut en bas
            if ( vue.y <= 15 && down) {
                vue.y = vue.y + 1;
                if ( vue.y == 15) {
                    down = 0;
                }
            }
            else if ( vue.y >= 0 && !down ) {
                vue.y = vue.y - 1;
                if ( vue.y == 0) {
                    down = 1;
                }
            }
        }

        // mise à jour du renderer ( update affichage)
        SDL_RenderPresent(renderer);

        // gestion fps
        SDL_Delay( (1000 / FRAME_PER_SECONDE ));

    }

    Detruire_Texture(mapTexture);

    Detruire_SpriteMap(&spriteMap,continent);

    Detruire_Liste_Sprite_Type(&listeType);

    // destruction en mémoire du sprite en paramètre
    Detruire_Sprite(&slime);
    
    // destruction en mémoire de la map en paramètre
    Detruire_Map(continent);

    // Fin de SDL + destruction allocation mémoire
    Quit_SDL(window,renderer);

    return 0;
}

