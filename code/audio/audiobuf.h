// audiobuf.h

#include <mutex>
#include <stdint.h> // For uint32_t

// Define BYTE if not on Windows, where it's usually defined in windows.h
#ifndef _WIN32
typedef unsigned char BYTE;
#else
 // On Windows, windows.h (included indirectly or directly by common.h) should provide BYTE
 // If common.h is also modified to not include windows.h directly, ensure BYTE is defined.
 // For now, assuming common.h or another header pulls in windows.h for _WIN32 builds.
 // If errors occur, explicitly include <windows.h> or define BYTE here for Windows too.
#include <windows.h> // This is to ensure BYTE and UINT32 are defined for Windows if not already.
                     // Ideally, this windows.h include would be managed better.
#endif


// Reset audio buffer discarding stored audio data
void ResetAudioBuf();

// Return previously saved audio data for visualizer
void GetAudioBuf(unsigned char *pWaveL, unsigned char *pWaveR, int SamplesCount);

// Save audio data for visualizer
// Parameters previously in WAVEFORMATEX are now passed directly.
// is_float indicates if pData is float (true) or int16_t (false)
// nNumFramesToRead is the number of audio frames (a frame contains samples for all channels)
void SetAudioBuf(const unsigned char *pData, const uint32_t nNumFramesToRead, int num_channels, int bits_per_sample, bool is_float);
