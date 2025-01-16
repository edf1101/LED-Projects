#ifndef LED_PROJECTS_SPECTRUMANALYSIS_H
#define LED_PROJECTS_SPECTRUMANALYSIS_H

#define I2S_WS 15        // aka LRCL
#define I2S_SD 32        // aka DOUT
#define I2S_SCK 14       // aka BCLK

#include <driver/i2s.h>
#include "arduinoFFT.h"

const int I2S_Samples = 512;
const int SAMPLE_RATE = 10240;

class SpectrumAnalysis {

public:
    void setupAudio(int sck, int ws, int sd, bool leftChannel);

    static int fftResult[16];                      // Our calculated result table, which we feed to the animations.

private:

    static double fftAdd(int from, int to);

    static void FFTcode(void *parameter);


    static const i2s_port_t I2S_PORT = I2S_NUM_0;
    const int BLOCK_SIZE = 64;


    TaskHandle_t FFT_Task;

    static const int squelch = 14;                           // Squelch, cuts out low level sounds
    static int gain;                            // Gain, boosts input level*/
    static uint16_t micData;                               // Analog input for FFT
    uint16_t micDataSm;                             // Smoothed mic data, as it's a bit twitchy

    static const uint16_t samples = I2S_Samples;                     // This value MUST ALWAYS be a power of 2
    static unsigned int sampling_period_us;
    static unsigned long microseconds;

    static double FFT_MajorPeak;
    static double FFT_Magnitude;
    uint16_t mAvg = 0;

    // These are the input and output vectors.  Input vectors receive computed results from FFT.
    static double vReal[I2S_Samples];
    static double vImag[I2S_Samples];
    static double fftBin[I2S_Samples];

    // Try and normalize fftBin values to a max of 4096, so that 4096/16 = 256.
    // Oh, and bins 0,1,2 are no good, so we'll zero them out.
    static double fftCalc[16];

    // Table of linearNoise results to be multiplied by squelch in order to reduce squelch across fftResult bins.
    static const int linearNoise[16];

    // Table of multiplication factors so that we can even out the frequency response.
    static const double fftResultPink[16];

    // declare FFT object
    static ArduinoFFT<double> FFT;

};


#endif //LED_PROJECTS_SPECTRUMANALYSIS_H
