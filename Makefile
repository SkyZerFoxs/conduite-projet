CC = gcc
CFLAGS = -Wall -g
EXEC = nomExec

SRCDIR=src
OBJDIR=obj
BINDIR=bin

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
	$(CC) $(OBJ) -o $(BINDIR)/$(EXEC) $(CFLAGS) -lm

obj/%.o: src/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	-rm -rf $(OBJDIR)
mrproper:
	-rm -rf $(BINDIR)