// common.h

#include <stdio.h> // Standard I/O, generally cross-platform

#ifdef _WIN32
// Windows-specific headers
#include <windows.h>
#include <mmsystem.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <avrt.h>
#include <functiondiscoverykeys_devpkey.h>
#endif // _WIN32

// Cross-platform or project-specific headers
#include <mutex> // C++ standard mutex, cross-platform

#include "log.h"
#include "cleanup.h" // Review cleanup.h for Windows dependencies if any
#include "prefs.h"   // Review prefs.h for Windows dependencies if any
#include "loopback-capture.h"
#include "audiobuf.h"
