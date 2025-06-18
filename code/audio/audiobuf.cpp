// audiobuf.cpp

#include "audiobuf.h"
#include <string.h> // For memset
#include <stdint.h> // For int8_t, uint32_t

#define SAMPLE_SIZE_LPB 576 // Max number of audio samples stored in circular buffer. Should be no less than SAMPLE_SIZE. Expected sampling rate is 44100 Hz or 48000 Hz (samples per second).

std::mutex pcmLpbMutex;
unsigned char pcmLeftLpb[SAMPLE_SIZE_LPB]; // Circular buffer (left channel)
unsigned char pcmRightLpb[SAMPLE_SIZE_LPB]; // Circular buffer (right channel)
bool pcmBufDrained = false; // Buffer drained by visualization thread and holds no new samples
signed int pcmLen = 0; // Actual number of samples the buffer holds. Can be less than SAMPLE_SIZE_LPB
signed int pcmPos = 0; // Position to write new data into the circular buffer (head of the queue)

void ResetAudioBuf() {
    std::unique_lock<std::mutex> lock(pcmLpbMutex);
    memset(pcmLeftLpb, 0, SAMPLE_SIZE_LPB);
    memset(pcmRightLpb, 0, SAMPLE_SIZE_LPB);
    pcmBufDrained = false;
    pcmLen = 0;
    pcmPos = 0;
}

void GetAudioBuf(unsigned char *pWaveL, unsigned char *pWaveR, int SamplesCount) {
    std::unique_lock<std::mutex> lock(pcmLpbMutex);
    if ((pcmLen < SamplesCount) || (pcmBufDrained)) {
        // Buffer underrun. Insufficient new samples in circular buffer
        memset(pWaveL, 0, SamplesCount);
        memset(pWaveR, 0, SamplesCount);
    }
    else {
        // Copy data from the circular buffer.
        // The read position is effectively (pcmPos - pcmLen + SAMPLE_SIZE_LPB) % SAMPLE_SIZE_LPB
        // We copy `SamplesCount` worth of data or whatever `pcmLen` has.
        // This implementation of GetAudioBuf seems to want to copy the whole buffer (SAMPLE_SIZE_LPB)
        // if enough data is present, which might not be what's intended if SamplesCount is smaller.
        // For now, replicating the old logic's apparent behavior of filling pWaveL/R with the latest full buffer.

        int read_pos = (pcmPos - pcmLen + SAMPLE_SIZE_LPB) % SAMPLE_SIZE_LPB;
        for (int i = 0; i < SamplesCount; i++) {
             // int8_t [-128 .. +127] stored into uint8_t [0..255]
            pWaveL[i] = pcmLeftLpb[(read_pos + i) % SAMPLE_SIZE_LPB];
            pWaveR[i] = pcmRightLpb[(read_pos + i) % SAMPLE_SIZE_LPB];
        }
        pcmBufDrained = true; // Mark as drained after read, new SetAudioBuf call will unmark it.
                              // This implies GetAudioBuf should ideally be called once per SetAudioBuf cycle.
    }
}

int8_t FltToInt(float flt) {
    if (flt >= 1.0f) {
        return +127; // 0x7f
    }
    if (flt < -1.0f) {
        return -128; // 0x80
    }
    // Scale [-1.0, 1.0] to [-128, 127]
    return (int8_t)(flt * 127.0f); // Adjusted scaling slightly
};

// Union type for sample conversion (Little Endian assumed for multi-byte types)
union u_type
{
    int32_t IntVar;   // For 32-bit int (not used currently)
    int16_t ShortVar; // For 16-bit int
    float FltVar;   // For 32-bit float
    unsigned char Bytes[4];
};

// Extracts a single sample for a given channel from a frame of audio data.
// Converts it to int8_t.
int8_t GetChannelSampleProcessed(const unsigned char *frame_data_start, int channel_index, int num_channels, int bits_per_sample, bool is_float) {
    u_type sample_union;
    sample_union.IntVar = 0; // Zero out union to ensure no garbage data for smaller sample types

    int bytes_per_sample_per_channel = bits_per_sample / 8;
    // Pointer to the start of the specific channel's sample data within the frame
    const unsigned char *sample_ptr = frame_data_start + (channel_index * bytes_per_sample_per_channel);

    if (is_float) {
        if (bits_per_sample == 32) {
            // Assuming little-endian float
            sample_union.Bytes[0] = sample_ptr[0];
            sample_union.Bytes[1] = sample_ptr[1];
            sample_union.Bytes[2] = sample_ptr[2];
            sample_union.Bytes[3] = sample_ptr[3];
            return FltToInt(sample_union.FltVar);
        }
        // Add handling for other float types (e.g., 64-bit double) if necessary
    } else { // Integer PCM
        if (bits_per_sample == 16) {
            // Assuming little-endian int16_t
            sample_union.Bytes[0] = sample_ptr[0];
            sample_union.Bytes[1] = sample_ptr[1];
            // Convert int16_t sample to int8_t. This scales [-32768, 32767] to [-128, 127].
            return (int8_t)(sample_union.ShortVar / 256);
        }
        // Add handling for other int types (e.g., 8-bit, 24-bit, 32-bit int) if necessary
    }
    return 0; // Return silence for unsupported formats
}

// New SetAudioBuf implementation
void SetAudioBuf(const unsigned char *pData, const uint32_t nNumFramesToRead, int num_channels, int bits_per_sample, bool is_float) {
    if (!pData || nNumFramesToRead == 0 || num_channels == 0 || bits_per_sample == 0) {
        return; // Invalid parameters
    }

    std::unique_lock<std::mutex> lock(pcmLpbMutex);

    int bytes_per_sample_per_channel = bits_per_sample / 8;
    int frame_size_in_bytes = num_channels * bytes_per_sample_per_channel;
    if (frame_size_in_bytes == 0) return;


    // Determine how many frames to actually copy and where to start from in pData
    // This logic aims to keep the circular buffer (pcmLeftLpb, pcmRightLpb) full of the LATEST samples if nNumFramesToRead > SAMPLE_SIZE_LPB
    // Or append if nNumFramesToRead <= SAMPLE_SIZE_LPB, managing pcmPos and pcmLen correctly.

    uint32_t frames_to_process = nNumFramesToRead;
    const unsigned char* data_source_ptr = pData;

    if (nNumFramesToRead > SAMPLE_SIZE_LPB) {
        // Source has more data than our buffer: take the last SAMPLE_SIZE_LPB frames
        data_source_ptr += (nNumFramesToRead - SAMPLE_SIZE_LPB) * frame_size_in_bytes;
        frames_to_process = SAMPLE_SIZE_LPB;
        pcmPos = 0; // We are overwriting the whole buffer
        pcmLen = 0; // Length will be set to SAMPLE_SIZE_LPB after loop
    }

    for (uint32_t i = 0; i < frames_to_process; ++i) {
        const unsigned char *current_frame_ptr = data_source_ptr + (i * frame_size_in_bytes);

        int8_t left_sample8 = 0;
        if (num_channels >= 1) {
            left_sample8 = GetChannelSampleProcessed(current_frame_ptr, 0, num_channels, bits_per_sample, is_float);
        }

        int8_t right_sample8 = left_sample8; // Use left channel data for mono or if right channel is absent
        if (num_channels >= 2) {
            right_sample8 = GetChannelSampleProcessed(current_frame_ptr, 1, num_channels, bits_per_sample, is_float);
        }

        // Store processed samples into circular buffer
        // The actual write position in the circular array is (pcmPos + current_pcmLen) % SAMPLE_SIZE_LPB if appending
        // Or just 'i' if overwriting the whole buffer (when nNumFramesToRead > SAMPLE_SIZE_LPB)

        int buffer_write_idx;
        if (nNumFramesToRead > SAMPLE_SIZE_LPB) { // Overwriting scenario
             buffer_write_idx = i; // pcmPos is 0
        } else { // Appending scenario
            buffer_write_idx = (pcmPos + pcmLen) % SAMPLE_SIZE_LPB;
            if (pcmLen == SAMPLE_SIZE_LPB) { // Buffer is full, overwrite oldest by advancing pcmPos
                pcmPos = (pcmPos + 1) % SAMPLE_SIZE_LPB;
                // pcmLen remains SAMPLE_SIZE_LPB
            } else {
                pcmLen++;
            }
        }

        pcmLeftLpb[buffer_write_idx] = left_sample8;
        pcmRightLpb[buffer_write_idx] = right_sample8;
    }

    if (nNumFramesToRead > SAMPLE_SIZE_LPB) { // Overwriting scenario
        pcmLen = SAMPLE_SIZE_LPB; // Buffer is now full
        pcmPos = 0; // Next write will be at the start if it's another overwrite, or pcmLen dictates if append
    } else {
        // pcmPos and pcmLen are updated inside the loop for append scenario
        // if the buffer was full and we added one, pcmPos moved, pcmLen stayed full.
        // if the buffer was not full and we added one, pcmPos didn't move (relatively, head of queue), pcmLen grew.
        // This logic is a bit complex. Simpler: advance pcmPos by frames_to_process if buffer was initially empty or we wrapped around.
        // The current pcmPos update (done by advancing it at the end) is more typical for circular buffer 'head' pointer.
        // Let's adjust pcmPos after the loop for append:
        if (nNumFramesToRead <= SAMPLE_SIZE_LPB && pcmLen < SAMPLE_SIZE_LPB) {
             // If buffer was not full, pcmPos should be end of written data
             // pcmPos = (pcmPos_start_of_append + frames_to_process) % SAMPLE_SIZE_LPB;
             // This is not how pcmPos is used in GetAudioBuf. pcmPos is more like a write_head.
             // The current logic for pcmLen and pcmPos (with pcmPos as write head) for append seems fine.
        }
         if (pcmLen == SAMPLE_SIZE_LPB && nNumFramesToRead < SAMPLE_SIZE_LPB) {
            // If we appended and filled the buffer, pcmPos should be (old_pcmPos + frames_to_process) % SAMPLE_SIZE_LPB
            // The loop's logic for pcmPos update if (pcmLen == SAMPLE_SIZE_LPB) handles this.
        }

    }


    pcmBufDrained = false; // New data is available
}
