#ifndef MENU_H_
#define MENU_H_

/**
 * \file menu.h
 * \brief Header Gestion Menu
 * \author Cody Theard
 * \version 1.0
 * \date 5 avril 2023
 *
 * Header Gestion du menu:
 * \n Signature des fonction externe de menu.c
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>



extern int play_music(const char* filename);
extern int menu(SDL_Window *window,SDL_Renderer *renderer);
extern int echap(SDL_Window *window, SDL_Renderer *renderer);
extern void changeResolution(int indiceResolution, int indiceFullscreen, SDL_Window *window);
extern void getWinInfo(SDL_Window *window, int * width, int * height, int tileSize, SDL_Rect * view, int * dstCoef, int * xBorder, int * yBorder);
extern void Quit_SDL(SDL_Window** window, SDL_Renderer** renderer);
extern int Init_SDL(SDL_Window** window, SDL_Renderer** renderer, int width, int height);

#endif