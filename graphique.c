#include <stdio.h>
#include <stdlib.h>

// Nom du fichier texte contenant les valeurs
#define INPUT_FILE "output.txt"
// Nom du fichier script Gnuplot
#define GNUPLOT_SCRIPT "plot_script.gp"
// Fréquence d'échantillonnage (à adapter selon vos besoins)
#define SAMPLE_RATE 44100

int main() {
    FILE *inputFile, *gnuplotScript;
    float sample;
    int sampleCount = 0;
    
    // 1. Lire les données du fichier texte et compter le nombre de valeurs
    inputFile = fopen(INPUT_FILE, "r");
    if (inputFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier d'entrée");
        return 1;
    }

    // Compter le nombre d'échantillons
    while (fscanf(inputFile, "%f", &sample) == 1) {
        sampleCount++;
    }
    rewind(inputFile); // Retourner au début du fichier pour relire les données

    // Créer un tableau pour stocker les valeurs lues
    float *data = (float *)malloc(sampleCount * sizeof(float));
    if (data == NULL) {
        perror("Erreur d'allocation mémoire");
        fclose(inputFile);
        return 1;
    }

    // Lire les valeurs dans le tableau
    for (int i = 0; i < sampleCount; i++) {
        fscanf(inputFile, "%f", &data[i]);
    }
    fclose(inputFile);

    // 2. Générer un fichier script pour Gnuplot
    gnuplotScript = fopen(GNUPLOT_SCRIPT, "w");
    if (gnuplotScript == NULL) {
        perror("Erreur lors de la création du script Gnuplot");
        free(data);
        return 1;
    }

    // Écrire les commandes Gnuplot dans le script
    fprintf(gnuplotScript, "set title 'Signal Audio'\n");
    fprintf(gnuplotScript, "set xlabel 'Temps (s)'\n");
    fprintf(gnuplotScript, "set ylabel 'Amplitude'\n");
    fprintf(gnuplotScript, "set grid\n");
    fprintf(gnuplotScript, "plot '-' with lines title 'Signal'\n");

    // Calculer le temps pour chaque échantillon et écrire les données dans le script
    for (int i = 0; i < sampleCount; i++) {
        double time = (double)i / SAMPLE_RATE; // Calcul du temps en secondes
        fprintf(gnuplotScript, "%lf %f\n", time, data[i]);
    }

    fprintf(gnuplotScript, "e\n"); // Fin des données pour Gnuplot
    fclose(gnuplotScript);

    // 3. Exécuter Gnuplot pour afficher le graphique
    int status = system("gnuplot -p " GNUPLOT_SCRIPT);
    if (status == -1) {
        perror("Erreur lors de l'exécution de Gnuplot");
    }

    // Libérer la mémoire
    free(data);

    return 0;
}
