//
// Created by Sticks on 22-May-19.
//

#ifndef OSCOBOE_SOUNDGENERATOR_H
#define OSCOBOE_SOUNDGENERATOR_H

#include "Oscillator.h"
#include "IRenderableAudio.h"

/**
 * Generates a fixed frequency tone for each channel.
 */
class SoundGenerator : public IRenderableAudio {
public:
    /**
     * Create a new SoundGenerator object.
     *
     * @param sampleRate - The output sample rate.
     * @param maxFrames - The maximum number of audio frames which will be rendered, this is used to
     * calculate this object's internal buffer size.
     * @param channelCount - The number of channels in the output, one tone will be created for each
     * channel, the output will be interlaced.
     *
     */
    SoundGenerator(int32_t sampleRate, int32_t maxFrames, int32_t channelCount);
    ~SoundGenerator() = default;

    // Switch the tones on
    void setTonesOn(bool isOn);

    // update the oscillator frequency
    void setFrequency(double frequency);
    // test random drift > 18kHz
    void runDriftTest(bool runDrift);

    // From IRenderableAudio
    void renderAudio(float *audioData, int32_t numFrames) override;


private:
    const int32_t mSampleRate;
    const int32_t mChannelCount;
    //std::atomic<Oscillator> mOscar;
    const std::unique_ptr<Oscillator[]> mOscillators;
    const std::unique_ptr<float[]> mBuffer;
};

#endif //OSCOBOE_SOUNDGENERATOR_H
