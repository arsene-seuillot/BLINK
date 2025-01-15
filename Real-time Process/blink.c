#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "portaudio.h"
#include "effects.h"

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512
#define MAX_DELAY_SAMPLES (SAMPLE_RATE * 2) // Maximum 2 seconds delay
#define MAX_LOOP_SAMPLES (SAMPLE_RATE * 10) // Maximum 10 seconds loop

volatile int keepRunning = 1;

static int paCallback(const void *inputBuffer, void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData) {
    paData *data = (paData*)userData;
    const float *rptr = (const float*)inputBuffer;
    float *wptr = (float*)outputBuffer;
    unsigned int i;

     // Ouverture du fichier en mode "append" pour ajouter les données à chaque itération
    const char *output_filename = "output.txt";
    FILE *file = fopen(output_filename, "a");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    (void) timeInfo;
    (void) statusFlags;

    // Copy input to buffer
    if (inputBuffer == NULL) {
        for (i = 0; i < framesPerBuffer; i++) {
            data->buffer[i] = 0;
        }
    } else {
        for (i = 0; i < framesPerBuffer; i++) {
            data->buffer[i] = *rptr++;
            fprintf(file, "%f\n", data->buffer[i]);
        }
    }
    
    /* Apply effects */

    applyDistortion(data, framesPerBuffer);
    //applyReverb(data, framesPerBuffer);
    applyLowPassFilter(data, framesPerBuffer);

    // Process delay effect
    if (data->delayActive) {
        for (i = 0; i < framesPerBuffer; i++) {
            float delayedSample = data->delayBuffer[data->delayIndex];
            data->delayBuffer[data->delayIndex] = data->buffer[i] + delayedSample * data->feedback;
            data->buffer[i] += delayedSample;
            data->delayIndex = (data->delayIndex + 1) % data->delaySamples;
        }
    }

    // Process looper recording
    if (data->loopRecording) {
        for (i = 0; i < framesPerBuffer; i++) {
            data->loopBuffer[data->loopIndex] = data->buffer[i];
            data->loopIndex = (data->loopIndex + 1) % MAX_LOOP_SAMPLES;
            if (data->loopSamples < MAX_LOOP_SAMPLES) {
                data->loopSamples++;
            }
        }
    }

    // Process looper overdubbing
    if (data->loopOverdubbing && data->loopSamples > 0) {
        for (i = 0; i < framesPerBuffer; i++) {
            int loopIdx = data->loopIndex % data->loopSamples; // Ensure index is within loopSamples
            data->loopBuffer[loopIdx] += data->buffer[i]; // Add buffer to loop
            data->loopIndex = (data->loopIndex + 1) % MAX_LOOP_SAMPLES;
        }
    }

    // Process looper playback
    if (data->loopPlaying && data->loopSamples > 0) {
        for (i = 0; i < framesPerBuffer; i++) {
            int loopIdx = data->loopIndex % data->loopSamples; // Ensure index is within loopSamples
            data->buffer[i] += data->loopBuffer[loopIdx]; // Mix loop with input
            data->loopIndex = (data->loopIndex + 1) % data->loopSamples;
        }
    }

    // Write to output
    for (i = 0; i < framesPerBuffer; i++) {
        *wptr++ = data->buffer[i];
        *wptr++ = data->buffer[i];
    }

    return paContinue;
}

void* userCommandThread(void* userData) {
    paData* data = (paData*)userData;
    char command[256];

    while (keepRunning) {
        printf("Enter command (delay on/off, distortion on/off, reverb on/off, lowpass on/off, quit): ");
        if (fgets(command, sizeof(command), stdin) != NULL) {
            if (strncmp(command, "delay on", 8) == 0) {
                data->delayActive = 1;
                printf("Delay activated.\n");
            } else if (strncmp(command, "delay off", 9) == 0) {
                data->delayActive = 0;
                printf("Delay deactivated.\n");
            
            /* 
            On laisse le looper de côté pour l'instant, on se focus sur les effets en temps réel.
            } else if (strncmp(command, "loop record", 11) == 0) {
                data->loopRecording = 1;
                data->loopPlaying = 0;
                data->loopOverdubbing = 0;
                data->loopIndex = 0;
                data->loopSamples = 0;
                printf("Loop recording started.\n");
            } else if (strncmp(command, "loop play", 9) == 0) {
                if (data->loopSamples > 0) {
                    data->loopPlaying = 1;
                    data->loopRecording = 0;
                    data->loopOverdubbing = 0;
                    data->loopIndex = 0;
                    printf("Loop playing started.\n");
                } else {
                    printf("No loop to play.\n");
                }
            } else if (strncmp(command, "loop overdub", 12) == 0) {
                if (data->loopSamples > 0) {
                    data->loopOverdubbing = 1;
                    data->loopPlaying = 1;
                    data->loopRecording = 0;
                    data->loopIndex = 0;
                    printf("Loop overdubbing started.\n");
                } else {
                    printf("No loop to overdub.\n");
                }
            } else if (strncmp(command, "loop stop", 9) == 0) {
                data->loopPlaying = 0;
                data->loopRecording = 0;
                data->loopOverdubbing = 0;
                printf("Loop stopped.\n");
            */
            } else if (strncmp(command, "distortion on", 13) == 0) {
                data->distortionActive = 1;
                printf("Distortion activated.\n");
            } else if (strncmp(command, "distortion off", 14) == 0) {
                data->distortionActive = 0;
                printf("Distortion deactivated.\n");
            } else if (strncmp(command, "reverb on", 9) == 0) {
                data->reverbActive = 1;
                printf("Reverb activated.\n");
            } else if (strncmp(command, "reverb off", 10) == 0) {
                data->reverbActive = 0;
                printf("Reverb deactivated.\n");
            } else if (strncmp(command, "lowpass on", 10) == 0) {
                data->lowPassActive = 1;
                printf("Low-pass filter activated.\n");
            } else if (strncmp(command, "lowpass off", 11) == 0) {
                data->lowPassActive = 0;
                printf("Low-pass filter deactivated.\n");
            } else if (strncmp(command, "quit", 4) == 0) {
                keepRunning = 0;
                break;
            } else {
                printf("Unknown command.\n");
            }
        }
    }

    return NULL;
}

int main(void) {

    PaStreamParameters inputParameters, outputParameters;
    PaStream *stream;
    PaError err;
    paData data;

    memset(&data, 0, sizeof(paData));
    data.feedback = 0.2f; // Feedback value
    data.delaySamples = SAMPLE_RATE / 2; // 0.5 seconds delay
    data.delayActive = 0;
    data.lowPassCutoff = 2000.0f; // Default cutoff frequency for low-pass filter

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

    pthread_t commandThread;
    pthread_create(&commandThread, NULL, userCommandThread, &data);

    while (keepRunning) {
        Pa_Sleep(100);
    }

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

    pthread_join(commandThread, NULL);

    // Libérer les ressources associées à l'IR
    free(data.ir.irBuffer);
    

    return 0;
}
