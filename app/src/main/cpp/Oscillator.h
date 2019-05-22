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

constexpr double kDefaultFrequency = 3700.0;
// get an int here for the freqs
        
constexpr int32_t kDefaultSampleRate = 48000;
constexpr double kPi = M_PI; // 3.141
constexpr double kTwoPi = kPi * 2; // 6.283

class Oscillator : public IRenderableAudio {

public:

    ~Oscillator() = default;

    void setWaveOn(bool isWaveOn) {
        mIsWaveOn.store(isWaveOn);
    };

    void setSampleRate(int32_t sampleRate) {
        mSampleRate = sampleRate;
        updatePhaseIncrement();
    };

    void setFrequency(double frequency) {
        mFrequency = frequency;
        //updatePhaseIncrement();
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
                    // every nth iteration get a new drift freq (48k rate / driftSpeed ) (1 to 10 * 1000)
                    if (i % 1000 == 0) {
                        // test every 1000th
                        //needs a carrier var
                        mFrequency = 18000 + (rand() % 2000);
                    }
                }
                audioData[i] = sinf(mFrequency * 2 * kPi * i / mSampleRate);

                // Sine wave (sinf)
                //audioData[i*2] = sinf(mPhase) * mAmplitude;

                // Square wave
                /*
                if (mPhase <= kPi){
                    audioData[i] = -mAmplitude;
                } else {
                    audioData[i] = mAmplitude;
                }
                */

                mPhase += mPhaseIncrement;
                if (mPhase > kTwoPi) mPhase -= kTwoPi;
            }
        } else {
            memset(audioData, 0, sizeof(float) * numFrames);
        }
    };

private:
    std::atomic<bool> mIsWaveOn { false };
    std::atomic<bool> mDriftTestOn { false };
    float mPhase = 0.0;
    std::atomic<float> mAmplitude { 0 };
    std::atomic<double> mPhaseIncrement { 0.0 };
    double mFrequency = kDefaultFrequency;
    int32_t mSampleRate = kDefaultSampleRate;

    void updatePhaseIncrement() {
        mPhaseIncrement.store((kTwoPi * mFrequency) / static_cast<double>(mSampleRate));
        // eg. kTwoPi * 3700 / 48000 = phaseIncrement = 0.484
    };
};

#endif //OSCOBOE_OSCILLATOR_H
