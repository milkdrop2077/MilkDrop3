// common.h

#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <avrt.h>
#include <functiondiscoverykeys_devpkey.h>
#include <mutex>

#include "log.h"
#include "cleanup.h"
#include "prefs.h"
#include "loopback-capture.h"
#include "audiobuf.h"
