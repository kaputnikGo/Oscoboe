//
// Created by Sticks on 22-May-19.
//

#ifndef OSCOBOE_OSCILLATOR_H
#define OSCOBOE_OSCILLATOR_H

#include <cstdint>
#include <atomic>
#include <math.h>
#include <memory>
#include <chrono>
#include <thread>
#include "IRenderableAudio.h"
#include "logging_macros.h"

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

    void setNuhfRandom() {
        // call setFrequency() every time = t with a new 18-22kHz freq
        // needs carrier
        while (mDriftTestOn) {
            std::chrono::milliseconds dura(mNumFrames);
            std::this_thread::sleep_for(dura);
            mFrequency = 18000 + (std::rand() % mNumFrames);
        }
    }

    void runDriftTimer(bool runDrift) {
        // start it on separate THREAD
        if (runDrift) {
            timerThread = std::thread(&Oscillator::setNuhfRandom, this);
        }
        else {
            timerThread.detach();
        }
    }

    void runDriftTest(bool runDrift, int32_t numFrames) {
        // test with rand NUHF, lotsof harmonics, esp ~ 10k
        mDriftTestOn.store(runDrift);
        mNumFrames = numFrames;
        //LOGE("numFrames for rand(): %d", numFrames);
        runDriftTimer(runDrift);

    }

    inline void setAmplitude(float amplitude) {
        mAmplitude = amplitude;
    };

    // 1 frame size = sample size * channels,
    // ie: a stereo frame of shorts samples is 32bits (16 * 2)
    // and a buffer of 100 stereo frame of shorts is 400 bytes (4 * 100)
    // OpenSLES reports 192 frames per callback

    // From IRenderableAudio
    void renderAudio(float *audioData, int32_t numFrames) override {
        // main synth on switch
        if (mIsWaveOn){
            for (int i = 0; i < numFrames; ++i) {
                // check for drift on
                // sample[i] = Math.sin(driftFreq * 2 * Math.PI * i / sampleRate);
                if (mDriftTestOn) {
                    // needs a carrier var
                    // maybe drift the carrier per frame to increase range

                    // This will not yield a uniform distribution, but for this maybe is acceptable
                    // rand 192 for every frame
                    //mFrequency = 20000 + (std::rand() % mNumFrames);
                    // does not return mFrequency to init value after bool == Off
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
    int32_t mNumFrames = 100; // default
    std::thread timerThread;
};

#endif //OSCOBOE_OSCILLATOR_H
