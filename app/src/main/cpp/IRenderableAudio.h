//
// Created by Sticks on 22-May-19.
//

#ifndef OSCOBOE_IRENDERABLEAUDIO_H
#define OSCOBOE_IRENDERABLEAUDIO_H

#include <cstdint>
#include <string>

class IRenderableAudio {

public:
    virtual ~IRenderableAudio() = default;
    virtual void renderAudio(float *audioData, int32_t numFrames) = 0;
};


#endif //OSCOBOE_IRENDERABLEAUDIO_H
