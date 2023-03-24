#include <stdlib.h>

#include <affichage.h>
#include <inventaire.h>
#include <data.h>


/**
 * \file play.c
 * \brief Fonction Principal Du Jeu
 * \author Yamis MANFALOTI
 * \version 2.5
 * \date 16 mars 2023
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
    /* ------------------ Initialisation variable ------------------ */

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
    SDL_timer_t DeplacementCooldown;
    SDL_timer_t SpeCooldown;
    SDL_timer_t UltCooldown;

    // Variable qui correspond au sprite detecté
    sprite_t * detectedMonstre;

    // Varriable qui correspond a la detection de mosntre
    int detect;

    // Variable Pour Quitter La Boucle Principal
    int quit = SDL_FALSE;

    // Nombre De FPS A Afficher
    int FRAME_PER_SECONDE = 30;

    // Nombre De Ms Par Frame Produite
    int msPerFrame;

    // Variable SDL_Event Pour Detecter Les Actions
    SDL_Event event;

    // Variable qui detecte si une touche est deja été préssé
    int keyPressed = 0;

    // Variable qui detecte si un clic gauche est déja en cour
    int mouseClicked = 0;

    // Variable qui detecte si une touche 'a' est déja préssé
    int aKeyClicked = 0;

    // Variable qui detecte si une touche 'r' est déja préssé
    int rKeyClicked = 0;
    
    // Variable qui correspond au frame d'animation de l'attaque
    int frameAtck = 0;

    // Varaible de direction du personnage
    char direction = 'S';

    // 
    int mortMonstre = 0;
    int frameMortMonstre = 0;
    int degatMonstre = 0;
    int frameDegatMonstre = 0;

    // 
    int sortieInv = 0;


    // Variables des temps de cooldowns
    int MsAtkCooldown = 1000;
    int MsSpeCooldown = 1000;
    int MsUltCooldown = 1000;
    int MsDeplacementCooldown = 160;

    /* ------------------ Initialisation resource jeux ------------------ */

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
    listePersoSprite = Load_PersoSprite_List(ListeTypeSprite,continent,0,BORNE_PERSO_SPRITE);
    if ( listePersoSprite == NULL ) {
        printf("Erreur : Echec Load_PersoSprite_List() dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // positionement du personnage
    if ( Deplacement_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,direction)  ) {
        printf("Erreur : Echec Deplacement_PersoSprite() dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // chargement structure monstre_t
    monstre_liste_t * listeMonstre = Load_Monster(continent, spriteMap);
    if ( listeMonstre == NULL ) {
        printf("Erreur : Echec Load_Monster() dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // chargement personnage
    personnage_t * perso = creer_personnage("Cody");
    if ( perso == NULL ) {
        printf("Erreur : Echec creer_personnage() dans Play()\n");
        erreur = 1;
        goto detruire;

    }

    // chargement texture inventaire
    SDL_Texture * textInventaire = IMG_LoadTexture(renderer,"asset/hud/inventaire.png");

    // Debut Des Timers De Frame Pour Les Sprites
    Timer_Start( &frameTimer1 );
    Timer_Start( &frameTimer2 );
    Timer_Start( &lastKey );
    lastKey.start -= 151;
    Timer_Start( &DeplacementCooldown );
    DeplacementCooldown.start -= (MsDeplacementCooldown + 1);
    Timer_Start( &AtkCooldown );
    AtkCooldown.start -= (MsAtkCooldown + 1);
    Timer_Start( &SpeCooldown );
    SpeCooldown.start -= (MsSpeCooldown + 1);
    Timer_Start( &UltCooldown );
    UltCooldown.start -= (MsUltCooldown + 1);


    for ( int i = 0; i < listeMonstre->nbElem; i++ ) {
        afficher_monstre(listeMonstre->tabMonstres[i]);
        printf("\n");
    }

    

    afficher_perso(perso);
    
    printf("\n");

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
                    break;
                // Evenement Touche Clavier
                case SDL_KEYDOWN:
                    if (  !mouseClicked && !keyPressed && !aKeyClicked && !rKeyClicked) {
                        // Gestion Touche Clavier
                        switch (event.key.keysym.sym) {
                            case SDLK_z:
                                direction = 'Z';
                                break;
                            case SDLK_q:
                                direction = 'Q';
                                break;
                            case SDLK_s:
                                direction = 'S';
                                break;
                            case SDLK_d: 
                                direction = 'D';
                                break;
                            case SDLK_a:
                                // temps dans le jeux final pour l'atk spéciale surement 15 seconde
                                if ( (int)Timer_Get_Time(&SpeCooldown) > MsSpeCooldown ) {
                                    if ( Special_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,direction) ) {
                                        printf("Erreur : Echec Attack_PersoSprite() dans play()\n");
                                        erreur = 1;
                                        goto detruire;
                                    }
                                    aKeyClicked = 1;
                                    // Gestion cooldown
                                    Timer_Start( &SpeCooldown );
                                    break;  
                                }
                                break;
                            case SDLK_r:
                                // temps dans le jeux final pour l'atk ultime surement 60 seconde
                                if ( (int)Timer_Get_Time(&UltCooldown) > MsUltCooldown ) {
                                    if ( Ultime_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,direction) ) {
                                        printf("Erreur : Echec Ultime_PersoSprite() dans play()\n");
                                        erreur = 1;
                                        goto detruire;
                                    }
                                    rKeyClicked = 1;
                                    // Gestion cooldown
                                    Timer_Start( &UltCooldown );
                                    break;  
                                }
                                break;
                            case SDLK_e:
                                break;
                            case SDLK_TAB:
                                sortieInv = inventaire(textInventaire, &CameraJoueur, window, renderer);
                                if ( sortieInv == -1 ) {
                                    quit = SDL_TRUE;
                                }
                                else if ( sortieInv == 1 ) {
                                    printf("Erreur : Echec inventaire() dans play()\n");
                                    erreur = 1;
                                    goto detruire;
                                }
                                break;
                            default:
                                break;
                        }
                        // Deplacement
                        if (  ( event.key.keysym.sym == SDLK_z || event.key.keysym.sym == SDLK_q ||
                                event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_d    )
                                && (int)Timer_Get_Time(&DeplacementCooldown) > MsDeplacementCooldown ) 
                        {
                            if ( Deplacement_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,tolower(direction)) ) {
                                printf("Erreur : Echec Deplacement_PersoSprite() dans play()\n");
                                erreur = 1;
                                goto detruire;
                            }
                            Timer_Start( &DeplacementCooldown );
                        }
                        // Gestion dernières touches
                        Timer_Start( &lastKey );
                        keyPressed = 1;
                    }
                    break;
                // Evenement Souris
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT ) {
                        // temps dans le jeux final pour l'atk normale surement 1 seconde
                        if ( (int)Timer_Get_Time(&AtkCooldown) > MsAtkCooldown && !aKeyClicked && !rKeyClicked ) {
                            if ( Attack_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,direction) ) {
                                printf("Erreur : Echec Attack_PersoSprite() dans play()\n");
                                erreur = 1;
                                goto detruire;
                            }
                            mouseClicked = 1;
                            // Gestion cooldown
                            Timer_Start( &AtkCooldown );
                            break;  
                        }
                    }
                    break;
                default:
                    break;
                    
            }
        }

        /* --------- Gestion Animation --------- */

        // Bloqué les actions pendants l'attaque de base
        if ( mouseClicked == 1 ) {
            frameAtck++;
            SDL_Delay(200);
            if ( frameAtck == 4 ) {
                mouseClicked = 0;
                frameAtck = 0;
                // Detection reussie attaque
                detect = Detecter_Monstre(spriteMap,continent,CameraJoueur.y+5+1, CameraJoueur.x+9,direction,1,&detectedMonstre);
                if ( detect == -1 ) {
                    printf("Erreur : Echec Detecter_Monstre('basique') dans play()\n");
                    erreur = 1;
                    goto detruire;
                }
                if ( detect == 1 && detectedMonstre->monstre->caract->pv > 0 ) {
                    combat_joueur(perso, detectedMonstre->monstre, 0);
                    printf("\n");
                    if ( detectedMonstre->monstre->caract->pv <= 0 ) {
                        mortMonstre = 1;
                        level_up(perso, detectedMonstre->monstre); 
                        afficher_perso(perso);
                    }
                    else {
                        degatMonstre = 1;
                    }
                }
                if ( detectedMonstre != NULL ) {
                    afficher_monstre(detectedMonstre->monstre);
                    printf("\n");
                }
            }
        }

        // Bloqué les actions pendants l'attaque spéciale
        if ( aKeyClicked == 1 ) {
            frameAtck++;
            SDL_Delay(200);
            if ( frameAtck == 6 ) {
                aKeyClicked = 0;
                frameAtck = 0;
                // Detection reussie attaque
                detect = Detecter_Monstre(spriteMap,continent,CameraJoueur.y+5+1, CameraJoueur.x+9,direction,2,&detectedMonstre);
                if ( detect == -1 ) {
                    printf("Erreur : Echec Detecter_Monstre('spe') dans play()\n");
                    erreur = 1;
                    goto detruire;
                }
                if ( detect == 1 && detectedMonstre->monstre->caract->pv > 0 ) {
                    combat_joueur(perso, detectedMonstre->monstre, 1);
                    printf("\n");
                    if ( detectedMonstre->monstre->caract->pv <= 0 ) {
                        mortMonstre = 1;
                        level_up(perso, detectedMonstre->monstre); 
                        afficher_perso(perso);
                    }
                    else {
                        degatMonstre = 1;
                    }
                }
                if ( detectedMonstre != NULL ) {
                    afficher_monstre(detectedMonstre->monstre);
                    printf("\n");
                }
            }
        }

        // Bloqué les actions pendants l'attaque spéciale
        if ( rKeyClicked == 1 ) {
            frameAtck++;
            SDL_Delay(200);
            if ( frameAtck == 6 ) {
                rKeyClicked = 0;
                frameAtck = 0;
                // Detection reussie attaque
                detect = Detecter_Monstre(spriteMap,continent,CameraJoueur.y+5+1, CameraJoueur.x+9,direction,3,&detectedMonstre);
                if ( detect == -1 ) {
                    printf("Erreur : Echec Detecter_Monstre('ult') dans play()\n");
                    erreur = 1;
                    goto detruire;
                }
                if ( detect == 1 && detectedMonstre->monstre->caract->pv > 0 ) {
                    combat_joueur(perso, detectedMonstre->monstre, 2);
                    printf("\n");
                    if ( detectedMonstre->monstre->caract->pv <= 0 ) {
                        mortMonstre = 1;
                        level_up(perso, detectedMonstre->monstre); 
                        afficher_perso(perso);
                    }
                    else {
                        degatMonstre = 1;
                    }
                }
                if ( detectedMonstre != NULL ) {
                    afficher_monstre(detectedMonstre->monstre);
                    printf("\n");
                }
            }
        }

        if ( degatMonstre ) {
            if ( frameDegatMonstre == 0 ) {
                detectedMonstre->spriteTypeId++;
                detectedMonstre->frame = 0;
            }
            SDL_Delay(200);
            frameDegatMonstre++;
            if ( frameDegatMonstre == 2 ) {
                degatMonstre = 0;
                detectedMonstre->spriteTypeId--;
                frameDegatMonstre = 0;
            }
        }


        if ( mortMonstre ) {
            if ( frameMortMonstre == 0 ) {
                detectedMonstre->spriteTypeId++;
                detectedMonstre->frame = 0;
                detectedMonstre->monstre->caract->pv = 1; 

            }
            SDL_Delay(200);
            frameMortMonstre++;
            if ( frameMortMonstre == 2 ) {
                mortMonstre = 0;
                detectedMonstre->monstre->caract->pv = -1;
                detectedMonstre->spriteTypeId--;
                frameMortMonstre = 0;
            }
        }
        
        // Changement vers animation Idle   Deplacement_PersoSprite(spriteMap,continent,&CameraJoueur,direction)
        if ( (int)Timer_Get_Time( &lastKey ) > 150  && mouseClicked == 0 && aKeyClicked == 0 && rKeyClicked == 0 ) {
            if ( Deplacement_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,direction)  ) {
                printf("Erreur : Echec Change_Sprite //Deplacement_PersoSprite() dans play()\n");
                erreur = 1;
                goto detruire;
            }
        }
    
       /* --------- Gestion Frame Sprite --------- */ 

        // Gestion Frame Monstre
        if ( (int)Timer_Get_Time( &frameTimer1 ) > 600 ) {
            AddFrame(spriteMap,0,ListeTypeSprite,continent,&CameraJoueur);
            Timer_Start( &frameTimer1 );
        }

        // Gestion Frame Perso
        if ( (int)Timer_Get_Time( &frameTimer2 ) > 200 ) {
            AddFrame(spriteMap,1,ListeTypeSprite,continent,&CameraJoueur);
            Timer_Start( &frameTimer2 );
        }

        /* --------- Gestion Affichage --------- */

        // remise à 0 du renderer ( fond noir )
        if ( SDL_RenderClear(renderer) < 0 ) {
            printf("Erreur : Echec SDL_RenderClear() dans play()\n");
            erreur = 1;
            goto detruire;
        }

       

        // Affichage Complet
        if ( Affichage_All(mapTexture, continent, SpriteTextureListe, spriteMap, ListeTypeSprite, window, renderer,&CameraJoueur) ) {
            printf("Erreur : Echec Affichage_All() dans play()\n");
            erreur = 1;
            goto detruire;
        }

        // Gestion fps
        if ( ( msPerFrame = (int)Timer_Get_Time( &fps ) ) < (1000 / FRAME_PER_SECONDE) ) {
            SDL_Delay( (1000 / FRAME_PER_SECONDE)  - msPerFrame );
        }

        // mise à jour du renderer ( update affichage)
        SDL_RenderPresent(renderer);
        //SDL_Delay(200);
        

        // Affichage du temps d'execution en Ms
        //printf("%dms\n", (int)Timer_Get_Time( &fps ) );
    }
    
    /* Destruction de la mémoire */
    detruire:

    // clean old sprite
    for (int j = -4; j < 5; j++) {
        for (int i = -4; i < 5; i++ ) {
            int y = CameraJoueur.y + 5 + i;
            int x = CameraJoueur.x + 9 + j;
            if (y >= continent->height || x >= continent->width) {
                printf("Erreur : En Dehors de la map dans Deplacement_PersoSprite()\n");
                return 1;
            } else if (spriteMap[1][y][x] != NULL) {
                spriteMap[1][y][x] = NULL;
            }
        }
    }

    // destruction en mémoire de la SpriteMap en paramètre
    Detruire_SpriteMap(&spriteMap,continent);

    // destruction liste sprite perso
    Detruire_Sprite_Liste(&listePersoSprite);
    
    // destruction personnage
    supprimer_perso(&perso);

    // destruction en mémoire de la liste de monstre en paramètre
    Detruire_Liste_Monstres(&listeMonstre);

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