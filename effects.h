// effects.h

#ifndef EFFECTS_H
#define EFFECTS_H

#include "portaudio.h"

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512
#define MAX_DELAY_SAMPLES (SAMPLE_RATE * 2) // Maximum 2 seconds delay
#define MAX_LOOP_SAMPLES (SAMPLE_RATE * 10) // Maximum 10 seconds loop

// Réponse impultionelle pour la réverbération
typedef struct {
    float *irBuffer;   // Buffer pour la réponse impulsionnelle
    unsigned int irLength;  // Longueur de l'IR
} ImpulseResponse;

// structure qui contient toutes les données à utiliser dans le programme principal
typedef struct {
    float buffer[FRAMES_PER_BUFFER];
    float delayBuffer[MAX_DELAY_SAMPLES];
    int delayIndex;
    float feedback;
    int delaySamples;
    int delayActive;
    float loopBuffer[MAX_LOOP_SAMPLES];
    int loopIndex;
    int loopSamples;
    int loopActive;
    int loopRecording;
    int loopPlaying;
    int loopOverdubbing;
    int distortionActive;
    int reverbActive;
    int lowPassActive;

    // Add fields for effects
    ImpulseResponse ir; // For reverb effect
    float lowPassCutoff; // For low-pass filter effect
    // Add any other fields needed for your effects
} paData;




void applyDistortion(paData* data, unsigned long framesPerBuffer);
void loadImpulseResponse(ImpulseResponse *ir, const char *irFilePath);
void applyReverb(paData* data, unsigned long framesPerBuffer);
void applyLowPassFilter(paData* data, unsigned long framesPerBuffer);

#endif
