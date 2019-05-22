//
// Created by Sticks on 22-May-19.
//

#ifndef OSCOBOE_OSCILLATOR_H
#define OSCOBOE_OSCILLATOR_H

#include <cstdint>
#include <atomic>
#include <math.h>
#include <memory>
#include "IRenderableAudio.h"

// get an int here for the freqs
constexpr double kDefaultFrequency = 3700;
constexpr int32_t kDefaultSampleRate = 48000;

constexpr float_t kPi = M_PI; // 3.141
constexpr float_t kTwoPi = kPi * 2; // 6.283

class Oscillator : public IRenderableAudio {

public:

    ~Oscillator() = default;

    void setWaveOn(bool isWaveOn) {
        mIsWaveOn.store(isWaveOn);
    };

    void setSampleRate(int32_t sampleRate) {
        mSampleRate = sampleRate;
    };

    void setFrequency(double frequency) {
        mFrequency = frequency;
    };

    void runDriftTest(bool runDrift) {
        // test with rand 18kHz + 0-2000
        mDriftTestOn.store(runDrift);
    }

    inline void setAmplitude(float amplitude) {
        mAmplitude = amplitude;
    };

    // From IRenderableAudio
    void renderAudio(float *audioData, int32_t numFrames) override {

        if (mIsWaveOn){
            for (int i = 0; i < numFrames; ++i) {

                // sample[i] = Math.sin(driftFreq * 2 * Math.PI * i / sampleRate);
                if (mDriftTestOn) {
                    // numFrames is NOT sampleRate...
                    // every nth iteration get a new drift freq (48k rate / driftSpeed ) (1~10 * 1000)
                    if (i % 1000 == 0) {
                        //needs a carrier var
                        mFrequency = 18000 + (rand() % 2000);
                    }
                }
                audioData[i] = sinf(float(mFrequency * 2 * kPi * i / mSampleRate));
            }
        }
        else {
            memset(audioData, 0, sizeof(float) * numFrames);
        }
    };

private:
    std::atomic<bool> mIsWaveOn { false };
    std::atomic<bool> mDriftTestOn { false };
    std::atomic<float> mAmplitude { 0 };
    double mFrequency = kDefaultFrequency;
    int32_t mSampleRate = kDefaultSampleRate;
};

#endif //OSCOBOE_OSCILLATOR_H
