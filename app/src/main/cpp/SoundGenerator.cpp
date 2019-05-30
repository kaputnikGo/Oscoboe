//
// Created by Sticks on 22-May-19.
//

#include <memory>
#include "SoundGenerator.h"

SoundGenerator::SoundGenerator(int32_t sampleRate, int32_t maxFrames, int32_t channelCount)
        : mSampleRate(sampleRate)
        , mChannelCount(channelCount)
        , mOscillators(std::make_unique<Oscillator[]>(channelCount))
        , mBuffer(std::make_unique<float[]>(maxFrames)){

    double frequency = 3700.0; // frequency default
    constexpr float amplitude = 1.0;

    // Set up the oscillators, (default 1=mono)
    for (int i = 0; i < mChannelCount; ++i) {
        mOscillators[i].setFrequency(frequency);
        mOscillators[i].setSampleRate(mSampleRate);
        mOscillators[i].setAmplitude(amplitude);
    }
}

void SoundGenerator::renderAudio(float *audioData, int32_t numFrames) {

    // Render each oscillator into its own channel
    for (int i = 0; i < mChannelCount; ++i) {
        mOscillators[i].renderAudio(mBuffer.get(), numFrames);
        for (int j = 0; j < numFrames; ++j) {
            audioData[(j * mChannelCount) + i] = mBuffer[j];
        }
    }
}

void SoundGenerator::setTonesOn(bool isOn) {
    for (int i = 0; i < mChannelCount; ++i) {
        mOscillators[i].setWaveOn(isOn);
    }
}

void SoundGenerator::setFrequency(double frequency) {
    for (int i = 0; i < mChannelCount; ++i) {
        mOscillators[i].setFrequency(frequency);
    }
}

void SoundGenerator::runDriftTest(bool runDrift, int32_t numFrames) {
    for (int i = 0; i < mChannelCount; ++i) {
        mOscillators[i].runDriftTest(runDrift, numFrames);
    }
}