////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                     Membres
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


RACCOUARD Nathan
THEARD Cody
MANFALOTI Yamis
SARDON ARRAZ William


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                Présentation du projet
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Présentation projet :
Nous sommes des étudiants en 2ème année de licence informatique à l'université du Mans. Dans ce projet, nous devons par groupe créer un jeu vidéo en une durée de 4 mois, du 16 janvier au 14 avril. Pour cela, nous devons utiliser le langage C ainsi que la librairie SDL. Nous avons réfléchis entre la réalisation d'un jeu puissance 4 ou rogue-like (comme présenté durant le cours d'introduction) et la création de notre propre jeu avec ses règles et ses graphismes. Après réflexion, nous avons décidé de créer notre propre RPG/jeu d'aventure.

Présentation jeux :
Notre jeu est un RPG/jeu d'aventure nommé Hero's Quest : "Explorez un monde sombre et terrifiant, envahi par des démons assoiffés de sang. En tant que héros invoqué par la famille royale, vous avez la lourde tâche de sauver le royaume humain de l'extinction. Parcourez des terres dangereuses, affrontez des monstres terrifiants et complétez des quêtes pour gagner en expérience et améliorer vos compétences. Débloquez de nouvelles attaques dévastatrices, utilisez des potions pour restaurer votre santé et équipez-vous d'armures et d'armes légendaires pour vous protéger contre les ennemis les plus redoutables. Parviendrez-vous à relever le défi et vaincre le roi démon pour sauver le monde ?"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                Fonctionnalités du jeu
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Le personnage :
Il y a un personnage principal qui peut être contrôler. Ce personnage à différentes caractéristiques : Un nom, un niveau, des points d'expérience, des statistiques (points de vie, points de vie maximum, attaque, défense), des équipements et des attaques

La carte :
Le personnage se déplace sur une carte où il y a des décors, des maisons, des PNJ etc.

Les PNJ (Personnages Non Joueur):
Le personnage pourra interagir avec les différents PNJ disponibles sur la carte. Chaque PNJ auront des caractéristiques : un id, un nom, un type et un dialogue standard.

Les monstres ennemis :
Les monstres ennemis sont disponibles sur la carte. Ils ont quatre caractéristiques : Un nom, un niveau, des statistiques (points de vie, points de vie maximum, attaque, défense), une position sur la map (coordonnés x et y) et une taille.

L'inventaire :
Le joueur a un inventaire dans lequel il peut gérer ses objets. Il y a soixante places dans l'inventaire plus six places d'équipement. L'équipement sont les objets équipés sur le personnage qui lui donne des statistiques en plus.

Les objets : 
Il y a sept types d'objets : casque, plastron, jambières, bottes, épée, accessoire (bouclier et amulette) ainsi que des potions. Chaque objet a trois niveaux, sachant que plus le niveau est élevé, plus les statistiques d'équipement sont élevées. Chaque objet donne en plus au personnage des statistiques prédéfinies et détient un attribut de quantité, qui correspond au nombre de cet objet dans l'inventaire.

Les récompenses / butins :
Après avoir vaincu un monstre, le joueur obtient en récompense des objets. Le type d'objet donné et le niveau de celui-ci sont aléatoires, mais les chances d'obtenir chaque objet sont différentes selon le niveau du monstre vaincu.

Les interfaces utilisateurs :
On retrouve plusieurs interfaces utilisateurs dans le jeu, comme celle de l'inventaire, celle de la progression de niveau, ou encore celle du dialogue avec un Personnage Non Joueur. Chaque interface est différente et dispose de ses fonctionnalités distinctes.

Le menu :
Le jeu est accompagné d'un menu permettant de lancer la partie, réglez les options d'affichage ou encore s'informer des commandes / touches utilisées par le jeu.

La sauvegarde :
À chaque fin de partie, que le joueur quitte le jeu ou retourne au menu, l'avancement dans le jeu sera garder en mémoire dans un fichier de sauvegarde. Dans le menu, le joueur a la possibilité de choisir entre démarrer une nouvelle partie, ou reprendre la dernière partie qui a été sauvegardé.


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                               Instructions d'installation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Prérequis :
Un système d'exploitation sous Linux ( de préférence Ubuntu ).
Un périphérique d'affichage vidéo.
Le langage C est installé sur la machine.
La bibliothèque standard du C est installé sur la machine.
Un compilateur C++ avec le support de C++11, tel que G++. ( utile pour l'installation manuelle de SDL )


Mode d'emploi :
- Installer SDL :
    - gestionnaire de paquets APT (Advanced Package Tool) :
        - sudo apt install libsdl2-dev
        - sudo apt install libsdl2-image-dev
        - sudo apt install libsdl2-ttf-dev

    OU

    - installation manuelle :
        - Installer SDL:
            ( il est conseillé de choisir ces versions pour éviter tout problème )
            - SDL2_image-2.6.3
            - SDL2_ttf-2.20.1
            - SDL2-2.26.4
        - Changer le LDFLAGS ( ligne 4 ) dans le Makefile:
            APT (original)  :  $(shell sdl2-config --cflags --libs)
            Manuelle        :  $(shell Chemin_Installation_SDL/bin/sdl2-config --cflags --libs)

- Compilation :
    - make all

- Lancement
    - ./bin/main


Avertissement : Si des erreurs sont rencontrée lors de la compilation ou l'exécution du programme, il est fort probable que cela soit dû à une mauvaise installation de la librairie SDL. Dans ce cas, n'hésitez pas à nous joindre afin de régler ce problème.


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                   Credits
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


=> Tile Set ( Modified )
Credits to magiscarf. ( https://www.deviantart.com/magiscarf )
https://www.deviantart.com/magiscarf/art/Tileset-ver-3-Free-690477146

=> Sprite Perso Epee ( Modified )
Credits to GameEndeavor. ( https://game-endeavor.itch.io/ )
https://game-endeavor.itch.io/mystic-woods

=> Sprite Effect Attaque ( Modified )
Credits to BDragon1727. ( https://bdragon1727.itch.io/ )
https://bdragon1727.itch.io/free-effect-and-bullet-16x16

=> Inventaire ( Modified )
Credits to ElvGames. ( https://elv-games.itch.io/ )
https://elv-games.itch.io/free-inventory-for-rpg-or-farming-games

=> Sprite Slime ( Modified )
Credits to melthie. ( https://melthie.itch.io/ )
https://melthie.itch.io/mini-dungeon

=> Sprite Mushroom & Goblin & Skull ( Modified )
Credtis to Stephen "Redshrike" Challener. ( https://opengameart.org/users/redshrike )
https://opengameart.org/content/16x16-16x24-32x32-rpg-enemies-updated

=> Sprite Boss Monster ( Modified )
Credtis to DeepDiveGameStudio. ( https://deepdivegamestudio.itch.io/ )
https://deepdivegamestudio.itch.io/demon-sprite-pack


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                   Autres ressources
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Diagramme de Gantt :
https://docs.google.com/spreadsheets/d/1DZzRiIsoBUpBN3pmYb6aemPsn6XqX6zBIC3njUfudq8/
