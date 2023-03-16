#include <stdlib.h>
#include <affichage.h>

/**
 * \file play.c
 * \brief Fonction Principal Du Jeu
 * \author Yamis MANFALOTI
 * \version 2.2
 * \date 12 mars 2023
 *
 * Fonctionnalité implémentée :
 * \n Chargement / Initialisation Des Données Nécessaires ( map, ListeTypeSprite, spriteMap )
 * \n Boucle Principal
 * \n Gestion De La Camera Du Joueur
 * \n Affichage De La TileMap
 * \n Affiche Des Sprite De Monstre
 * \n Affichage Des Sprite Du Personnage
 * \n Gestion Des Frames De Sprite Par Catégorie
 * \n Gestion FPS
 * \n Destruction En Mémoire Des Données Initialisée ( map, ListeTypeSprite, spriteMap )
 */


/**
 * \fn void play(SDL_Window *window, SDL_Renderer *renderer)
 * \fn Fonction Principal Du Jeu
 * 
 * \param window Pointeur sur l'objet SDL_Window
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \return 0 Success || 1 Fail
*/
int play(SDL_Window *window, SDL_Renderer *renderer) {
    /* Initialisation variable */

    // statut des erreurs
    int erreur = 0;

    // initialisation CameraJoueur du joueur
    SDL_Rect CameraJoueur;
    CameraJoueur.x = 21;
    CameraJoueur.y = 34;
    CameraJoueur.w = 20;
    CameraJoueur.h = 11;

    // initialisation des timers
    SDL_timer_t frameTimer1;
    SDL_timer_t frameTimer2;
    SDL_timer_t fps;
    SDL_timer_t lastKey;
    SDL_timer_t AtkCooldown;

    // Variable Pour Quitter La Boucle Principal
    int quit = SDL_FALSE;

    // Nombre De FPS A Afficher
    int FRAME_PER_SECONDE = 60;

    // Nombre De Ms Par Frame Produite
    int msPerFrame;

    // Variable SDL_Event Pour Detecter Les Actions
    SDL_Event event;

    // Variable qui detecte si un touche a deja été préssé
    int keyPressed = 0;

    //int lastAttack = 0;

    // Varaible de direction du personnage
    char direction = 'S';

    /* Initialisation resource jeux */

    // initialisation des variables
    map_t * continent = NULL;
    sprite_type_liste_t *ListeTypeSprite = NULL;
    sprite_t **** spriteMap = NULL;
    SDL_Texture * mapTexture = NULL;
    Sprite_Texture_Liste_t * SpriteTextureListe = NULL;
    sprite_liste_t * listePersoSprite = NULL;

    // initialisation de la map continent
    continent = Initialiser_Map( "asset/map/map.txt");
    if ( continent == NULL ) {
        printf("Erreur : Echec Initialiser_Map() dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // initialisation de la liste de types des sprites
    ListeTypeSprite = Load_Sprite_Type("asset/sprite/spriteType.txt");
    if ( ListeTypeSprite == NULL ) {
        printf("Erreur : Echec Load_Sprite_Type() dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // initialisation de la spriteMap
    spriteMap = Load_SpriteMap(ListeTypeSprite,continent);
    if ( spriteMap == NULL ) {
        printf("Erreur : Echec Load_SpriteMap() dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // initialisation Map Texture
    mapTexture = IMG_LoadTexture(renderer, "asset/tileset.png");
    if ( mapTexture == NULL ) {
        printf("Erreur : Echec IMG_LoadTexture(mapTexture) dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // initialisation Sprite Texture Liste
    SpriteTextureListe = Init_Sprite_Texture_Liste();
    if ( SpriteTextureListe == NULL ) {
        printf("Erreur : Echec Init_Sprite_Texture_Liste() dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // chargmement Sprite Texture Liste
    if ( Load_Sprite_Texture_Liste(SpriteTextureListe,ListeTypeSprite,renderer) ) {
        printf("Erreur : Echec Load_Sprite_Texture_Liste() dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // chargment liste sprite animation personnage
    listePersoSprite = Load_PersoSprite_List(ListeTypeSprite,continent,0,40);
    if ( listePersoSprite == NULL ) {
        printf("Erreur : Echec Load_PersoSprite_List() dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // positionement du personnage
    if ( Deplacement_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,direction)  ) {
        printf("Erreur : Echec Change_Sprite //Deplacement_PersoSprite() dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // Debut Des Timers De Frame Pour Les Sprites
    Timer_Start( &frameTimer1 );
    Timer_Start( &frameTimer2 );
    Timer_Start( &AtkCooldown );
    AtkCooldown.start -= 1000;

    /* Boucle Principal */

    while( quit == SDL_FALSE ) {
        // Lancement timer temps d'execution
        Timer_Start( &fps );

        keyPressed = 0;

        //Detection evenement
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = SDL_TRUE;
                    break;
                case SDL_KEYDOWN:
                    if ( !keyPressed ) {
                        switch (event.key.keysym.sym) {
                            case SDLK_z:
                                if ( Deplacement_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,'z') ) {
                                    printf("Erreur : Echec Deplacement_PersoSprite() dans play()\n");
                                    erreur = 1;
                                    goto detruire;
                                }
                                direction = 'Z';
                                Timer_Start( &lastKey );
                                keyPressed = 1;
                                break;
                            case SDLK_q:        
                                if ( Deplacement_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,'q') ) {
                                    printf("Erreur : Echec Deplacement_PersoSprite() dans play()\n");
                                    erreur = 1;
                                    goto detruire;
                                }
                                direction = 'Q';
                                Timer_Start( &lastKey );
                                keyPressed = 1;
                                break;
                            case SDLK_s:
                                if ( Deplacement_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,'s') ) {
                                    printf("Erreur : Echec Deplacement_PersoSprite() dans play()\n");
                                    erreur = 1;
                                    goto detruire;
                                }
                                direction = 'S';
                                Timer_Start( &lastKey );
                                keyPressed = 1;
                                break;
                            case SDLK_d:
                                if ( Deplacement_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,'d') ) {
                                    printf("Erreur : Echec Deplacement_PersoSprite() dans play()\n");
                                    erreur = 1;
                                    goto detruire;
                                }
                                direction = 'D';
                                Timer_Start( &lastKey );
                                keyPressed = 1;
                                break;
                            case SDLK_e:
                                // Interaction avec les PNG / Objet / Batiment
                                keyPressed = 1;
                                break;
                            default:
                                break;
                        }
                    }
                case SDL_MOUSEBUTTONDOWN:
                    // Vérifie si le bouton gauche de la souris a été cliqué
                    if (event.button.button == SDL_BUTTON_LEFT ) {
                    // Appelle la fonction attack_spritePerso()
                        if ( (int)Timer_Get_Time(&AtkCooldown) > 1000 ) {
                            if ( Attack_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,direction) ) {
                                printf("Erreur : Echec Attack_PersoSprite() dans play()\n");
                                erreur = 1;
                                goto detruire;
                            }
                            Timer_Start( &AtkCooldown );
                        }

                    }
                    break;
                default:
                    break;
            }
        }   

        /*

        // Changement vers animation Idle   Deplacement_PersoSprite(spriteMap,continent,&CameraJoueur,direction)
        if ( (int)Timer_Get_Time( &lastKey ) > 150 ) {
            if ( Deplacement_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,direction)  ) {
                printf("Erreur : Echec Change_Sprite //Deplacement_PersoSprite() dans play()\n");
                erreur = 1;
                goto detruire;
            }
        }
        
        */

        // remise à 0 du renderer ( fond noir )
        if ( SDL_RenderClear(renderer) < 0 ) {
            printf("Erreur : Echec SDL_RenderClear() dans play()\n");
            erreur = 1;
            goto detruire;
        }
        
        // Gestion Frame
        if ( (int)Timer_Get_Time( &frameTimer1 ) > 800 ) {
            AddFrame(spriteMap,0,ListeTypeSprite,continent,&CameraJoueur);
            Timer_Start( &frameTimer1 );
        }

        // Gestion Frame
        if ( (int)Timer_Get_Time( &frameTimer2 ) > 200 ) {
            AddFrame(spriteMap,1,ListeTypeSprite,continent,&CameraJoueur);
            Timer_Start( &frameTimer2 );
        }

        // Affichage Complet
        if ( Affichage_All(mapTexture, continent, SpriteTextureListe, spriteMap, ListeTypeSprite, window, renderer,&CameraJoueur) ) {
            printf("Erreur : Echec Affichage_All() dans play()\n");
            erreur = 1;
            goto detruire;
        }

        // Gestion fps
        if ( ( msPerFrame = (int)Timer_Get_Time( &fps ) ) < (1000 / FRAME_PER_SECONDE) ) {
            //SDL_Delay( (1000 / FRAME_PER_SECONDE)  - msPerFrame );
        }

        // mise à jour du renderer ( update affichage)
        SDL_RenderPresent(renderer);

        

        // Affichage du temps d'execution en Ms
        //printf("%dms\n", (int)Timer_Get_Time( &fps ) );
    }
    
    /* Destruction de la mémoire */
    detruire:

    if ( spriteMap[1] != NULL && spriteMap[1][CameraJoueur.y + 5][CameraJoueur.x + 9]->spriteTypeId < 40 && spriteMap[1][CameraJoueur.y + 5 + 1][CameraJoueur.x + 9]->spriteTypeId < 40 ) {
        spriteMap[1][CameraJoueur.y + 5][CameraJoueur.x + 9] = NULL;
        spriteMap[1][CameraJoueur.y + 5 + 1][CameraJoueur.x + 9] = NULL;
    }

    // destruction en mémoire de la SpriteMap en paramètre
    Detruire_SpriteMap(&spriteMap,continent);

    // destruction liste sprite perso
    Detruire_Sprite_Liste(&listePersoSprite);

    // destruction en mémoire de la Liste de texture des sprites
    Detruire_Sprite_Texture_Liste(&SpriteTextureListe); 

    // destruction en mémoire de la liste des types de sprite en paramètre
    Detruire_Liste_Sprite_Type(&ListeTypeSprite);

    // destruction en mémoire de la texture en paramètre
    Detruire_Texture(mapTexture);

    // destruction en mémoire de la map en paramètre
    Detruire_Map(&continent); 

    return erreur;

}

/**
 * \brief Fonction principale qui init SDL et appelle la fonction Play
 * 
 * \param void Aucun paramètre en entrée 
 * \return Int qui caractérise la réussite de la fonction
 */
int main() {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    int erreur = 0;

    // initalisation de SDL
    if ( Init_SDL(&window,&renderer, 1600, 900) ) {
        printf("Erreur : Init_SDL() à échoué\n");
        Quit_SDL(window,renderer);
        erreur = 1;
        goto quit;
    }
    printf("Init SDL ................ OK\n");

    // fonction principal du jeu (play) 
    printf("Debut Play .............. OK\n");
    if ( play(window,renderer) ) {
        printf("Erreur : play() à échoué\n");
        erreur = 1;
        goto quit;
    }
    printf("Fin Play ................ OK\n");

    
    // Fin de SDL + destruction allocation mémoire
    quit:
    
    Quit_SDL(window,renderer);
    printf("Quit SDL ................ OK\n");

    return erreur;
}