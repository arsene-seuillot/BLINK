# Nom des exécutables
TARGET = blink
PLOTTER = plotter

# Fichiers source
SRC = blink.c effects.c
PLOTTER_SRC = graphique.c

# Fichiers objets
OBJ = $(SRC:.c=.o)
PLOTTER_OBJ = $(PLOTTER_SRC:.c=.o)

# Chemins vers les répertoires d'inclusion et de bibliothèque de PortAudio
INCLUDE_PATH = /opt/homebrew/include
LIBRARY_PATH = /opt/homebrew/lib

# Options du compilateur
CFLAGS = -I$(INCLUDE_PATH) -I. -Wall -g
LDFLAGS = -L$(LIBRARY_PATH) -lportaudio -lSDL2

# Règle par défaut
all: $(TARGET) $(PLOTTER)

# Règle pour construire l'exécutable principal
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Règle pour compiler les fichiers source en fichiers objets pour le programme principal
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour construire l'exécutable du programme de traçage
$(PLOTTER): $(PLOTTER_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Règle pour nettoyer les fichiers générés
clean:
	rm -f $(TARGET) $(OBJ) $(PLOTTER) $(PLOTTER_OBJ) output.txt plot_script.gp
	clear

# Règle pour exécuter l'exécutable principal
run: $(TARGET)
	./$(TARGET)

# Règle pour exécuter le programme de traçage
plot: $(PLOTTER)
	./$(PLOTTER)

.PHONY: all clean run run_plot
