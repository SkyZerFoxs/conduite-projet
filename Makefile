OBJETS = personnage.o objets.o test_perso.o test_objet.o
CC = gcc
CFLAGS = -W -Wall -Iinclude

SRC=src
INCLUDE=include
OBJ=obj
TEST=test

all : test_perso test_objet test_monstre test_combat

test_perso : test_perso.o objets.o personnage.o 
	$(CC) -o $@ $^ $(CFLAGS)

test_objet : test_objet.o objets.o 
	$(CC) -o $@ $^ $(CFLAGS)

test_monstre : test_monstre.o monstre.o
	$(CC) -o $@ $^ $(CFLAGS)

test_combat : test_combat.o combat.o personnage.o monstre.o 
	$(CC) -o $@ $^ $(CFLAGS)

test_perso.o : $(TEST)/test_perso.c $(INCLUDE)/objets.h $(INCLUDE)/personnage.h
	$(CC) -c $^ $(CFLAGS)

test_objet.o : $(TEST)/test_objet.c $(INCLUDE)/objets.h 
	$(CC) -c $^ $(CFLAGS)

test_monstre.o : $(TEST)/test_monstre.c $(INCLUDE)/monstre.h 
	$(CC) -c $^ $(CFLAGS)

test_combat.o : $(TEST)/test_combat.c $(INCLUDE)/combat.h $(INCLUDE)/personnage.h $(INCLUDE)/monstre.h 
	$(CC) -c $^ $(CFLAGS)

monstre.o : $(SRC)/monstre.c $(INCLUDE)/monstre.h 
	$(CC) -c $^ $(CFLAGS)

objets.o : $(SRC)/objets.c $(INCLUDE)/objets.h 
	$(CC) -c $^ $(CFLAGS)

personnage.o : $(SRC)/personnage.c $(INCLUDE)/personnage.h
	$(CC) -c $^ $(CFLAGS)

combat.o : $(SRC)/combat.c $(INCLUDE)/combat.h $(INCLUDE)/personnage.h $(INCLUDE)/monstre.h 
	$(CC) -c $^ $(CFLAGS)

clean:
	rm -rf $(OBJETS)