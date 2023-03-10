CC = gcc
CFLAGS = -Wall -g
EXEC = nomExec
LIBS = 

SRCDIR=src
OBJDIR=obj
BINDIR=bin

SDL_DIR=${HOME}/libs/SDL2
SDLLIB_DIR=${SDL_DIR}/lib
SDLINC_DIR=${SDL_DIR}/include

LIBS=-L${SDLLIB_DIR} -lSDL2
INCLUDES=-I${SDLINC_DIR} 


DIRS=$(OBJDIR) $(BINDIR)

all: $(DIRS) $(BINDIR)/$(EXEC)

$(OBJDIR):
	mkdir -p $(OBJDIR)
$(BINDIR):
	mkdir -p $(BINDIR)
$(DOCDIR):
	mkdir -p $(DOCDIR)

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(BINDIR)/$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(BINDIR)/$(EXEC) $(CFLAGS) ${LIBS} ${INCLUDES}

obj/%.o: src/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	-rm -rf $(OBJDIR)
mrproper:
	-rm -rf $(BINDIR)