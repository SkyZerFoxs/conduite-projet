#include <stdio.h>
#include <stdlib.h>
#include <menu.h>

int main(void) {
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	
	// Initialize SDL
    if (Init_SDL(&window, &renderer, 1600, 900) != 0) {
        fprintf(stderr, "SDL initialization failed\n");
        return 1;
    }
	
	menu(window,renderer);
	// Quit SDL
    Quit_SDL(&window,&renderer);
	exit (0);
}