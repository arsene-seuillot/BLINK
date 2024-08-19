# Nom de l'exécutable
TARGET = blink

# Fichiers source
SRC = blink.c effects.c

# Fichiers objets
OBJ = $(SRC:.c=.o)

# Chemins vers les répertoires d'inclusion et de bibliothèque de PortAudio
INCLUDE_PATH = /opt/homebrew/include
LIBRARY_PATH = /opt/homebrew/lib

# Options du compilateur
CFLAGS = -I$(INCLUDE_PATH) -Wall -g
LDFLAGS = -L$(LIBRARY_PATH) -lportaudio

# Règle par défaut
all: $(TARGET)

# Règle pour construire l'exécutable
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Règle pour compiler les fichiers source en fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers générés
clean:
	rm -f $(TARGET) $(OBJ)
	clear

# Règle pour exécuter l'exécutable
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
