//
// Created by Sticks on 22-May-19.
//

#ifndef OSCOBOE_PLAYAUDIOENGINE_H
#define OSCOBOE_PLAYAUDIOENGINE_H

#include <thread>
#include <array>
#include <oboe/Oboe.h>

#include "Mixer.h"

#include "SoundGenerator.h"

constexpr int32_t kBufferSizeAutomatic = 0;

class PlayAudioEngine : oboe::AudioStreamCallback {

public:
    PlayAudioEngine();

    ~PlayAudioEngine();

    void setAudioApi(oboe::AudioApi audioApi);

    void setDeviceId(int32_t deviceId);

    void setToneOn(bool isToneOn);

    void setFrequency(double frequency);

    void runDriftTest(bool runDrift);

    void setBufferSizeInBursts(int32_t numBursts);

    double getCurrentOutputLatencyMillis();

    bool isLatencyDetectionSupported();

    // oboe::StreamCallback methods
    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames);

    void onErrorAfterClose(oboe::AudioStream *oboeStream, oboe::Result error);

    void setChannelCount(int channelCount);

private:
    oboe::AudioApi mAudioApi = oboe::AudioApi::Unspecified;
    int32_t mPlaybackDeviceId = oboe::kUnspecified;
    int32_t mSampleRate;
    int32_t mChannelCount = 2; // Stereo
    int32_t mFramesPerBurst;
    double mCurrentOutputLatencyMillis = 0;
    int32_t mBufferSizeSelection = kBufferSizeAutomatic;
    bool mIsLatencyDetectionSupported = false;
    oboe::AudioStream *mPlayStream;
    std::unique_ptr<oboe::LatencyTuner> mLatencyTuner;
    std::mutex mRestartingLock;
    std::unique_ptr<SoundGenerator> mSoundGenerator;
    std::unique_ptr<float[]> mConversionBuffer { nullptr };

    void createPlaybackStream();

    void closeOutputStream();

    void restartStream();

    void setupPlaybackStreamParameters(oboe::AudioStreamBuilder *builder);

    oboe::Result calculateCurrentOutputLatencyMillis(oboe::AudioStream *stream, double *latencyMillis);

};

#endif //OSCOBOE_PLAYAUDIOENGINE_H
