test_perso : test_perso.o personnage.o objets.o
	gcc personnage.o test_perso.o objets.o -o test_perso -I.

test_perso.o : test_perso.c 
	gcc -g test_perso.c -c test_perso.o -I.

personnage : personnage.o
	gcc personnage.o -o personnage -I.

personnnage.o : personnage.c personnage.h
	gcc -g personnage.c personnage.h -c personnage.o -I.

objets : objets.o 
	gcc objets.o -o objets -I.

objets.o : objets.c objets.h
	gcc -g objets.c objets.h -c objets.o -I.