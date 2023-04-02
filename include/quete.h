#ifndef QUETE_H
#define QUETE_H

typedef struct quete_s quete_t;

typedef struct quete_s {
    // Information quete
    int id;
    char * objectifQuete;

    // Dialogue du pnj durant la quete
    char * dialogueQuetePnj;

    // Element Liste Quete
    quete_t * avant;
    quete_t * apres;

}quete_t;

extern quete_t * Load_Quete(const char * nom_fichier) ;

#endif