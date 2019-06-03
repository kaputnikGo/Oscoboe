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
constexpr double kDefaultFrequency = 1000;
constexpr int32_t kDefaultSampleRate = 48000;

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
        mNextFrequency = frequency;
        // frequency precision is +/- 500 hz, other multiples cause multiple harmonics below fundamental
        // change here requires one numFrames buffer to be zeroed first before
    };

    void setNuhfRandom() {
        // call setFrequency() every time = t with a new 18-22kHz freq
        // needs carrier
        // mDriftSpeedMs def = 1000, minimum = mNumFrames (192) causes static

        while (mDriftTestOn) {
            std::chrono::milliseconds dura(mDriftSpeedMs);
            std::this_thread::sleep_for(dura);
            mNextFrequency = 18000 + (500 * (std::rand() % 8));
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

    // change in frequency via any method causes peak transients at start and end of waveform

    // From IRenderableAudio
    void renderAudio(float *audioData, int32_t numFrames) override {
        // main synth on switch
        if (mIsWaveOn){
            for (int i = 0; i < numFrames; ++i) {
                // always starts at peak amplitude values (0,360 deg on circle) or 90deg phase
                // sRC-1/sRC+1, which will induce a 90 degree phase shift.
                audioData[i] = sinf(float((2 * M_PI * mFrequency) / mSampleRate * i)); // starts at 90deg phase

                //audioData[i] = cosf(float((2 * M_PI * mFrequency) / mSampleRate * i)); // starts at 180deg phase
            }
            // after numFrames buffer end, change freq if diff
            mFrequency = mNextFrequency;
        }
        else {
            // pad the audioData buffer with zeros
            memset(audioData, 0, sizeof(float) * numFrames);
        }
    };

private:
    std::atomic<bool> mIsWaveOn { false };
    std::atomic<bool> mDriftTestOn { false };
    std::atomic<float> mAmplitude { 0 };
    double mFrequency = kDefaultFrequency;
    double mNextFrequency = kDefaultFrequency;
    int32_t mSampleRate = kDefaultSampleRate;
    int32_t mNumFrames = 100; // default
    int32_t mDriftSpeedMs = 250; // ms
    std::thread timerThread;
};

#endif //OSCOBOE_OSCILLATOR_H
