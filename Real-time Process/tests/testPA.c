## Template de base pour le fonctionnement de Port Audio


#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512

typedef struct {
    float buffer[FRAMES_PER_BUFFER];
} paData;

/* This routine will be called by the PortAudio engine when audio is needed. */
static int paCallback(const void *inputBuffer, void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData) {
    paData *data = (paData*)userData;
    const float *rptr = (const float*)inputBuffer;
    float *wptr = (float*)outputBuffer;
    unsigned int i;

    /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;

    if (inputBuffer == NULL) {
        for (i = 0; i < framesPerBuffer; i++) {
            data->buffer[i] = 0;
        }
    } else {
        for (i = 0; i < framesPerBuffer; i++) {
            data->buffer[i] = *rptr++;
        }
    }

    for (i = 0; i < framesPerBuffer; i++) {
        *wptr++ = data->buffer[i]; /* left */
        *wptr++ = data->buffer[i]; /* right */
    }

    return paContinue;
}

int main(void) {
    PaStreamParameters inputParameters, outputParameters;
    PaStream *stream;
    PaError err;
    paData data;

    err = Pa_Initialize();
    if (err != paNoError) {
        fprintf(stderr, "Error initializing PortAudio: %s\n", Pa_GetErrorText(err));
        return -1;
    }

    inputParameters.device = Pa_GetDefaultInputDevice();
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr, "No default input device.\n");
        Pa_Terminate();
        return -1;
    }
    inputParameters.channelCount = 1;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr, "No default output device.\n");
        Pa_Terminate();
        return -1;
    }
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(&stream, &inputParameters, &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, paCallback, &data);
    if (err != paNoError) {
        fprintf(stderr, "Error opening stream: %s\n", Pa_GetErrorText(err));
        Pa_Terminate();
        return -1;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "Error starting stream: %s\n", Pa_GetErrorText(err));
        Pa_CloseStream(stream);
        Pa_Terminate();
        return -1;
    }

    printf("Press ENTER to stop the stream...\n");
    getchar();

    err = Pa_StopStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "Error stopping stream: %s\n", Pa_GetErrorText(err));
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "Error closing stream: %s\n", Pa_GetErrorText(err));
    }

    Pa_Terminate();
    printf("PortAudio terminated.\n");

    return 0;
}
