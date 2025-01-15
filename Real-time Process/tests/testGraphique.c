#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Graphique en temps réel", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Erreur de création de fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Erreur de création de renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int running = 1;
    SDL_Event event;
    float time = 0.0f;
    const float dt = 0.05f;  // Incrément de temps

    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Dessiner la sinusoïde
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Couleur verte pour le graphique
        int previousX = 0;
        int previousY = SCREEN_HEIGHT / 2;

        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            // Calcul de la valeur de la sinusoïde avec un léger décalage en fonction du temps
            int y = (int)(SCREEN_HEIGHT / 2 + 100 * sinf(0.04f * x + time));
            SDL_RenderDrawLine(renderer, previousX, previousY, x, y);  // Relie les points pour dessiner la courbe
            previousX = x;
            previousY = y;
        }

        // Mettre à jour le rendu
        SDL_RenderPresent(renderer);

        // Mettre à jour le temps pour animer la sinusoïde
        time += dt;

        // Attendre un peu pour contrôler la vitesse de rafraîchissement
        SDL_Delay(16);  // ~60 FPS
    }

    // Nettoyage
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
