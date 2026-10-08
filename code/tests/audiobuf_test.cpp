#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "../audio/audiobuf.h"
#include <mmreg.h>

namespace {
const int sampleCount = 576;

int SampleAt(int frame, int channel) {
    return ((frame * (channel == 0 ? 1 : 17) + channel * 29) % 255) - 128;
}

bool CheckPackets(const char* name, const std::vector<UINT32>& packets,
                  bool int16, WORD channels) {
    WAVEFORMATEX format = {};
    format.wFormatTag = int16 ? WAVE_FORMAT_PCM : WAVE_FORMAT_IEEE_FLOAT;
    format.nChannels = channels;
    format.nSamplesPerSec = 48000;
    format.wBitsPerSample = int16 ? 16 : 32;
    format.nBlockAlign = channels * format.wBitsPerSample / 8;
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

    ResetAudioBuf();
    int totalFrames = 0;
    for (UINT32 frames : packets) {
        std::vector<BYTE> data(frames * format.nBlockAlign);
        for (UINT32 frame = 0; frame < frames; ++frame) {
            for (WORD channel = 0; channel < channels; ++channel) {
                BYTE* destination = data.data() + frame * format.nBlockAlign
                    + channel * (format.wBitsPerSample / 8);
                const int sample = SampleAt(totalFrames + frame, channel);
                if (int16) {
                    const int16_t value = static_cast<int16_t>(sample * 256);
                    std::memcpy(destination, &value, sizeof(value));
                } else {
                    const float value = sample / 128.0f;
                    std::memcpy(destination, &value, sizeof(value));
                }
            }
        }
        SetAudioBuf(data.data(), frames, &format, int16);
        totalFrames += frames;
    }

    unsigned char left[sampleCount];
    unsigned char right[sampleCount];
    GetAudioBuf(left, right, sampleCount);
    for (int i = 0; i < sampleCount; ++i) {
        const int frame = totalFrames - sampleCount + i;
        const unsigned char expectedLeft = totalFrames < sampleCount ? 0
            : static_cast<unsigned char>(SampleAt(frame, 0));
        const unsigned char expectedRight = totalFrames < sampleCount ? 0
            : static_cast<unsigned char>(SampleAt(frame, channels == 1 ? 0 : 1));
        if (left[i] != expectedLeft || right[i] != expectedRight) {
            std::printf("FAIL %s (%s, %u channels): sample %d, got %u/%u, expected %u/%u\n",
                name, int16 ? "int16" : "float", static_cast<unsigned>(channels), i,
                static_cast<unsigned>(left[i]), static_cast<unsigned>(right[i]),
                static_cast<unsigned>(expectedLeft), static_cast<unsigned>(expectedRight));
            return false;
        }
    }

    // A second read without new capture data must still return silence.
    GetAudioBuf(left, right, sampleCount);
    for (int i = 0; i < sampleCount; ++i) {
        if (left[i] != 0 || right[i] != 0) {
            std::printf("FAIL %s: drained buffer did not return silence\n", name);
            return false;
        }
    }
    return true;
}
}

int main() {
    struct Case {
        const char* name;
        std::vector<UINT32> packets;
    };
    const Case cases[] = {
        {"underrun", {575}},
        {"exact capacity", {576}},
        {"one frame over capacity", {577}},
        {"large packet", {960}},
        {"twice capacity", {1152}},
        {"over twice capacity", {1153}},
        {"very large packet", {2048}},
        {"two short packets", {288, 288}},
        {"short packets across wrap", {128, 128, 128, 128, 128}},
        {"full then short packets", {576, 144, 144}},
        {"mixed packet sizes", {300, 960, 1, 575, 2048, 288}}
    };

    int failures = 0;
    int checked = 0;
    for (bool int16 : {false, true}) {
        for (WORD channels : {1, 2, 6}) {
            for (const Case& test : cases) {
                ++checked;
                if (!CheckPackets(test.name, test.packets, int16, channels)) {
                    ++failures;
                }
            }
        }
    }
    std::printf("%d/%d audio-buffer cases passed\n", checked - failures, checked);
    return failures == 0 ? 0 : 1;
}
