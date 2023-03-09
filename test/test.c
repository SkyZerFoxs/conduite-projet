#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

float CalculBouton(float x, float y){
	int i=900;
    float bouton=i/y;
    bouton=x/bouton;
	return bouton;
}

int main(){
    int WINDOW_WIDTH = 1900;
    int WINDOW_HEIGHT = 1080;

    float Calcul= CalculBouton(WINDOW_HEIGHT, -150);
    printf("%f\n", Calcul);
}