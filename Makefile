# Compilateur et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -I$(INCLUDEDIR) -g
LDFLAGS = $(shell sdl2-config --cflags --libs) -lSDL2 -lSDL2_image -lSDL2_ttf

# Dossiers du projet
SRCDIR = src
INCLUDEDIR = include
OBJDIR = obj
BINDIR = bin
TESTDIR = test
TEST_RESULT_DIR = test/resultat

# Fichiers sources et objets
MAIN_SRCS = $(SRCDIR)/play.c $(SRCDIR)/map.c $(SRCDIR)/affichage.c $(SRCDIR)/sprite.c \
            $(SRCDIR)/monstre.c $(SRCDIR)/personnage.c $(SRCDIR)/combat.c $(SRCDIR)/hud.c \
            $(SRCDIR)/objets.c $(SRCDIR)/pnj.c $(SRCDIR)/save.c $(SRCDIR)/quete.c  $(SRCDIR)/menu.c  $(SRCDIR)/main.c 
MAIN_OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(MAIN_SRCS))

#Liste des fichiers de test et de leurs dépendances
TEST_MAP_SRCS = $(TESTDIR)/test_map.c $(SRCDIR)/map.c
TEST_OBJET_SRCS = $(TESTDIR)/test_objet.c $(SRCDIR)/objets.c
TEST_PERSO_SRCS = $(TESTDIR)/test_perso.c $(SRCDIR)/personnage.c
TEST_SPRITE_SRCS = $(TESTDIR)/test_sprite.c $(SRCDIR)/sprite.c $(SRCDIR)/map.c $(SRCDIR)/pnj.c $(SRCDIR)/monstre.c
JEU_TEST_PERSO_SRCS = $(TESTDIR)/jeu_test_personnage.c $(SRCDIR)/personnage.c


#Liste des fichiers objets correspondant aux fichiers de test
TEST_MAP_OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(TEST_MAP_SRCS))
TEST_OBJET_OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(TEST_OBJET_SRCS))
TEST_PERSO_OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(TEST_PERSO_SRCS))
TEST_SPRITE_OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(TEST_SPRITE_SRCS))
TEST_JEU_TEST_PERSONNAGE_OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(JEU_TEST_PERSO_SRCS))


# Règles de construction des fichiers objets
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDEDIR)/*.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Règles de construction de l'exécutable
$(BINDIR)/main: $(MAIN_OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $(MAIN_OBJS) -o $@ $(LDFLAGS)

$(BINDIR):
	mkdir -p $(BINDIR)

# Règles de construction de l'exécutable principal et de ses dépendances
all: $(BINDIR)/main

$(BINDIR)/main: $(MAIN_OBJS)

#Règles de construction des fichiers objets de test
$(OBJDIR)/test_map.o: $(TESTDIR)/test_map.c $(INCLUDEDIR)/map.h $(SRCDIR)/map.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/test_objet.o: $(TESTDIR)/test_objet.c $(INCLUDEDIR)/objets.h $(SRCDIR)/objets.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/test_perso.o: $(TESTDIR)/test_perso.c $(INCLUDEDIR)/personnage.h $(SRCDIR)/personnage.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/test_sprite.o: $(TESTDIR)/test_sprite.c $(INCLUDEDIR)/sprite.h $(SRCDIR)/sprite.c $(INCLUDEDIR)/map.h $(SRCDIR)/map.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Règles de construction des programmes de test
$(BINDIR)/test_map: $(TEST_MAP_OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $(TEST_MAP_OBJS) -o $@ $(LDFLAGS)

$(BINDIR)/test_objet: $(TEST_OBJET_OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $(TEST_OBJET_OBJS) -o $@ $(LDFLAGS)

$(BINDIR)/test_perso: $(TEST_PERSO_OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $(TEST_PERSO_OBJS) -o $@ $(LDFLAGS)

$(BINDIR)/test_sprite: $(TEST_SPRITE_OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $(TEST_SPRITE_OBJS) -o $@ $(LDFLAGS)

$(BINDIR)/jeu_test_personnage: $(JEU_TEST_PERSO_SRCS) | $(BINDIR)
	$(CC) $(CFLAGS) $(JEU_TEST_PERSO_SRCS) -o $@ $(LDFLAGS) -DUNIT_TEST -lcunit

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -rf obj/*.o

mrproper: clean
	rm -rf $(BINDIR)/main
	rm -rf $(TEST_RESULT_DIR)

#Création du dossier de tests
$(TESTDIR)/:
	mkdir -p $(TESTDIR)

#Lancement des tests
test: $(BINDIR)/test_map $(BINDIR)/test_objet $(BINDIR)/test_perso $(BINDIR)/test_sprite $(BINDIR)/jeu_test_personnage
	mkdir -p $(TEST_RESULT_DIR)

test_full: test
	$(TESTDIR)/test.sh