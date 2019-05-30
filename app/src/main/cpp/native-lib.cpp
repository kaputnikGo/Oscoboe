#include <jni.h>
#include <string>
#include <oboe/Oboe.h>
#include "PlayAudioEngine.h"
#include "logging_macros.h"

extern "C" {
    JNIEXPORT jstring JNICALL
    Java_cityfreqs_com_oscoboe_MainActivity_stringFromJNI(
            JNIEnv *env,
            jobject /* this */) {
        std::string hello = "Hello from C++";
        return env->NewStringUTF(hello.c_str());
    }

    /**
    * Creates the audio engine
    *
    * @return a pointer to the audio engine. This should be passed to other methods
    */
    JNIEXPORT jlong JNICALL
    Java_cityfreqs_com_oscoboe_PlaybackEngine_native_1createEngine(
            JNIEnv *env,
            jclass /*unused*/) {
        // We use std::nothrow so `new` returns a nullptr if the engine creation fails
        PlayAudioEngine *engine = new(std::nothrow) PlayAudioEngine();
        return reinterpret_cast<jlong>(engine);
    }

    JNIEXPORT void JNICALL
    Java_cityfreqs_com_oscoboe_PlaybackEngine_native_1deleteEngine(
            JNIEnv *env,
            jclass,
            jlong engineHandle) {

        delete reinterpret_cast<PlayAudioEngine *>(engineHandle);
    }

    JNIEXPORT void JNICALL
    Java_cityfreqs_com_oscoboe_PlaybackEngine_native_1setToneOn(
            JNIEnv *env,
            jclass,
            jlong engineHandle,
            jboolean isToneOn) {

        PlayAudioEngine *engine = reinterpret_cast<PlayAudioEngine *>(engineHandle);
        if (engine == nullptr) {
            LOGE("Engine handle is invalid, call createHandle() to create a new one");
            return;
        }
        engine->setToneOn(isToneOn);
    }

    JNIEXPORT void JNICALL
    Java_cityfreqs_com_oscoboe_PlaybackEngine_native_1setFrequency(
            JNIEnv *env,
            jclass,
            jlong engineHandle,
            jdouble frequency) {

        PlayAudioEngine *engine = reinterpret_cast<PlayAudioEngine *>(engineHandle);
        if (engine == nullptr) {
            LOGE("Engine handle is invalid, call createHandle() to create a new one");
            return;
        }
        engine->setFrequency(frequency);
    }

    JNIEXPORT void JNICALL
    Java_cityfreqs_com_oscoboe_PlaybackEngine_native_1runDriftTest(
            JNIEnv *env,
            jclass,
            jlong engineHandle,
            jboolean runDrift) {

        PlayAudioEngine *engine = reinterpret_cast<PlayAudioEngine *>(engineHandle);
        if (engine == nullptr) {
            LOGE("Engine handle is invalid, call createHandle() to create a new one");
            return;
        }
        engine->runDriftTest(runDrift);
    }

    JNIEXPORT jint JNICALL
    Java_cityfreqs_com_oscoboe_PlaybackEngine_native_1getBufferSizeSelection(
            JNIEnv *env,
            jclass,
            jlong engineHandle) {

        PlayAudioEngine *engine = reinterpret_cast<PlayAudioEngine*>(engineHandle);
        if (engine == nullptr) {
            LOGE("Engine is null, you must call createEngine before calling this method");
            return static_cast<jint>(0);
        }
        return static_cast<jint>(engine->getBufferSizeSelection());
    }

    JNIEXPORT void JNICALL
    Java_cityfreqs_com_oscoboe_PlaybackEngine_native_1setAudioApi(
            JNIEnv *env,
            jclass type,
            jlong engineHandle,
            jint audioApi) {

        PlayAudioEngine *engine = reinterpret_cast<PlayAudioEngine*>(engineHandle);
        if (engine == nullptr) {
            LOGE("Engine handle is invalid, call createHandle() to create a new one");
            return;
        }

        oboe::AudioApi api = static_cast<oboe::AudioApi>(audioApi);
        engine->setAudioApi(api);
    }

    JNIEXPORT void JNICALL
    Java_cityfreqs_com_oscoboe_PlaybackEngine_native_1setAudioDeviceId(
            JNIEnv *env,
            jclass,
            jlong engineHandle,
            jint deviceId) {

        PlayAudioEngine *engine = reinterpret_cast<PlayAudioEngine*>(engineHandle);
        if (engine == nullptr) {
            LOGE("Engine handle is invalid, call createHandle() to create a new one");
            return;
        }
        engine->setDeviceId(deviceId);
    }

    JNIEXPORT void JNICALL
    Java_cityfreqs_com_oscoboe_PlaybackEngine_native_1setChannelCount(
            JNIEnv *env,
            jclass type,
            jlong engineHandle,
            jint channelCount) {

        PlayAudioEngine *engine = reinterpret_cast<PlayAudioEngine*>(engineHandle);
        if (engine == nullptr) {
            LOGE("Engine handle is invalid, call createHandle() to create a new one");
            return;
        }
        engine->setChannelCount(channelCount);
    }

    JNIEXPORT void JNICALL
    Java_cityfreqs_com_oscoboe_PlaybackEngine_native_1setBufferSizeInBursts(
            JNIEnv *env,
            jclass,
            jlong engineHandle,
            jint bufferSizeInBursts) {

        PlayAudioEngine *engine = reinterpret_cast<PlayAudioEngine*>(engineHandle);
        if (engine == nullptr) {
            LOGE("Engine handle is invalid, call createHandle() to create a new one");
            return;
        }
        engine->setBufferSizeInBursts(bufferSizeInBursts);
    }


    JNIEXPORT jdouble JNICALL
    Java_cityfreqs_com_oscoboe_PlaybackEngine_native_1getCurrentOutputLatencyMillis(
            JNIEnv *env,
            jclass,
            jlong engineHandle) {

        PlayAudioEngine *engine = reinterpret_cast<PlayAudioEngine*>(engineHandle);
        if (engine == nullptr) {
            LOGE("Engine is null, you must call createEngine before calling this method");
            return static_cast<jdouble>(-1.0);
        }
        return static_cast<jdouble>(engine->getCurrentOutputLatencyMillis());
    }

    JNIEXPORT jboolean JNICALL
    Java_cityfreqs_com_oscoboe_PlaybackEngine_native_1isLatencyDetectionSupported(
            JNIEnv *env,
            jclass type,
            jlong engineHandle) {

        PlayAudioEngine *engine = reinterpret_cast<PlayAudioEngine*>(engineHandle);
        if (engine == nullptr) {
            LOGE("Engine is null, you must call createEngine before calling this method");
            return JNI_FALSE;
        }
        return (engine->isLatencyDetectionSupported() ? JNI_TRUE : JNI_FALSE);
    }


    JNIEXPORT void JNICALL
    Java_cityfreqs_com_oscoboe_PlaybackEngine_native_1setDefaultSampleRate(JNIEnv *env,
                                                                                      jclass type,
                                                                                      jint sampleRate) {
        oboe::DefaultStreamValues::SampleRate = (int32_t) sampleRate;
    }

    JNIEXPORT void JNICALL
    Java_cityfreqs_com_oscoboe_PlaybackEngine_native_1setDefaultFramesPerBurst(JNIEnv *env,
                                                                                          jclass type,
                                                                                          jint framesPerBurst) {
        oboe::DefaultStreamValues::FramesPerBurst = (int32_t) framesPerBurst;
    }
}
