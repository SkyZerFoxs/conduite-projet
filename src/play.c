#include <stdlib.h>
#include <affichage.h>

/**
 * \file play.c
 * \brief Fonction Principal Du Jeu
 * \author Yamis MANFALOTI
 * \version 1.0
 * \date 07 mars 2023
 *
 * Fonctionnalité implémentée :
 * \n Chargement / Initialisation Des Données Nécessaires ( map, listeType, spriteMap )
 * \n Boucle Principal
 * \n Gestion De La Camera Du Joueur
 * \n Affichage De La Map
 * \n Affiche Des Sprite De Monstre
 * \n Affichage Des Sprite Du Personnage
 * \n Gestion Des Frames Par Catégorie
 * \n Destruction En Mémoire Des Données Initialisée ( map, listeType, spriteMap )
 */


/**
 * \fn void play(SDL_Window *window, SDL_Renderer *renderer)
 * \fn Fonction Principal Du Jeu
 * 
 * \param window Pointeur sur l'objet SDL_Window
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \return Int qui caractérise la réussite de la fonction
*/
void play(SDL_Window *window, SDL_Renderer *renderer) {
    /* Initialisation */

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

    // initi frame timer
    SDL_timer_t frameTimer1;
    SDL_timer_t frameTimer2;
    SDL_timer_t fps;

    /* variable utile à la boucle principal */

    int FRAME_PER_SECONDE = 20;
    int quit = SDL_FALSE;
    int msPerFrame;
    SDL_Event event;
    Timer_Start( &frameTimer1 );
    Timer_Start( &frameTimer2 );
    
    /* Boucle Principal */

    while( quit == SDL_FALSE ) {
        // Lancement timer temps d'execution
        Timer_Start( &fps );

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
        
        // Gestion Frame
        if ( (int)Timer_Get_Time( &frameTimer1 ) > 800 ) {
            AddFrame(spriteMap,0,listeType,continent,&vue);
            Timer_Start( &frameTimer1 );
        }

        // Gestion Frame
        if ( (int)Timer_Get_Time( &frameTimer2 ) > 200 ) {
            AddFrame(spriteMap,1,listeType,continent,&vue);
            Timer_Start( &frameTimer2 );
        }

        // Affichage
        Affichage_All(mapTexture,continent, spriteMap, listeType, window, renderer,&vue);

        // mise à jour du renderer ( update affichage)
        SDL_RenderPresent(renderer);

        // Gestion fps

        if ( ( msPerFrame = (int)Timer_Get_Time( &fps ) ) < (1000 / FRAME_PER_SECONDE) ) {
            SDL_Delay( (1000 / FRAME_PER_SECONDE)  - msPerFrame );
        }

        printf("%dms\n", (int)Timer_Get_Time( &fps ) );

        
    }

    /* Destruction de la mémoire */

    // // destruction en mémoire de la texture en paramètre
    Detruire_Texture(mapTexture);
    // destruction en mémoire de la SpriteMap en paramètre
    Detruire_SpriteMap(&spriteMap,continent);
    // destruction en mémoire de la liste des types de sprite en paramètre
    Detruire_Liste_Sprite_Type(&listeType);
    // destruction en mémoire de la map en paramètre
    Detruire_Map(continent); 

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
        printf("Erreur : Init_SDL à échoué\n");
        Quit_SDL(window,renderer);
        return 1;
    }

    play(window,renderer);

    // Fin de SDL + destruction allocation mémoire
    Quit_SDL(window,renderer);

    return 0;
}