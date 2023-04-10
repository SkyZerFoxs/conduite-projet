#include <stdlib.h>

#include <play.h>
#include <menu.h>


/**
 * \file play.c
 * \brief Fonction Principal Du Jeu
 * \author Yamis MANFALOTI
 * \version 2.5
 * \date 16 mars 2023
 *
   \n Fonctionnalité implémentée :
   \n - Chargement / Initialisation Des Données Nécessaires ( map, ListeTypeSprite, spriteMap )
   \n - Boucle Principal
   \n - Gestion De La Camera Du Joueur
   \n - Affichage De La TileMap
   \n - Affiche Des Sprite De Monstre
   \n - Affichage Des Sprite Du Personnage
   \n - Gestion Des Frames De Sprite Par Catégorie
   \n - Gestion FPS
   \n - Destruction En Mémoire Des Données Initialisée ( map, ListeTypeSprite, spriteMap, ... )
   \n - ...
 */


/**
 * \fn int play(SDL_Window *window, SDL_Renderer *renderer, int charger, SDL_Texture *tabTextGif[9])
 * \brief Fonction Principal Du Jeu
 *
 * \param window Pointeur sur l'objet SDL_Window
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param charger Int qui correspond au methode de chargement ( Nouvelle Partie / Reprendre Partie )
 * \param tabTextGif Tableau des SDL_Texture du fond animé dans les menus
 * \return 0 Success || 1 Fail
 */
extern int play(SDL_Window *window, SDL_Renderer *renderer, int charger, SDL_Texture *tabTextGif[9]) {
    /* ------------------ Initialisation variable ------------------ */

    // statut de la sortie du programme
    int sortie = 0;

    // initialisation CameraJoueur du joueur
    SDL_Rect CameraJoueur;
    CameraJoueur.x = 24;
    CameraJoueur.y = 115;
    CameraJoueur.w = 20;
    CameraJoueur.h = 11;

    // Variable Blocage FPS
    int lockedFPS = 0;

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
    SDL_timer_t CooldownAtkMonstre;
    SDL_timer_t timerRegenVie;
    SDL_timer_t timerDegatHUD;
    SDL_timer_t lastReceveidAtk;

    // Variable qui correspond au sprite detecté
    sprite_t * detectedMonstre = NULL;
    sprite_t * detectedMonstreAtkZone = NULL;

    // Varriable qui correspond a la detection de mosntre
    int detectMonstre;
    int detecZoneAtkMonstre;

    // Variable resultat comabt monstre
    int resultAtkMonstre = 0;

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
    int detectionPnj = 0;
    sprite_t * detectedPnj = NULL;

    //Variable detection coffre
    int detectionCoffre = 0;

    // Variable detection attaque joueur
    int attaqueJoueur = 0;

    // Variable Gestion Animation Degat & Mort Monstre
    int mortMonstre = 0;
    int frameMortMonstre = 0;
    int degatMonstre = 0;
    int frameDegatMonstre = 0;
    int degatJoueur = 0;
    int MortJoueur = 0;

    // Variable Gestion Regen Pv
    int regenPvJoueur = 0;
    int regenPvMonstre = 0;

    // Variable Id Item Loot Aléatoire
    int RandomItemLootID;

    // Variable coordonées detected monstre 
    int yMonstre, xMonstre;

    // Variable getWinInfo
    int win_width;
    int win_height;
    int dstCoef, xBorder, yBorder;

    // Variable Gestion Sortie Inventaire
    int sortieInv = 0;

    // Variable Gestion Sortie Inventaire
    int sortieDiag = 0;
    
    // Variable Gestion Sortie Level UP
    int sortieLevlUP = 0;

    // Variable Gestion Sortie Death
    int sortieDeath = 0;
    

    // Variable gestion Sortie Echap
    int sortieEchap = 0;

    // Variables des temps de cooldowns
    int MsCooldownFrame1 = 600;
    int MsCooldownFrame2 = 200;
    int MsCooldownIdleAnimation = 180;
    int MsAtkCooldown = 1000;
    int MsSpeCooldown = 5000;
    int MsUltCooldown = 15000;
    int MsDeplacementCooldown = 160;
    int MsRespawnMonsterCooldown = 300000;
    int MsCooldownAtkMonstre = MsAtkCooldown + 500;
    int MsRegenVie = 5000;
    int msTimerDegatHUD = 800;
    int MsLastReceveidAtk = 10000;

    // Tableau Skill Unblocked
    int tabUnlockedSkill[3] = { 1, 0, 0};

    // Tableau Locked Skill
    int tabSkill[3] = { 1, 1, 1 };

    // Tableau Respawn Par Zone ( tab[Zone] = {y , x} )
    int tabRespawnJoueur[6][2] = {
        {132, 25},
        {199, 103},
        {131, 113},
        {81 , 53 },
        {38 , 93 },
        {16 , 94 }
    };

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
    SDL_Texture * textDamageHUD = NULL;
    SDL_Texture * textFondDeath = NULL;
    
    // initialisation générateur de nombres aléatoires
    srand(time(NULL));

    // initialisation de la map continent
    continent = Initialiser_Map( "asset/map/map.txt");
    if ( continent == NULL ) {
        printf("Erreur : Echec Initialiser_Map() dans play()\n");
        sortie = 1;
        goto detruire;
    }

    // initialisation de la liste de types des sprites
    ListeTypeSprite = Load_Sprite_Type("asset/sprite/spriteType.txt");
    if ( ListeTypeSprite == NULL ) {
        printf("Erreur : Echec Load_Sprite_Type() dans play()\n");
        sortie = 1;
        goto detruire;
    }

    // initialisation de la spriteMap
    spriteMap = Load_SpriteMap(ListeTypeSprite,continent);
    if ( spriteMap == NULL ) {
        printf("Erreur : Echec Load_SpriteMap() dans play()\n");
        sortie = 1;
        goto detruire;
    }

    // initialisation Map Texture
    mapTexture = IMG_LoadTexture(renderer, "asset/tileset.png");
    if ( mapTexture == NULL ) {
        printf("Erreur : Echec IMG_LoadTexture(mapTexture) dans play()\n");
        sortie = 1;
        goto detruire;
    }

    // initialisation Sprite Texture Liste
    SpriteTextureListe = Init_Sprite_Texture_Liste();
    if ( SpriteTextureListe == NULL ) {
        printf("Erreur : Echec Init_Sprite_Texture_Liste() dans play()\n");
        sortie = 1;
        goto detruire;
    }

    // chargmement Sprite Texture Liste
    if ( Load_Sprite_Texture_Liste(SpriteTextureListe,ListeTypeSprite,renderer) ) {
        printf("Erreur : Echec Load_Sprite_Texture_Liste() dans play()\n");
        sortie = 1;
        goto detruire;
    }

    // chargment liste sprite animation personnage
    listePersoSprite = Load_PersoSprite_List(ListeTypeSprite,continent,0,BORNE_PERSO_SPRITE);
    if ( listePersoSprite == NULL ) {
        printf("Erreur : Echec Load_PersoSprite_List() dans play()\n");
        sortie = 1;
        goto detruire;
    }

    // chargement liste type pnj
    listeTypePnj = Load_Liste_Type_Pnj("asset/sprite/pnjType.txt");
    if ( listeTypePnj == NULL ) {
        printf("Erreur : Echec Load_Liste_Type_Pnj() dans play()\n");
        sortie = 1;
        goto detruire;
    }

    // chargement structure pnj_t
    listePnj = Load_Pnj(continent,spriteMap,listeTypePnj);
    if ( listePnj == NULL ) {
        printf("Erreur : Echec Load_Pnj() dans play()\n");
        sortie = 1;
        goto detruire;
    }

    // chargement structure monstre_t
    listeMonstre = Load_Monster(continent, spriteMap);
    if ( listeMonstre == NULL ) {
        printf("Erreur : Echec Load_Monster() dans play()\n");
        sortie = 1;
        goto detruire;
    }

    // chargement personnage
    perso = creer_personnage("Cody");
    if ( perso == NULL ) {
        printf("Erreur : Echec creer_personnage() dans Play()\n");
        sortie = 1;
        goto detruire;

    }

    // chargement inventaire
    inventaire = Load_Inventaire("asset/hud/inventaire/inventaire.png","asset/objet/objets.png","asset/hud/inventaire/selecteur.png","asset/hud/inventaire/item_info.png",6, 9, 3, 2, renderer) ;
    if ( inventaire == NULL ) {
        printf("Erreur : Echec Load_Inventaire() dans Play()\n");
        sortie = 1;
        goto detruire;
    }

    // chargement liste objet
    listeObjets = load_liste_objets("asset/objet/objet.txt");
    if ( listeObjets == NULL ) {
        printf("Erreur : Echec load_liste_objets() dans play()\n");
        sortie = 1;
        goto detruire;
    }
    
    // chargement texture boite de dialogue
    textHudDialogue = IMG_LoadTexture(renderer, "asset/hud/dialogue/boite_dialogue.png");
    if ( textHudDialogue == NULL ) {
        printf("Erreur : Echec IMG_LoadTexture(textHudDialogue) dans play()\n");
        sortie = 1;
        goto detruire;
    }

    // chargement texture pnj dialogue
    listeTextPnjDialogue = Load_Liste_Texture_Pnj_Dialogue(listeTypePnj,renderer);
    if ( listeTextPnjDialogue == NULL ) {
        printf("Erreur : Echec Load_Liste_Texture_Pnj_Dialogue() dans play()\n");
        sortie = 1;
        goto detruire;
    }

    // Récupération des informations de la fenêtre utile à l'affichage
    getWinInfo(window,&win_width,&win_height,0,NULL,NULL,NULL,NULL);

    // Gestion font 1280 x 720
    if ( win_width > 1000 && win_width < 1400 ) {
        font1 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 22);
        if (font1 == NULL) {
            printf("Erreur : Echec TTF_OpenFont(font1) dans play()\n");
            sortie = 1;
            goto detruire;
        }
    }
    else {
        font1 = TTF_OpenFont("asset/font/RobotoMono-Medium.ttf", 26);
        if (font1 == NULL) {
            printf("Erreur : Echec TTF_OpenFont(font1) dans play()\n");
            sortie = 1;
            goto detruire;
        }
    }
    
    // chargement texture boite Level UP
    textFondLevelUP = IMG_LoadTexture(renderer, "asset/hud/lvl_up/lvl_up.png");
    if ( textFondLevelUP == NULL ) {
        printf("Erreur : Echec IMG_LoadTexture(textFondLevelUP) dans play()\n");
        sortie = 1;
    }

    // chargement texture skill bar
    for (int i = 0; i < 4; i++ ) {
        textSkillBar[i] =  IMG_LoadTexture(renderer,cheminSkillBar[i]);
        if ( textSkillBar[i] == NULL ) {
            printf("Erreur : Echec IMG_LoadTexture(textSkillBar[%d]) dans play()\n",i);
            sortie = 1;
        }
    }

    // chargement texture hud degat
    textDamageHUD = IMG_LoadTexture(renderer, "asset/hud/degat.png");
    if ( textDamageHUD == NULL ) {
        printf("Erreur : Echec IMG_LoadTexture(textDamageHUD) dans play()\n");
        sortie = 1;
    }

    // chargement texture HUD Death
    textFondDeath = IMG_LoadTexture(renderer, "asset/hud/death/death.png");
    if ( textFondDeath == NULL ) {
        printf("Erreur : Echec IMG_LoadTexture(textFondDeath) dans play()\n");
        sortie = 1;
    }

    /* ------------------ Initialisation Boucle Jeu  ------------------ */

    // positionement du personnage
    if ( Deplacement_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,direction)  ) {
        printf("Erreur : Echec Deplacement_PersoSprite() dans play()\n");
        sortie = 1;
        goto detruire;
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
    timerRespawnMonstre.start -= (MsRespawnMonsterCooldown + 1 );
    Timer_Start( &CooldownAtkMonstre );
    CooldownAtkMonstre.start -= (MsCooldownAtkMonstre + 1);
    Timer_Start( &timerRegenVie );
    timerRegenVie.start -= (MsRegenVie + 1);
    Timer_Start( &lastReceveidAtk );
    lastReceveidAtk.start -= (MsLastReceveidAtk + 1 );
    
    

    /* ------------------ Debut Partie  ------------------ */

    // Chargement Partie Precedente
    if ( charger ) {
        Clean_Remanent_Sprite(continent,spriteMap,&CameraJoueur,1);
        if ( load_game(&CameraJoueur.x,&CameraJoueur.y,perso,inventaire,listeObjets,continent,"asset/save/auto-save") ) {
            printf("Warning : Echec de load_game() dans play()\n");
            printf("Warning : Continuité du jeu en mode nouvelle partie\n");
        }
    }
    // Introduction - Nouvelle Partie
    else {
        Introduction(window,renderer,&CameraJoueur);
    }
    


    /* ------------------ Boucle Principal ------------------ */

    while( sortie == 0 ) {
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
                    sortie = -1;
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
                                        sortie = 1;
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
                                        sortie = 1;
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
                                // Detection pnj
                                detectionPnj = Detecter_Pnj(spriteMap,continent,CameraJoueur.y+5+1, CameraJoueur.x+9,direction,1,&detectedPnj);
                                if ( detectionPnj == -1 ) {
                                    printf("Erreur : Echec Detecter_Monstre('basique') dans play()\n");
                                    sortie = 1;
                                    goto detruire;
                                }
                                else if ( detectionPnj == 1 ) {
                                    sortieDiag = Dialogue(textHudDialogue, listeTextPnjDialogue, detectedPnj->pnj, listeTypePnj, &CameraJoueur, window, renderer);
                                    if ( sortieDiag == -1 ) {
                                        sortie = -1;
                                        goto detruire;
                                    }
                                    else if ( sortieDiag == 1 ) {
                                        printf("Erreur : Echec Dialogue() dans play()\n");
                                        sortie = 1;
                                        goto detruire;
                                    }
                                }

                                // Detection coffre
                                detectionCoffre = Detecter_Coffre(continent,CameraJoueur.y+5+1, CameraJoueur.x+9,direction,1);
                                if ( detectionCoffre == -1 ) {
                                printf("Erreur : Echec Detecter_Coffre() dans play()\n");
                                    sortie = 1;
                                    goto detruire;
                                }
                                else if ( detectionCoffre > 0 ) {
                                    
                                    if ( Add_Item_Inventaire(inventaire, listeObjets, loot_coffre(detectionCoffre - 1) ) ) {
                                        printf("Erreur : Echec Add_Item_Inventaire( loot coffre ) dans play()\n");
                                        sortie = 1;
                                        goto detruire;
                                    }
                                    
                                }
                                keyPressed = 1;
                                break;
                            case SDLK_TAB:
                                // Récupération des informations de la fenêtre utile à l'affichage
                                getWinInfo(window, &win_width, &win_height, 0, NULL, NULL, NULL, NULL);

                                // Recuperation background_texture
                                surface = SDL_CreateRGBSurface(0, win_width, win_height, 32, 0, 0, 0, 0);
                                if ( surface == NULL ) {
                                    printf("Erreur : Echec SDL_CreateRGBSurface(Inventaire) dans Play()\n");
                                    sortie = 1;
                                    goto detruire;
                                }
                                if ( SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch) < 0) {
                                    printf("Erreur : Echec SDL_RenderReadPixels(Inventaire) dans Play()\n");
                                    sortie = 1;
                                    goto detruire;
                                }
                                background_texture = SDL_CreateTextureFromSurface(renderer, surface);
                                if ( background_texture == NULL ) {
                                    printf("Erreur : Echec SDL_CreateTextureFromSurface(Inventaire) dans Play()\n");
                                    sortie = 1;
                                    goto detruire;
                                }
                                
                                // Appelle fonction inventaire
                                sortieInv = Inventaire(inventaire, listeObjets, perso, SpriteTextureListe, ListeTypeSprite, listePersoSprite, &CameraJoueur, window, background_texture, renderer);
                                if ( sortieInv == -1 ) {
                                    sortie = -1;
                                    goto detruire;
                                }
                                else if ( sortieInv == 1 ) {
                                    printf("Erreur : Echec inventaire() dans play()\n");
                                    sortie = 1;
                                    goto detruire;
                                }
                                
                                break;
                            case SDLK_ESCAPE:
                                // Récupération des informations de la fenêtre utile à l'affichage
                                getWinInfo(window, &win_width, &win_height, 0, NULL, NULL, NULL, NULL);

                                // Recuperation background_texture
                                surface = SDL_CreateRGBSurface(0, win_width, win_height, 32, 0, 0, 0, 0);
                                if ( surface == NULL ) {
                                    printf("Erreur : Echec SDL_CreateRGBSurface(Level_UP) dans Play()\n");
                                    sortie = 1;
                                    goto detruire;
                                }
                                if ( SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch) < 0) {
                                    printf("Erreur : Echec SDL_RenderReadPixels(Level_UP) dans Play()\n");
                                    sortie = 1;
                                    goto detruire;
                                }
                                background_texture = SDL_CreateTextureFromSurface(renderer, surface);
                                if ( background_texture == NULL ) {
                                    printf("Erreur : Echec SDL_CreateTextureFromSurface(Level_UP) dans Play()\n");
                                    sortie = 1;
                                    goto detruire;
                                }
                                sortieEchap =  echap(window, renderer, background_texture, tabTextGif);
                                if ( sortieEchap == -1 ) {
                                    sortie = -1;
                                }
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
                                sortie = 1;
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
                                sortie = 1;
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

        // Gestion Skill Personnage
        if ( perso->niveau >= 5 ) {
            tabUnlockedSkill[1] = 1;
        }

        if ( perso->niveau >= 15 ) {
            tabUnlockedSkill[2] = 1;
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

        // Regen Pv Personnage & Monstre
        if ( (int)Timer_Get_Time( &lastReceveidAtk ) > MsLastReceveidAtk ) {
            regenPvJoueur = 1;
            regenPvMonstre = 1;
        }

        if ( regenPvJoueur == 1 ) {
            if ( (int)Timer_Get_Time( &timerRegenVie ) > MsRegenVie ) {
                // Calcule Des Stats Du Joueur
                caract_t caractJoueur = { 0, 0, 0, 0 };
                calculer_stats_perso(perso,&caractJoueur);
                // Regen PV
                if ( perso->caract->pv < caractJoueur.maxPv ) {
                    perso->caract->pv += perso->caract->maxPv * 0.1;
                    
                    if ( perso->caract->pv > caractJoueur.maxPv ) {
                        perso->caract->pv = caractJoueur.maxPv;
                    }
                }
                // Reset Timer
                Timer_Start( &timerRegenVie );
            }
        }

        if ( regenPvMonstre ) {
            if ( (int)Timer_Get_Time( &timerRespawnMonstre ) > MsRespawnMonsterCooldown ) {
                Respawn_Monstre( listeMonstre, continent, CameraJoueur.y+5+1, CameraJoueur.x+9);
                Timer_Start( &timerRespawnMonstre );
            }
        }

        // Detection Mort Personnage
        if ( perso->caract->pv <= 0 ) {
            // Reset Pv 
            perso->caract->pv = 0;

            MortJoueur = 1;
            degatJoueur = 0;


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
               detectMonstre = Detecter_Monstre(spriteMap,continent,CameraJoueur.y+5+1, CameraJoueur.x+9,direction,1,&detectedMonstre);
                if ( detectMonstre == -1 ) {
                    printf("Erreur : Echec Detecter_Monstre('basique') dans play()\n");
                    sortie = 1;
                    goto detruire;
                }
                if ( detectMonstre == 1 && detectedMonstre->monstre->caract->pv > 0 ) {
                    attaqueJoueur = 1;
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
               detectMonstre = Detecter_Monstre(spriteMap,continent,CameraJoueur.y+5+1, CameraJoueur.x+9,direction,2,&detectedMonstre);
                if ( detectMonstre == -1 ) {
                    printf("Erreur : Echec Detecter_Monstre('spe') dans play()\n");
                    sortie = 1;
                    goto detruire;
                }
                if ( detectMonstre == 1 && detectedMonstre->monstre->caract->pv > 0 ) {
                    attaqueJoueur = 2;
                }
            }
        }

        // Bloqué les actions pendants l'attaque ultime
        if ( rKeyClicked == 1 ) {
            frameAtck++;
            SDL_Delay(200);
            if ( frameAtck == 6 ) {
                rKeyClicked = 0;
                frameAtck = 0;
                // Detection reussie attaque
               detectMonstre = Detecter_Monstre(spriteMap,continent,CameraJoueur.y+5+1, CameraJoueur.x+9,direction,3,&detectedMonstre);
                if ( detectMonstre == -1 ) {
                    printf("Erreur : Echec Detecter_Monstre('ult') dans play()\n");
                    sortie = 1;
                    goto detruire;
                }
                if ( detectMonstre == 1 && detectedMonstre->monstre->caract->pv > 0 ) {
                    attaqueJoueur = 3;
                }
            }
        }

        // Attaque Du Joueur
        if ( attaqueJoueur > 0 ) {
            combat_joueur(perso, detectedMonstre->monstre, attaqueJoueur);
            if ( detectedMonstre->monstre->caract->pv <= 0 ) {
                mortMonstre = 1;
            }
            else {
                degatMonstre = 1;
            }
            attaqueJoueur = 0;
        }

        // Animation Degat Monstre
        if ( degatMonstre ) {
            if ( frameDegatMonstre == 0 ) {
                yMonstre = detectedMonstre->monstre->pos_y;
                xMonstre = detectedMonstre->monstre->pos_x;
                if ( detectedMonstre->monstre->monstreSize == 4) {
                    spriteMap[0][yMonstre][xMonstre]->spriteTypeId++;
                    spriteMap[0][yMonstre][xMonstre]->frame = 0;

                    spriteMap[0][yMonstre+1][xMonstre]->spriteTypeId++;
                    spriteMap[0][yMonstre+1][xMonstre]->frame = 0;

                    spriteMap[0][yMonstre][xMonstre+1]->spriteTypeId++;
                    spriteMap[0][yMonstre][xMonstre+1]->frame = 0;

                    spriteMap[0][yMonstre+1][xMonstre+1]->spriteTypeId++;
                    spriteMap[0][yMonstre+1][xMonstre+1]->frame = 0;
                }
                else if ( detectedMonstre->monstre->monstreSize == 2 ) {
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
                yMonstre = detectedMonstre->monstre->pos_y;
                xMonstre = detectedMonstre->monstre->pos_x;
                if ( detectedMonstre->monstre->monstreSize == 4 ) {
                    spriteMap[0][yMonstre][xMonstre]->spriteTypeId--;
                    spriteMap[0][yMonstre][xMonstre]->frame = 0;

                    spriteMap[0][yMonstre+1][xMonstre]->spriteTypeId--;
                    spriteMap[0][yMonstre+1][xMonstre]->frame = 0;

                    spriteMap[0][yMonstre][xMonstre+1]->spriteTypeId--;
                    spriteMap[0][yMonstre][xMonstre+1]->frame = 0;

                    spriteMap[0][yMonstre+1][xMonstre+1]->spriteTypeId--;
                    spriteMap[0][yMonstre+1][xMonstre+1]->frame = 0;
                }
                else if ( detectedMonstre->monstre->monstreSize == 2 ) {
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

        // Detection Mort Monstre ( Animation + Reward )
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
                perso->exp += ( detectedMonstre->monstre->niveau ) * 10 +  ( ( detectedMonstre->monstre->niveau - perso->niveau ) % detectedMonstre->monstre->niveau ) * 8 ;
                RandomItemLootID = loot_monstre(detectedMonstre->monstre->niveau);
                if ( RandomItemLootID != -1 ) {
                    if ( Add_Item_Inventaire(inventaire, listeObjets, RandomItemLootID) ) {
                        printf("Erreur : Echec Add_Item_Inventaire( loot monstre ) dans play()\n");
                        sortie = 1;
                        goto detruire;
                    }
                }
                
            }
        }
        
        // Changement vers animation Idle  Deplacement_PersoSprite(spriteMap,continent,&CameraJoueur,direction)
        if ( (int)Timer_Get_Time( &lastKey ) > MsCooldownIdleAnimation  && mouseClicked == 0 && aKeyClicked == 0 && rKeyClicked == 0 ) {
            if ( Deplacement_PersoSprite(spriteMap,continent,listePersoSprite,&CameraJoueur,direction)  ) {
                printf("Erreur : Echec Deplacement_PersoSprite() dans play()\n");
                sortie = 1;
                goto detruire;
            }
        }

        // Detection attaque monstre
        if ( (int)Timer_Get_Time( &CooldownAtkMonstre ) > MsCooldownAtkMonstre ) {
            for (int i = 1; i < 4; i++ ) {
                detecZoneAtkMonstre = Detecter_Zone_Atk_Monstre(spriteMap,continent,CameraJoueur.y+5+1, CameraJoueur.x+9,i,&detectedMonstreAtkZone);
                if ( detecZoneAtkMonstre == -1 ) {
                    printf("Erreur : Echec Detecter_Zone_Atk_Monstre() dans play()\n");
                    sortie = 1;
                    goto detruire;
                }
                if ( detecZoneAtkMonstre == 1 && detectedMonstreAtkZone->monstre->caract->pv > 0 ) {
                    resultAtkMonstre = combat_monstre(detectedMonstreAtkZone->monstre,perso,i);
                    if ( resultAtkMonstre == -1 ) {
                        printf("Erreur : Echec combat_monstre() dans play()\n");
                        sortie = 1;
                        goto detruire;
                    }
                    else if ( resultAtkMonstre == 1 ) {
                        degatJoueur = 1;
                        Timer_Start( &timerDegatHUD );
                        Timer_Start( &lastReceveidAtk );
                        regenPvJoueur = 0;
                    }
                    
                    break; // Sortir de la boucle
                }
            }
            Timer_Start( &CooldownAtkMonstre );
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
            sortie = 1;
            goto detruire;
        }
        
        
        // Affichage Complet
        if ( Affichage_All(perso, tabSkill, textSkillBar, mapTexture, continent, SpriteTextureListe, spriteMap, ListeTypeSprite, window, font1, renderer,&CameraJoueur) ) {
            printf("Erreur : Echec Affichage_All() dans play()\n");
            sortie = 1;
            goto detruire;
        }

        // Affichage HUD Degat
        if ( degatJoueur == 1 && (int)Timer_Get_Time( &timerDegatHUD ) < msTimerDegatHUD ) {
            // Récupération des informations de la fenêtre utile à l'affichage
            getWinInfo(window, &win_width, &win_height, 16, &CameraJoueur, &dstCoef, &xBorder, &yBorder );

            // Rect Destination ( renderer )
            SDL_Rect dest;
            dest.h = dstCoef * 16 * 11;
            dest.w = dstCoef * 16 * 20;
            dest.x = xBorder;
            dest.y = yBorder;

            // Affichage HUD Skill Bar
            SDL_RenderCopy(renderer, textDamageHUD, NULL, &dest);
        }

        // Gestion HUD Degat Joueur
        if ( degatJoueur == 1 && (int)Timer_Get_Time( &timerDegatHUD ) > msTimerDegatHUD ) {
            degatJoueur = 0;
        }

        // Affichage Level UP
        if ( perso->exp >= ( perso->palierExp = 100 * perso->niveau ) ) {
            // Récupération des informations de la fenêtre utile à l'affichage
            getWinInfo(window, &win_width, &win_height, 0, NULL, NULL, NULL, NULL);

            // Recuperation background_texture
            surface = SDL_CreateRGBSurface(0, win_width, win_height, 32, 0, 0, 0, 0);
            if ( surface == NULL ) {
                printf("Erreur : Echec SDL_CreateRGBSurface(Level_UP) dans Play()\n");
                sortie = 1;
                goto detruire;
            }
            if ( SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch) < 0) {
                printf("Erreur : Echec SDL_RenderReadPixels(Level_UP) dans Play()\n");
                sortie = 1;
                goto detruire;
            }
            background_texture = SDL_CreateTextureFromSurface(renderer, surface);
            if ( background_texture == NULL ) {
                printf("Erreur : Echec SDL_CreateTextureFromSurface(Level_UP) dans Play()\n");
                sortie = 1;
                goto detruire;
            }
            
            perso->niveau++;
            perso->pts_upgrade += 3;
            sortieLevlUP = Level_UP(textFondLevelUP,background_texture,perso,&CameraJoueur,window,renderer);
            if ( sortieLevlUP == -1 ) {
                sortie = -1;
            }
            else if ( sortieLevlUP == 1 ) {
                printf("Erreur : Echec Level_UP() dans play()\n");
                sortie = 1;
                goto detruire;
            }
        }

        // Affichage HUD Mort Joueur
        if ( MortJoueur ) {
            // Récupération des informations de la fenêtre utile à l'affichage
            getWinInfo(window, &win_width, &win_height, 0, NULL, NULL, NULL, NULL);
            // Recuperation background_texture
            surface = SDL_CreateRGBSurface(0, win_width, win_height, 32, 0, 0, 0, 0);
            if ( surface == NULL ) {
                printf("Erreur : Echec SDL_CreateRGBSurface(Death) dans Play()\n");
                sortie = 1;
                goto detruire;
            }
            if ( SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch) < 0) {
                printf("Erreur : Echec SDL_RenderReadPixels(Death) dans Play()\n");
                sortie = 1;
                goto detruire;
            }
            background_texture = SDL_CreateTextureFromSurface(renderer, surface);
            if ( background_texture == NULL ) {
                printf("Erreur : Echec SDL_CreateTextureFromSurface(Death) dans Play()\n");
                sortie = 1;
                goto detruire;
            }
            // HUD Death
            sortieDeath = Death(textFondDeath,background_texture,&CameraJoueur,window,renderer);
            if ( sortieDeath == -1 ) {
                sortie = -1;
            }
            else if ( sortieDeath == 1 ) {
                printf("Erreur : Echec Death() dans play()\n");
                sortie = 1;
                goto detruire;
            }

            // Clean Sprite View
            Clean_Remanent_Sprite(continent,spriteMap,&CameraJoueur,1);
            // Respawn Joueur
            if ( Respawn_Joueur(continent,perso,&CameraJoueur,tabRespawnJoueur ) ) {
                printf("Erreur : Echec Respawn_Joueur() dans play()\n");
                sortie = 1;
                goto detruire;
            }
            
            if ( Mort_Joueur(perso,inventaire,listeObjets) ) {
                printf("Erreur : Echec Mort_Joueur() dans play()\n");
                sortie = 1;
                goto detruire;
            }

            MortJoueur = 0;
        }

        // Gestion fps
        if ( ( msPerFrame = (int)Timer_Get_Time( &fps ) ) < (1000 / FRAME_PER_SECONDE) ) {
            //SDL_Delay( (1000 / FRAME_PER_SECONDE)  - msPerFrame );
        }

        // mise à jour du renderer ( update affichage)
        SDL_RenderPresent(renderer);
        

        // Affichage du temps d'execution en Ms
        if ( lockedFPS ) {
            printf("fps : %-3dms\n", (int)Timer_Get_Time( &fps ) );
        }
    }

    // Sauvegarde du jeu
    save_game(CameraJoueur.x,CameraJoueur.y,perso,inventaire,listeObjets,continent);
    

    /* -------  Destruction de la mémoire -------*/
    detruire:

    // Clean variable detected monstre
    detectedMonstre = NULL;
    detectedMonstreAtkZone = NULL;

    // Destruction texture textFondDeath
    if ( textFondDeath != NULL ) {
        Detruire_Texture(&textFondDeath);
    }
    
    // Destruction texture DamageHUD
    if ( textDamageHUD != NULL ) {
        Detruire_Texture(&textDamageHUD);
    }

    // Destruction texture SkillBar
    for (int i = 0; i < 4; i++ ) {
        cheminSkillBar[i] = NULL;
        if ( textSkillBar[i] != NULL ) {
            Detruire_Texture( &(textSkillBar[i]) );
        }
    }
	
    // Destruction texture Boite Level UP
    if ( textFondLevelUP != NULL ) {
        Detruire_Texture(&textFondLevelUP);
    }
    

    // Destruction background_texture
    if ( background_texture != NULL ) {
        Detruire_Texture( &background_texture );
    }

    // Destruction SDL_Surface surface (background_text)
    if ( surface != NULL ) {
        SDL_FreeSurface(surface);
        surface = NULL;
    }

	// Detruire Font
    if ( font1 != NULL ) {
        TTF_CloseFont(font1);
        font1 = NULL;
    }
	
	// Detruite liste texture pnj dialogue
	Detruire_Liste_Texture_Pnj_Dialogue(&listeTextPnjDialogue);

    // Detruire texture Hud Dialogue
    if ( textHudDialogue != NULL ) {
        Detruire_Texture(&textHudDialogue);
    }

    // Detruire liste objets
    detruire_liste_objet(&listeObjets);

    // destruction en mémoire de l'inventaire
    Detruire_Inventaire(&inventaire);

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
    if ( mapTexture != NULL ) {
        Detruire_Texture(&mapTexture);
    }

    // destruction en mémoire de la SpriteMap en paramètre
    Detruire_SpriteMap(&spriteMap,continent);

    // destruction en mémoire de la liste des types de sprite en paramètre
    Detruire_Liste_Sprite_Type(&ListeTypeSprite);

	// destruction en mémoire de la map en paramètre
    Detruire_Map(&continent); 
    

    /* -------  Fin fonction Play() + sortie status du programme -------*/
    
    return sortie;

}