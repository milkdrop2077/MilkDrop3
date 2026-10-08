# Audio-buffer regression tests

These tests compile the checked-in `audio/audiobuf.cpp` directly, without the
visualizer or DirectX SDK. Use CMake, Visual Studio's C++ tools and the Windows SDK:

```powershell
cmake -S code/tests -B ../milkdrop-audio-tests -G "Visual Studio 17 2022" -A Win32
cmake --build ../milkdrop-audio-tests --config Debug
ctest --test-dir ../milkdrop-audio-tests -C Debug --output-on-failure
```

The cases check chronological samples for packets below, at and above the
576-frame capacity, including consecutive packets and wraparound. They exercise
float and 16-bit PCM with mono, stereo and six-channel inputs, plus underrun and
drained-buffer silence. Multichannel input uses the first two channels, matching
the existing visualizer behavior.

The public source is an old alpha, as the maintainer explains in
[issue #63](https://github.com/milkdrop2077/MilkDrop3/issues/63#issuecomment-2041618757).
These tests do not validate the current distributed application or live WASAPI
capture.
