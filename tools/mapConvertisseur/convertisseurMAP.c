#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_FILENAME_LEN 50

/**
 * \file convertisseurMAP.c
 * \brief Convertisseur Fichier .tmx En .txt
 * \author Yamis MANFALOTI
 * \version 1.0
 * \date 14 février 2023
 *
 * Convertir Fichier .tmx En .txt:
 * \n Cherche dans le dossier mapTMX les fichiers .tmx
 * \n Récupère les métadonnées de la map
 * \n Récupère les layes de la tilemap
 * \n Ecriture des données dans un fichier .txt au format:
 * \n ************************
 * \n *   Longeur Map        *  
 * \n *   Hauteur Map        *
 * \n *   Taille Des Tiles   *
 * \n *   Nombre De Layer    *
 * \n *                      *
 * \n *      Layer 1         *
 * \n *                      *
 * \n *      Layer 2         *
 * \n *        ...           *
 * \n *************************
 */

int main(void) {
  // initialisation variable
  int fileCount = 0;
  char **fileList = NULL;
  struct dirent *dir;
  
  // ouvertue du répertoire
  DIR *rep = opendir("tools/mapConvertisseur/mapTMX/");
  if (rep == NULL) {
    perror("Erreur d'ouverture du dossier");
    exit(EXIT_FAILURE);
  }

  // compte le nombre de fichier .txt dans le répertoire
  while ((dir = readdir(rep)) != NULL) {
    if (dir->d_name[0] != '.' && dir->d_name[0] != '~') {
      fileCount++;
    }
  }

  // allocation en mémoire de la liste de fichier
  fileList = malloc(sizeof(char *) * fileCount);
  if (fileList == NULL) {
    perror("Erreur d'allocation de mémoire");
    exit(EXIT_FAILURE);
  }

  // réinitialise la position du pointeur du flux rep au début du répertoire
  rewinddir(rep);

  // ajout à a liste les fichier .tmx présent dans le dossier
  int i = 0;
  while ((dir = readdir(rep)) != NULL) {
    if (dir->d_name[0] != '.' && dir->d_name[0] != '~') {
      fileList[i] = malloc(sizeof(char) * MAX_FILENAME_LEN);
      if (fileList[i] == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
      }
      strncpy(fileList[i], dir->d_name, MAX_FILENAME_LEN);
      i++;
    }
  }
  // fermture du repéretoire
  closedir(rep);

  // affichage de la liste des fichier .txm trouvés
  printf("fichier (.tmx) dans le dossier mapTMX:\n");
  for (int n = 0; n < fileCount; n++) {
    printf("%s\n", fileList[n]);
  }

  // affichage decoratif
  printf("\n");
  printf("Fichier en sortie:\n");

  // boucle qui parcourt la liste des fichiers a convertire
  for (int n = 0; n < fileCount; n++) {
    // création chaine de caractère chemin + nom fichier .tmx
    const char *prefix = "tools/mapConvertisseur/mapTMX/";
    char filePath[256];
    strcpy(filePath, prefix);
    strcat(filePath, fileList[n]);
  
    // ouvertue du fichier .tmx en mode lecture
    FILE *tmxFile = fopen(filePath, "r");
    if (!tmxFile) {
      printf("Echec de l'ouverture du fichier TMX '%s'\n", filePath);
      return 1;
    }

    // initialisation des variables
    char buffer[1024];
    int width = 0;
    int height = 0;
    int tilewidth = 0;
    int tileheight = 0;
    int nextlayerid = 1;
    int layerCount = 0;
    int inData = 0;
    int ***matrice;
    int tile;

    // lecture du fichier .tmx
    while (fgets(buffer, sizeof(buffer), tmxFile)) {
      if (strstr(buffer, "<map")) {
        sscanf(buffer, "<map version=\"1.9\" tiledversion=\"1.9.2\" orientation=\"orthogonal\" renderorder=\"right-down\" width=\"%d\" height=\"%d\" tilewidth=\"%d\" tileheight=\"%d\" infinite=\"0\" nextlayerid=\"%d\"", &width, &height, &tilewidth, &tileheight, &nextlayerid);
        // allocation en mémoire de la matrice de map
        matrice = (int***)malloc(sizeof(int**) * (nextlayerid - 1) );
        for (int n = 0; n < nextlayerid - 1; n++) {
          matrice[n] = (int**) malloc( height * sizeof(int*) );
          for ( int i = 0; i < height; i++ ) {
            matrice[n][i] = (int*) malloc( width * sizeof(int) );
          }
        }
      } 
      else if (strstr(buffer, "<layer")) {
        layerCount++;
      } 
      else if (strstr(buffer, "<data encoding=\"csv\">")) {
        inData = 1;
      } 
      else if (strstr(buffer, "</data>")) {
        inData = 0;
      } 
      else if (inData) {
        // chargement des numéros de tile dans la matriceMap depuis le fichier
        for (int i = 0; i < height; i++) {
          for (int j = 0; j < width; j++) {
            fscanf(tmxFile, "%d,", &tile);
            matrice[layerCount-1][i][j] = tile;
          }
        }
      }
    }
    // fermture fichier tmxFile
    fclose(tmxFile);

    // création chaine de caractère chemin + nom fichier output
    const char *prefix2 = "tools/mapConvertisseur/output/";
    const char *suffix = ".txt";
    char output[256];
    strcpy(output, prefix2);
    strcat(output, fileList[n]);
    char *pos = strchr(output, '.');
    if (pos != NULL) {
        *pos = '_';
    }
    strcat(output, suffix);
    printf("%s\n",output);
    
    // Ouvertue du fichier output en mode écriture
    FILE *outputFile = fopen(output, "w");
    if (!outputFile) {
      printf("Echec de l'ouverture du fichier de sortie %s\n",output);
      return 1;
    }

    // ecriture des des données de la map dans le fichier output
    fprintf(outputFile, "%d\n%d\n%d\n%d\n\n", width, height, tilewidth, layerCount);
    for (int n = 0; n < layerCount; n++) {
      for ( int i = 0; i < height; i++ ) {
        for ( int j = 0; j < width; j++ ) {
          fprintf(outputFile, "%d,",matrice[n][i][j]);
        }
        fprintf(outputFile, "\n");
      }
      fprintf(outputFile, "\n");
    }

    // libération de la mémoire de la matrice de map
    for (int n = 0; n < layerCount; n++) {
      for ( int i = 0; i < height; i++ ) {
        free(matrice[n][i]);
        matrice[n][i] = NULL;
      }
    }

    for (int n = 0; n < layerCount; n++) {
      free(matrice[n]);
      matrice[n] = NULL;
    }

    free(matrice);
    matrice = NULL;

    // fermture du fichier
    fclose(outputFile);
  }

  // libreation en mémoire de la liste de fichier
  for (int n = 0; n < fileCount; n++) {
    free(fileList[n]);
  }

  free(fileList);

  // return Exit Succes
  return 0;
}
