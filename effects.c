// effects.c

#include "effects.h"
#include <math.h>

void applyDistortion(paData *data, unsigned long framesPerBuffer)
{
    if (!data->distortionActive)
        return;

    for (unsigned int i = 0; i < framesPerBuffer; i++)
    {
        if (data->buffer[i] > 0.3f)
        {
            data->buffer[i] = 0.3f;
        }
        else if (data->buffer[i] < -0.3f)
        {
            data->buffer[i] = -0.3f;
        }
    }
}

/*
// fonction pour charger la réponse impultionelle
void loadImpulseResponse(ImpulseResponse *ir, const char *irFilePath) {
    // Pour cet exemple, je vais simplement initialiser un buffer simple ici.
    // Dans un cas réel, vous devrez charger l'IR depuis un fichier.

    // Simuler une IR simple pour l'exemple
    ir->irLength = SAMPLE_RATE / 2;  // Par exemple, une demi-seconde d'IR
    ir->irBuffer = (float *)malloc(ir->irLength * sizeof(float));
    for (unsigned int i = 0; i < ir->irLength; i++) {
        ir->irBuffer[i] = (float)(rand() % 1000) / 1000.0f;  // Exemple aléatoire
    }
}

// fonction pour appliquer la réverb par convolution de la réponse impultionelle
void applyReverb(paData *data, ImpulseResponse *ir, unsigned long framesPerBuffer) {
    if (!data->reverbActive || !ir->irBuffer) return;

    unsigned int N = framesPerBuffer + ir->irLength - 1;

    // Allouer des buffers pour la FFT
    fftwf_complex *in1 = fftwf_alloc_complex(N);
    fftwf_complex *in2 = fftwf_alloc_complex(N);
    fftwf_complex *out = fftwf_alloc_complex(N);

    // Plan de la FFT
    fftwf_plan p1 = fftwf_plan_dft_r2c_1d(N, data->buffer, in1, FFTW_ESTIMATE);
    fftwf_plan p2 = fftwf_plan_dft_r2c_1d(N, ir->irBuffer, in2, FFTW_ESTIMATE);
    fftwf_plan p3 = fftwf_plan_dft_c2r_1d(N, out, data->buffer, FFTW_ESTIMATE);

    // Appliquer la FFT sur le buffer d'entrée et l'IR
    fftwf_execute(p1);
    fftwf_execute(p2);

    // Convolution dans le domaine fréquentiel
    for (unsigned int i = 0; i < N; i++) {
        out[i][0] = in1[i][0] * in2[i][0] - in1[i][1] * in2[i][1];
        out[i][1] = in1[i][0] * in2[i][1] + in1[i][1] * in2[i][0];
    }

    // Transformer de nouveau dans le domaine temporel
    fftwf_execute(p3);

    // Normaliser le résultat
    for (unsigned int i = 0; i < framesPerBuffer; i++) {
        data->buffer[i] /= N;
    }

    // Libérer les ressources FFTW
    fftwf_destroy_plan(p1);
    fftwf_destroy_plan(p2);
    fftwf_destroy_plan(p3);
    fftwf_free(in1);
    fftwf_free(in2);
    fftwf_free(out);
}
*/

void applyLowPassFilter(paData *data, unsigned long framesPerBuffer)
{
    if (!data->lowPassActive)
        return;

    static float previousSample = 0.0f;
    float alpha = 0.1f; // Facteur de lissage : plus il est bas, plus les hautes fréquences sont atténuées

    for (unsigned int i = 0; i < framesPerBuffer; i++)
    {
        data->buffer[i] = alpha * data->buffer[i] + (1.0f - alpha) * previousSample;
        previousSample = data->buffer[i];
    }
}
