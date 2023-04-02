# Compilateur et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -I$(INCLUDEDIR) -g
LDFLAGS = $(shell ./tools/sdl2-config --cflags --libs) -lSDL2 -lSDL2_image -lSDL2_ttf

# Dossiers du projet
SRCDIR = src
INCLUDEDIR = include
OBJDIR = obj
BINDIR = bin
TESTDIR = test

# Fichiers sources et objets
PLAY_SRCS = $(SRCDIR)/play.c $(SRCDIR)/map.c $(SRCDIR)/affichage.c $(SRCDIR)/sprite.c \
            $(SRCDIR)/monstre.c $(SRCDIR)/personnage.c $(SRCDIR)/combat.c $(SRCDIR)/hud.c \
            $(SRCDIR)/objets.c $(SRCDIR)/pnj.c $(SRCDIR)/save.c $(SRCDIR)/quete.c
PLAY_OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(PLAY_SRCS))

#Liste des fichiers de test et de leurs dépendances
TEST_MAP_SRCS = $(TESTDIR)/test_map.c $(SRCDIR)/map.c
TEST_OBJET_SRCS = $(TESTDIR)/test_objet.c $(SRCDIR)/objets.c
TEST_PERSO_SRCS = $(TESTDIR)/test_perso.c $(SRCDIR)/personnage.c
TEST_SPRITE_SRCS = $(TESTDIR)/test_sprite.c $(SRCDIR)/sprite.c $(SRCDIR)/map.c $(SRCDIR)/pnj.c $(SRCDIR)/monstre.c

#Liste des fichiers objets correspondant aux fichiers de test
TEST_MAP_OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(TEST_MAP_SRCS))
TEST_OBJET_OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(TEST_OBJET_SRCS))
TEST_PERSO_OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(TEST_PERSO_SRCS))
TEST_SPRITE_OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(TEST_SPRITE_SRCS))


# Règles de construction des fichiers objets
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDEDIR)/*.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Règles de construction de l'exécutable
$(BINDIR)/play: $(PLAY_OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $(PLAY_OBJS) -o $@ $(LDFLAGS)

$(BINDIR):
	mkdir -p $(BINDIR)

# Règles de construction de l'exécutable principal et de ses dépendances
all: $(BINDIR)/play

$(BINDIR)/play: $(PLAY_OBJS)

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

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -rf $(OBJDIR)

mrproper: clean
	rm -rf $(BINDIR)

#Création du dossier de tests
$(TESTDIR)/:
	mkdir -p $(TESTDIR)

#Lancement des tests
test: $(BINDIR)/test_map $(BINDIR)/test_objet $(BINDIR)/test_perso $(BINDIR)/test_sprite

test_full: test
	$(TESTDIR)/test.sh