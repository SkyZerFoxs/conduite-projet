#include <stdlib.h>

#include <affichage.h>
#include <hud.h>
#include <data.h>
#include <save.h>


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
    CameraJoueur.x = 24;
    CameraJoueur.y = 115;
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
    SDL_timer_t timerRespawnMonstre;

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

    // Variable detection Pnj
    sprite_t * detectedPnj = NULL;

    // Variable Gestion Animation Degat & Mort Monstre
    int mortMonstre = 0;
    int frameMortMonstre = 0;
    int degatMonstre = 0;
    int frameDegatMonstre = 0;

    // Variable coordonées detected monstre 
    int yMonstre, xMonstre;

    // Variable getWinInfo
    int win_width;
    int win_height;

    // Variable Gestion Sortie Inventaire
    int sortieInv = 0;

    // Variable Gestion Sortie Inventaire
    int sortieDiag = 0;
    
    // Variable Gestion Sorite Level UP
    int sortieLevlUP = 0;

    // Variables des temps de cooldowns
    int MsCooldownFrame1 = 600;
    int MsCooldownFrame2 = 200;
    int MsCooldownIdleAnimation = 150;
    int MsAtkCooldown = 1000;
    int MsSpeCooldown = 1000;
    int MsUltCooldown = 1000;
    int MsDeplacementCooldown = 160;
    int MsRespawnMonsterCooldown = 60000;

    // Tableau Skill Unblocked
    int tabUnlockedSkill[3] = { 1, 1, 1};

    // Tableau Locked Skill
    int tabSkill[3] = { 1, 1, 1 };

    /* ------------------ Initialisation resource jeux ------------------ */

    // initialisation des variables
    map_t * continent = NULL;
    sprite_type_liste_t *ListeTypeSprite = NULL;
    sprite_t **** spriteMap = NULL;
    SDL_Texture * mapTexture = NULL;
    Sprite_Texture_Liste_t * SpriteTextureListe = NULL;
    sprite_liste_t * listePersoSprite = NULL;
    liste_type_pnj_t * listeTypePnj;
    pnj_liste_t * listePnj = NULL;
    monstre_liste_t * listeMonstre = NULL;
    personnage_t * perso = NULL;
	inventaire_t * inventaire = NULL;
	liste_objet_t * listeObjets = NULL;
	SDL_Texture * textHudDialogue = NULL;
    liste_texture_pnj_dialogue_t * listeTextPnjDialogue = NULL;
    TTF_Font * font1 = NULL;
    SDL_Surface * surface = NULL;
    SDL_Texture * background_texture = NULL;
    SDL_Texture * textFondLevelUP = NULL;
    char * cheminSkillBar[4] = {
        "asset/hud/skillBar/skillBar.png",
        "asset/hud/skillBar/lockedNormale.png",
        "asset/hud/skillBar/lockedSpeciale.png",
        "asset/hud/skillBar/lockedUltime.png"
    };
    SDL_Texture * textSkillBar[4] = { NULL, NULL, NULL, NULL };
    

    // initialisation de la map continent
    continent = Initialiser_Map( "asset/map/map_finale.txt");
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

    // chargement liste type pnj
    listeTypePnj = Load_Liste_Type_Pnj("asset/sprite/pnjType.txt");
    if ( listeTypePnj == NULL ) {
        printf("Erreur : Echec Load_Liste_Type_Pnj() dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // chargement structure pnj_t
    listePnj = Load_Pnj(continent,spriteMap,listeTypePnj);
    if ( listePnj == NULL ) {
        printf("Erreur : Echec Load_Pnj() dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // chargement structure monstre_t
    listeMonstre = Load_Monster(continent, spriteMap);
    if ( listeMonstre == NULL ) {
        printf("Erreur : Echec Load_Monster() dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // chargement personnage
    perso = creer_personnage("Cody");
    if ( perso == NULL ) {
        printf("Erreur : Echec creer_personnage() dans Play()\n");
        erreur = 1;
        goto detruire;

    }

    // chargement inventaire
    inventaire = Load_Inventaire("asset/hud/inventaire/inventaire.png","asset/objet/objets.png","asset/hud/inventaire/selecteur.png","asset/hud/inventaire/item_info.png",6, 9, 3, 2, renderer) ;
    if ( inventaire == NULL ) {
        printf("Erreur : Echec Load_Inventaire() dans Play()\n");
        erreur = 1;
        goto detruire;
    }

    // chargement liste objet
    listeObjets = load_liste_objets("asset/objet/objet.txt");
    if ( listeObjets == NULL ) {
        printf("Erreur : Echec load_liste_objets() dans play()\n");
        erreur = 1;
        goto detruire;
    }
    
    // chargement texture boite de dialogue
    textHudDialogue = IMG_LoadTexture(renderer, "asset/hud/dialogue/boite_dialogue.png");
    if ( textHudDialogue == NULL ) {
        printf("Erreur : Echec IMG_LoadTexture(textHudDialogue) dans play()\n");
        erreur = 1;
        goto detruire;
    }

    // chargement texture pnj dialogue
    listeTextPnjDialogue = Load_Liste_Texture_Pnj_Dialogue(listeTypePnj,renderer);
    if ( listeTextPnjDialogue == NULL ) {
        printf("Erreur : Echec Load_Liste_Texture_Pnj_Dialogue() dans play()\n");
        erreur = 1;
        goto detruire;
    }

    font1 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 26);
    if (font1 == NULL) {
        printf("Erreur : Echec TTF_OpenFont(font26) dans play()\n");
        erreur = 1;
        goto detruire;
    }
    
    // chargement texture boite Level UP
    textFondLevelUP = IMG_LoadTexture(renderer, "asset/hud/lvl_up/lvl_up.png");
    if ( textFondLevelUP == NULL ) {
        printf("Erreur : Echec IMG_LoadTexture(textFondLevelUP) dans play()\n");
        erreur = 1;
    }

    // chargement texture skill bar
    for (int i = 0; i < 4; i++ ) {
        textSkillBar[i] =  IMG_LoadTexture(renderer,cheminSkillBar[i]);
        if ( textSkillBar[i] == NULL ) {
            printf("Erreur : Echec IMG_LoadTexture(textSkillBar[%d]) dans play()\n",i);
            erreur = 1;
        }
    }


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
    Timer_Start( &timerRespawnMonstre );
    timerRespawnMonstre.start -= 5001;
    

    /* ------------------ Zone teste ( A supprimer ) ------------------ */

    /*
    afficher_pnj(spriteMap[0][39][19]->pnj);

    if ( Afficher_Liste_Type_Pnj(listeTypePnj) ) {
        printf("Erreur : Echec Afficher_Liste_Type_Pnj dans play()\n");
        erreur = 1;
        goto detruire;
    }

    for ( int i = 0; i < listePnj->nbElem; i++ ) {
        afficher_pnj(listePnj->tabPnj[i]);
        printf("\n");
    }

    
    for ( int i = 0; i < listeMonstre->nbElem; i++ ) {
        afficher_monstre(listeMonstre->tabMonstres[i]);
        printf("\n");
    }

    afficher_perso(perso);
    printf("\n");

    afficher_liste_objet(listeObjets);
    printf("\n");
    
    */

    // Teste inventaire
    inventaire->inventaire[2][2] = 20;
    listeObjets->tab[20]->nb = 2;
    inventaire->inventaire[3][3] = 5;
    listeObjets->tab[5]->nb = 3;
    inventaire->inventaire[1][1] = 12;
    listeObjets->tab[12]->nb++;
    inventaire->inventaire[0][0] = 24;
    listeObjets->tab[24]->nb = 100;

    /* ------------------ Boucle Principal ------------------ */

    while( quit == SDL_FALSE && !erreur ) {
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
                                if ( tabSkill[1] == 0 ) {
                                    if ( Special_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,direction) ) {
                                        printf("Erreur : Echec Attack_PersoSprite() dans play()\n");
                                        erreur = 1;
                                        goto detruire;
                                    }
                                    aKeyClicked = 1;
                                    // Gestion cooldown
                                    Timer_Start( &SpeCooldown );
                                    tabSkill[1] = 1;
                                    break;  
                                }
                                break;
                            case SDLK_r:
                                // temps dans le jeux final pour l'atk ultime surement 60 seconde
                                if ( tabSkill[2] == 0 ) {
                                    if ( Ultime_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,direction) ) {
                                        printf("Erreur : Echec Ultime_PersoSprite() dans play()\n");
                                        erreur = 1;
                                        goto detruire;
                                    }
                                    rKeyClicked = 1;
                                    // Gestion cooldown
                                    Timer_Start( &UltCooldown );
                                    tabSkill[2] = 1;
                                    break;  
                                }
                                break;
                            case SDLK_e:
                                detect = Detecter_Pnj(spriteMap,continent,CameraJoueur.y+5+1, CameraJoueur.x+9,direction,1,&detectedPnj);
                                if ( detect == -1 ) {
                                    printf("Erreur : Echec Detecter_Monstre('basique') dans play()\n");
                                    erreur = 1;
                                    goto detruire;
                                }
                                if ( detect == 1 ) {
                                    sortieDiag = Dialogue(textHudDialogue, listeTextPnjDialogue, detectedPnj->pnj, listeTypePnj, &CameraJoueur, window, renderer);
                                    if ( sortieDiag == -1 ) {
                                        quit = SDL_TRUE;
                                        goto detruire;
                                    }
                                    else if ( sortieDiag == 1 ) {
                                        printf("Erreur : Echec Dialogue() dans play()\n");
                                        erreur = 1;
                                        goto detruire;
                                    }
                                }
                                break;
                            case SDLK_TAB:
                                // Récupération des informations de la fenêtre utile à l'affichage
                                getWinInfo(window, &win_width, &win_height, 0, NULL, NULL, NULL, NULL);

                                // Recuperation background_texture
                                surface = SDL_CreateRGBSurface(0, win_width, win_height, 32, 0, 0, 0, 0);
                                if ( surface == NULL ) {
                                    printf("Erreur : Echec SDL_CreateRGBSurface(Inventaire) dans Play()\n");
                                    erreur = 1;
                                    goto detruire;
                                }
                                if ( SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch) < 0) {
                                    printf("Erreur : Echec SDL_RenderReadPixels(Inventaire) dans Play()\n");
                                    erreur = 1;
                                    goto detruire;
                                }
                                background_texture = SDL_CreateTextureFromSurface(renderer, surface);
                                if ( background_texture == NULL ) {
                                    printf("Erreur : Echec SDL_CreateTextureFromSurface(Inventaire) dans Play()\n");
                                    erreur = 1;
                                    goto detruire;
                                }

                                // Destruction SDL_Surface surface (background_text)
                                if ( surface != NULL ) {
                                    SDL_FreeSurface(surface);
                                }
                                
                                // Appelle fonction inventaire
                                sortieInv = Inventaire(inventaire, listeObjets, perso, SpriteTextureListe, ListeTypeSprite, listePersoSprite, &CameraJoueur, window, background_texture, renderer);
                                if ( sortieInv == -1 ) {
                                    quit = SDL_TRUE;
                                    goto detruire;
                                }
                                else if ( sortieInv == 1 ) {
                                    printf("Erreur : Echec inventaire() dans play()\n");
                                    erreur = 1;
                                    goto detruire;
                                }

                                // Destruction texture background_texture
                                if ( background_texture != NULL ) {
                                    SDL_DestroyTexture(background_texture);
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
                        if ( tabSkill[0] == 0 && !aKeyClicked && !rKeyClicked ) {
                            if ( Attack_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,direction) ) {
                                printf("Erreur : Echec Attack_PersoSprite() dans play()\n");
                                erreur = 1;
                                goto detruire;
                            }
                            mouseClicked = 1;
                            // Gestion cooldown
                            Timer_Start( &AtkCooldown );
                            tabSkill[0] = 1;
                            break;  
                        }
                    }
                    break;
                default:
                    break;
                    
            }
        }

        /* --------- Gestion Fonctionnalité Jeu  --------- */

        if ( (int)Timer_Get_Time( &timerRespawnMonstre ) > MsRespawnMonsterCooldown ) {
            Respawn_Monstre( listeMonstre, continent, CameraJoueur.y+5+1, CameraJoueur.x+9);
            Timer_Start( &timerRespawnMonstre );
        }

        if ( tabUnlockedSkill[0] && (int)Timer_Get_Time(&AtkCooldown) > MsAtkCooldown ) {
            tabSkill[0] = 0;
        }

        if ( tabUnlockedSkill[1] && (int)Timer_Get_Time(&SpeCooldown) > MsSpeCooldown ) {
            tabSkill[1] = 0;
        }

        if ( tabUnlockedSkill[2] &&  (int)Timer_Get_Time(&UltCooldown) > MsUltCooldown ) {
            tabSkill[2] = 0;
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
                if ( detectedMonstre->monstre->monstreSize > 1 ) {
                    yMonstre = detectedMonstre->monstre->pos_y;
                    xMonstre = detectedMonstre->monstre->pos_x;
                    
                    spriteMap[0][yMonstre][xMonstre]->spriteTypeId++;
                    spriteMap[0][yMonstre][xMonstre]->frame = 0;

                    spriteMap[0][yMonstre+1][xMonstre]->spriteTypeId++;
                    spriteMap[0][yMonstre+1][xMonstre]->frame = 0;
                }
                else {
                    detectedMonstre->spriteTypeId++;
                    detectedMonstre->frame = 0;
                }
            }
            SDL_Delay(200);
            frameDegatMonstre++;
            if ( frameDegatMonstre == 2 ) {
                degatMonstre = 0;
                if ( detectedMonstre->monstre->monstreSize > 1 ) {
                    yMonstre = detectedMonstre->monstre->pos_y;
                    xMonstre = detectedMonstre->monstre->pos_x;

                    spriteMap[0][yMonstre][xMonstre]->spriteTypeId--;
                    spriteMap[0][yMonstre][xMonstre]->frame = 0;

                    spriteMap[0][yMonstre+1][xMonstre]->spriteTypeId--;
                    spriteMap[0][yMonstre+1][xMonstre]->frame = 0;
                }
                else {
                    detectedMonstre->spriteTypeId--;
                    detectedMonstre->frame = 0;
                }
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
                perso->exp += 250;
            }
        }
        
        // Changement vers animation Idle  Deplacement_PersoSprite(spriteMap,continent,&CameraJoueur,direction)
        if ( (int)Timer_Get_Time( &lastKey ) > MsCooldownIdleAnimation  && mouseClicked == 0 && aKeyClicked == 0 && rKeyClicked == 0 ) {
            if ( Deplacement_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,direction)  ) {
                printf("Erreur : Echec Change_Sprite //Deplacement_PersoSprite() dans play()\n");
                erreur = 1;
                goto detruire;
            }
        }
    
       /* --------- Gestion Frame Sprite --------- */ 

        // Gestion Frame Monstre
        if ( (int)Timer_Get_Time( &frameTimer1 ) > MsCooldownFrame1 ) {
            AddFrame(spriteMap,0,ListeTypeSprite,continent,&CameraJoueur);
            Timer_Start( &frameTimer1 );
        }

        // Gestion Frame Perso
        if ( (int)Timer_Get_Time( &frameTimer2 ) > MsCooldownFrame2 ) {
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
        if ( Affichage_All(perso, tabSkill, textSkillBar, mapTexture, continent, SpriteTextureListe, spriteMap, ListeTypeSprite, window, font1, renderer,&CameraJoueur) ) {
            printf("Erreur : Echec Affichage_All() dans play()\n");
            erreur = 1;
            goto detruire;
        }

        // Affichage Level UP
        if ( perso->exp > ( perso->palierExp = perso->niveau * 249 ) ) {
             // Récupération des informations de la fenêtre utile à l'affichage
            getWinInfo(window, &win_width, &win_height, 0, NULL, NULL, NULL, NULL);

            // Recuperation background_texture
            surface = SDL_CreateRGBSurface(0, win_width, win_height, 32, 0, 0, 0, 0);
            if ( surface == NULL ) {
                printf("Erreur : Echec SDL_CreateRGBSurface(Level_UP) dans Play()\n");
                erreur = 1;
                goto detruire;
            }
            if ( SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch) < 0) {
                printf("Erreur : Echec SDL_RenderReadPixels(Level_UP) dans Play()\n");
                erreur = 1;
                goto detruire;
            }
            background_texture = SDL_CreateTextureFromSurface(renderer, surface);
            if ( background_texture == NULL ) {
                printf("Erreur : Echec SDL_CreateTextureFromSurface(Level_UP) dans Play()\n");
                erreur = 1;
                goto detruire;
            }

            // Destruction SDL_Surface surface (background_text)
            if ( surface != NULL ) {
                SDL_FreeSurface(surface);
            }
            
            perso->niveau++;
            perso->pts_upgrade += 2;
            sortieLevlUP = Level_UP(textFondLevelUP,background_texture,perso,&CameraJoueur,window,renderer);
            if ( sortieLevlUP == -1 ) {
                quit = SDL_TRUE;
            }
            else if ( sortieLevlUP == 1 ) {
                printf("Erreur : Echec sortieLevlUP() dans play()\n");
                erreur = 1;
                goto detruire;
            }

            // Destruction texture background_texture
            if ( background_texture != NULL ) {
                SDL_DestroyTexture(background_texture);
            }
            
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

    // Sauvegarde du jeu
    save_game(CameraJoueur.x+9,CameraJoueur.y+5+1,perso,inventaire,listeObjets,0);
    
    /* -------  Destruction de la mémoire -------*/
    detruire:

    for (int i = 0; i < 4; i++ ) {
        if ( textSkillBar[i] != NULL ) {
            Detruire_Texture( &(textSkillBar[i]) );
        }
    }

    // clean old sprite
    for (int j = -4; j < 5; j++) {
        for (int i = -4; i < 5; i++ ) {
            int y = CameraJoueur.y + 5 + i;
            int x = CameraJoueur.x + 9 + j;
            if (y < 0 || x < 0 || y >= continent->height || x >= continent->width) {
                printf("Erreur : Hors map , netoyage sprites personnage (detruire) dans Play()\n");
                return 1;
            } else if (spriteMap[1][y][x] != NULL) {
                spriteMap[1][y][x] = NULL;
            }
        }
    }
	
    // Destruction texture Boite Level UP
    Detruire_Texture(&textFondLevelUP);

    // Destruction background_texture
    if ( background_texture != NULL ) {
        SDL_DestroyTexture(background_texture);
    }

    // Destruction SDL_Surface surface (background_text)
    if ( background_texture != NULL ) {
        SDL_FreeSurface(surface);
    }

	// Detruire Font
	TTF_CloseFont(font1);

    // Detruire liste objets
    detruire_liste_objet(&listeObjets);
	
	// Detruite liste texture pnj dialogue
	Detruire_Liste_Texture_Pnj_Dialogue(&listeTextPnjDialogue);

	// destruction personnage
    supprimer_perso(&perso);
	
	// destruction en mémoire de la liste de monstre en paramètre
    Detruire_Liste_Monstres(&listeMonstre);

    // destruction en mémoire de la liste de pnj en paramètre
    Detruire_Liste_Pnj(&listePnj);
	
	// destruction en mémoire de la liste de type de pnj en paramètre
    Detruire_Liste_Type_Pnj(&listeTypePnj) ;
	
	// destruction liste sprite perso
    Detruire_Sprite_Liste(&listePersoSprite);

	// destruction en mémoire de la Liste de texture des sprites
    Detruire_Sprite_Texture_Liste(&SpriteTextureListe); 

	// destruction en mémoire de la texture en paramètre
    Detruire_Texture(&mapTexture);

    // destruction en mémoire de la SpriteMap en paramètre
    Detruire_SpriteMap(&spriteMap,continent);

    // destruction en mémoire de la liste des types de sprite en paramètre
    Detruire_Liste_Sprite_Type(&ListeTypeSprite);

	// destruction en mémoire de la map en paramètre
    Detruire_Map(&continent); 
    
    // destruction en mémoire de l'inventaire
    Detruire_Inventaire(&inventaire);
    

    /* -------  Fin fonction Play() + sortie status d'erreur -------*/

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