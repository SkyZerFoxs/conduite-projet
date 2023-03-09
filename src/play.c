#include <stdlib.h>
#include <affichage.h>

/**
 * \file play.c
 * \brief Fonction Principal Du Jeu
 * \author Yamis MANFALOTI
 * \version 2.1
 * \date 09 mars 2023
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
 * \return Int qui caractérise la réussite de la fonction
*/
int play(SDL_Window *window, SDL_Renderer *renderer) {
    /* Initialisation */

    // initialisation CameraJoueur du joueur
    SDL_Rect CameraJoueur;
    CameraJoueur.x = 21;
    CameraJoueur.y = 34;
    CameraJoueur.w = 20;
    CameraJoueur.h = 11;

    // initialisation de la map continent
    map_t * continent = Initialiser_Map( "asset/map/map.txt");
    if ( continent == NULL ) {
        printf("Erreur : Echec Initialiser_Map() dans play()\n");
        return 1;
    }

    // initialisation de la liste de types des sprites
    sprite_type_liste_t *ListeTypeSprite = Load_Sprite_Type("asset/sprite/spriteType.txt");
    if ( ListeTypeSprite == NULL ) {
        printf("Erreur : Echec Load_Sprite_Type() dans play()\n");
        return 1;
    }

    // initialisation de la spriteMap
    sprite_t ***spriteMap = Load_SpriteMap(ListeTypeSprite,continent);
    if ( spriteMap == NULL ) {
        printf("Erreur : Echec Load_SpriteMap() dans play()\n");
        return 1;
    }

    // initialisation Map Texture
    SDL_Texture * mapTexture = IMG_LoadTexture(renderer, "asset/tileset.png");
    if ( mapTexture == NULL ) {
        printf("Erreur : Echec IMG_LoadTexture() dans play()\n");
        return 1;
    }

    // initialisation Sprite Texture Liste
    Sprite_Texture_Liste_t * SpriteTextureListe = Init_Sprite_Texture_Liste();
    if ( Load_Sprite_Texture_Liste(SpriteTextureListe,ListeTypeSprite,renderer) ) {
        printf("Erreur : Echec Load_Sprite_Texture_Liste() dans play()\n");
        return 1;
    }

    
    // initialisation des timers
    SDL_timer_t frameTimer1;
    SDL_timer_t frameTimer2;
    SDL_timer_t fps;

    /* variable utile à la boucle principal */

    // Variable Pour Quitter La Boucle Principal
    int quit = SDL_FALSE;

    // Nombre De FPS A Afficher
    int FRAME_PER_SECONDE = 20;

    // Nombre De Ms Par Frame Produite
    int msPerFrame;

    // Variable SDL_Event Pour Detecter Les Actions
    SDL_Event event;

    // Debut Des Timers De Frame Pour Les Sprites
    Timer_Start( &frameTimer1 );
    Timer_Start( &frameTimer2 );
    
    /* Boucle Principal */

    while( quit == SDL_FALSE ) {
        // Lancement timer temps d'execution
        Timer_Start( &fps );

        //Tant qu'il y a un événement
        SDL_PollEvent( &event );
        switch(event.type) {
            // detection SDL_QUIT
			case SDL_QUIT:
				quit = SDL_TRUE;
                break;
			// detection touche clavier
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_z:
						if ( Deplacement_PersoSprite(spriteMap,continent,&CameraJoueur,'z') ) {
                            printf("Erreur : Echec Deplacement_PersoSprite() dans play()");
                            return 1;
                        }
						break;
					case SDLK_q:
                        if ( Deplacement_PersoSprite(spriteMap,continent,&CameraJoueur,'q') ) {
                            printf("Erreur : Echec Deplacement_PersoSprite() dans play()");
                            return 1;
                        }
						break;
					case SDLK_s:
                        if ( Deplacement_PersoSprite(spriteMap,continent,&CameraJoueur,'s') ) {
                            printf("Erreur : Echec Deplacement_PersoSprite() dans play()");
                            return 1;
                        }
						break;
					case SDLK_d:
                        if ( Deplacement_PersoSprite(spriteMap,continent,&CameraJoueur,'d') ) {
                            printf("Erreur : Echec Deplacement_PersoSprite() dans play()");
                            return 1;
                        }
						break;
					default:
						break;
				}
				break;
		}

        // remise à 0 du renderer ( fond noir )
        if ( SDL_RenderClear(renderer) < 0 ) {
            printf("Erreur : Echec SDL_RenderClear() dans play()\n");
            return 1;
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
            return 1;
        }

        // mise à jour du renderer ( update affichage)
        SDL_RenderPresent(renderer);

        // Gestion fps
        if ( ( msPerFrame = (int)Timer_Get_Time( &fps ) ) < (1000 / FRAME_PER_SECONDE) ) {
            //SDL_Delay( (1000 / FRAME_PER_SECONDE)  - msPerFrame );
        }

        // Affichage du temps d'execution en Ms
        printf("%dms\n", (int)Timer_Get_Time( &fps ) );

        
    }

    /* Destruction de la mémoire */

    // destruction en mémoire de la Liste de texture des sprites
    Detruire_Sprite_Texture_Liste(&SpriteTextureListe); 

    // destruction en mémoire de la texture en paramètre
    Detruire_Texture(mapTexture);

    // destruction en mémoire de la SpriteMap en paramètre
    Detruire_SpriteMap(&spriteMap,continent);
    
    // destruction en mémoire de la liste des types de sprite en paramètre
    Detruire_Liste_Sprite_Type(&ListeTypeSprite);

    // destruction en mémoire de la map en paramètre
    Detruire_Map(&continent); 

    return 0;

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

    // initalisation de SDL
    if ( Init_SDL(&window,&renderer, 1600, 900) ) {
        printf("Erreur : Init_SDL() à échoué\n");
        Quit_SDL(window,renderer);
        return 1;
    }

    if ( play(window,renderer) ) {
        printf("Erreur : play() à échoué\n");
        return 1;
    }

    // Fin de SDL + destruction allocation mémoire
    Quit_SDL(window,renderer);

    return 0;
}