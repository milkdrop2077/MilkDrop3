// prefs.cpp

#include "common.h"

#define DEFAULT_FILE L"loopback-capture.wav"

void usage(LPCWSTR exe);
HRESULT get_default_device(IMMDevice **ppMMDevice);
HRESULT list_devices();
HRESULT get_specific_device(LPCWSTR szLongName, IMMDevice **ppMMDevice);
HRESULT open_file(LPCWSTR szFileName, HMMIO *phFile);

void usage(LPCWSTR exe) {
    LOG(
        L"%ls -?\n"
        L"%ls --list-devices\n"
        L"%ls [--device \"Device long name\"] [--file \"file name\"] [--int-16]\n"
        L"\n"
        L"    -? prints this message.\n"
        L"    --list-devices displays the long names of all active playback devices.\n"
        L"    --device captures from the specified device (default if omitted)\n"
        L"    --file saves the output to a file (%ls if omitted)\n"
        L"    --int-16 attempts to coerce data to 16-bit integer format",
        exe, exe, exe, DEFAULT_FILE
    );
}

CPrefs::CPrefs(int argc, LPCWSTR argv[], HRESULT &hr)
: m_pMMDevice(NULL)
, m_hFile(NULL)
, m_bInt16(false)
, m_pwfx(NULL)
, m_szFilename(NULL)
{
    switch (argc) {
        case 2:
            if (0 == _wcsicmp(argv[1], L"-?") || 0 == _wcsicmp(argv[1], L"/?")) {
                // print usage but don't actually capture
                hr = S_FALSE;
                usage(argv[0]);
                return;
            } else if (0 == _wcsicmp(argv[1], L"--list-devices")) {
                // list the devices but don't actually capture
                hr = list_devices();

                // don't actually play
                if (S_OK == hr) {
                    hr = S_FALSE;
                    return;
                }
            }
        // intentional fallthrough
        
        default:
            // loop through arguments and parse them
            for (int i = 1; i < argc; i++) {
                
                // --device
                if (0 == _wcsicmp(argv[i], L"--device")) {
                    if (NULL != m_pMMDevice) {
                        ERR(L"%s", L"Only one --device switch is allowed");
                        hr = E_INVALIDARG;
                        return;
                    }

                    if (i++ == argc) {
                        ERR(L"%s", L"--device switch requires an argument");
                        hr = E_INVALIDARG;
                        return;
                    }

                    hr = get_specific_device(argv[i], &m_pMMDevice);
                    if (FAILED(hr)) {
                        return;
                    }

                    continue;
                }

                // --file
                if (0 == _wcsicmp(argv[i], L"--file")) {
                    if (NULL != m_szFilename) {
                        ERR(L"%s", L"Only one --file switch is allowed");
                        hr = E_INVALIDARG;
                        return;
                    }

                    if (i++ == argc) {
                        ERR(L"%s", L"--file switch requires an argument");
                        hr = E_INVALIDARG;
                        return;
                    }

                    m_szFilename = argv[i];
                    continue;
                }

                // --int-16
                if (0 == _wcsicmp(argv[i], L"--int-16")) {
                    if (m_bInt16) {
                        ERR(L"%s", L"Only one --int-16 switch is allowed");
                        hr = E_INVALIDARG;
                        return;
                    }

                    m_bInt16 = true;
                    continue;
                }

                ERR(L"Invalid argument %ls", argv[i]);
                hr = E_INVALIDARG;
                return;
            }

            // open default device if not specified
            if (NULL == m_pMMDevice) {
                hr = get_default_device(&m_pMMDevice);
                if (FAILED(hr)) {
                    return;
                }
            }

            if (NULL != m_szFilename) {
                // if no filename specified, use default
                if (NULL == m_szFilename) {
                    m_szFilename = DEFAULT_FILE;
                }

                // open file
                hr = open_file(m_szFilename, &m_hFile);
                if (FAILED(hr)) {
                    return;
                }
            }
    }
}

CPrefs::~CPrefs() {
    if (NULL != m_pMMDevice) {
        m_pMMDevice->Release();
    }

    if (NULL != m_hFile) {
        mmioClose(m_hFile, 0);
    }

    if (NULL != m_pwfx) {
        CoTaskMemFree(m_pwfx);
    }
}

HRESULT get_default_device(IMMDevice **ppMMDevice) {
    HRESULT hr = S_OK;
    IMMDeviceEnumerator *pMMDeviceEnumerator;

    // activate a device enumerator
    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, 
        __uuidof(IMMDeviceEnumerator),
        (void**)&pMMDeviceEnumerator
    );
    if (FAILED(hr)) {
        ERR(L"CoCreateInstance(IMMDeviceEnumerator) failed: hr = 0x%08x", hr);
        return hr;
    }
    ReleaseOnExit releaseMMDeviceEnumerator(pMMDeviceEnumerator);

    // get the default render endpoint
    hr = pMMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, ppMMDevice);
    if (FAILED(hr)) {
        ERR(L"IMMDeviceEnumerator::GetDefaultAudioEndpoint failed: hr = 0x%08x", hr);
        return hr;
    }

    return S_OK;
}

HRESULT list_devices() {
    HRESULT hr = S_OK;

    // get an enumerator
    IMMDeviceEnumerator *pMMDeviceEnumerator;

    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, 
        __uuidof(IMMDeviceEnumerator),
        (void**)&pMMDeviceEnumerator
    );
    if (FAILED(hr)) {
        ERR(L"CoCreateInstance(IMMDeviceEnumerator) failed: hr = 0x%08x", hr);
        return hr;
    }
    ReleaseOnExit releaseMMDeviceEnumerator(pMMDeviceEnumerator);

    IMMDeviceCollection *pMMDeviceCollection;

    // get all the active render endpoints
    hr = pMMDeviceEnumerator->EnumAudioEndpoints(
        eRender, DEVICE_STATE_ACTIVE, &pMMDeviceCollection
    );
    if (FAILED(hr)) {
        ERR(L"IMMDeviceEnumerator::EnumAudioEndpoints failed: hr = 0x%08x", hr);
        return hr;
    }
    ReleaseOnExit releaseMMDeviceCollection(pMMDeviceCollection);

    UINT count;
    hr = pMMDeviceCollection->GetCount(&count);
    if (FAILED(hr)) {
        ERR(L"IMMDeviceCollection::GetCount failed: hr = 0x%08x", hr);
        return hr;
    }
    LOG(L"Active render endpoints found: %u", count);

    for (UINT i = 0; i < count; i++) {
        IMMDevice *pMMDevice;

        // get the "n"th device
        hr = pMMDeviceCollection->Item(i, &pMMDevice);
        if (FAILED(hr)) {
            ERR(L"IMMDeviceCollection::Item failed: hr = 0x%08x", hr);
            return hr;
        }
        ReleaseOnExit releaseMMDevice(pMMDevice);

        // open the property store on that device
        IPropertyStore *pPropertyStore;
        hr = pMMDevice->OpenPropertyStore(STGM_READ, &pPropertyStore);
        if (FAILED(hr)) {
            ERR(L"IMMDevice::OpenPropertyStore failed: hr = 0x%08x", hr);
            return hr;
        }
        ReleaseOnExit releasePropertyStore(pPropertyStore);

        // get the long name property
        PROPVARIANT pv; PropVariantInit(&pv);
        hr = pPropertyStore->GetValue(PKEY_Device_FriendlyName, &pv);
        if (FAILED(hr)) {
            ERR(L"IPropertyStore::GetValue failed: hr = 0x%08x", hr);
            return hr;
        }
        PropVariantClearOnExit clearPv(&pv);

        if (VT_LPWSTR != pv.vt) {
            ERR(L"PKEY_Device_FriendlyName variant type is %u - expected VT_LPWSTR", pv.vt);
            return E_UNEXPECTED;
        }

        LOG(L"    %ls", pv.pwszVal);
    }    
    
    return S_OK;
}

HRESULT get_specific_device(LPCWSTR szLongName, IMMDevice **ppMMDevice) {
    HRESULT hr = S_OK;

    *ppMMDevice = NULL;
    
    // get an enumerator
    IMMDeviceEnumerator *pMMDeviceEnumerator;

    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, 
        __uuidof(IMMDeviceEnumerator),
        (void**)&pMMDeviceEnumerator
    );
    if (FAILED(hr)) {
        ERR(L"CoCreateInstance(IMMDeviceEnumerator) failed: hr = 0x%08x", hr);
        return hr;
    }
    ReleaseOnExit releaseMMDeviceEnumerator(pMMDeviceEnumerator);

    IMMDeviceCollection *pMMDeviceCollection;

    // get all the active render endpoints
    hr = pMMDeviceEnumerator->EnumAudioEndpoints(
        eRender, DEVICE_STATE_ACTIVE, &pMMDeviceCollection
    );
    if (FAILED(hr)) {
        ERR(L"IMMDeviceEnumerator::EnumAudioEndpoints failed: hr = 0x%08x", hr);
        return hr;
    }
    ReleaseOnExit releaseMMDeviceCollection(pMMDeviceCollection);

    UINT count;
    hr = pMMDeviceCollection->GetCount(&count);
    if (FAILED(hr)) {
        ERR(L"IMMDeviceCollection::GetCount failed: hr = 0x%08x", hr);
        return hr;
    }

    for (UINT i = 0; i < count; i++) {
        IMMDevice *pMMDevice;

        // get the "n"th device
        hr = pMMDeviceCollection->Item(i, &pMMDevice);
        if (FAILED(hr)) {
            ERR(L"IMMDeviceCollection::Item failed: hr = 0x%08x", hr);
            return hr;
        }
        ReleaseOnExit releaseMMDevice(pMMDevice);

        // open the property store on that device
        IPropertyStore *pPropertyStore;
        hr = pMMDevice->OpenPropertyStore(STGM_READ, &pPropertyStore);
        if (FAILED(hr)) {
            ERR(L"IMMDevice::OpenPropertyStore failed: hr = 0x%08x", hr);
            return hr;
        }
        ReleaseOnExit releasePropertyStore(pPropertyStore);

        // get the long name property
        PROPVARIANT pv; PropVariantInit(&pv);
        hr = pPropertyStore->GetValue(PKEY_Device_FriendlyName, &pv);
        if (FAILED(hr)) {
            ERR(L"IPropertyStore::GetValue failed: hr = 0x%08x", hr);
            return hr;
        }
        PropVariantClearOnExit clearPv(&pv);

        if (VT_LPWSTR != pv.vt) {
            ERR(L"PKEY_Device_FriendlyName variant type is %u - expected VT_LPWSTR", pv.vt);
            return E_UNEXPECTED;
        }

        // is it a match?
        if (0 == _wcsicmp(pv.pwszVal, szLongName)) {
            // did we already find it?
            if (NULL == *ppMMDevice) {
                *ppMMDevice = pMMDevice;
                pMMDevice->AddRef();
            } else {
                ERR(L"Found (at least) two devices named %ls", szLongName);
                return E_UNEXPECTED;
            }
        }
    }
    
    if (NULL == *ppMMDevice) {
        ERR(L"Could not find a device named %ls", szLongName);
        return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
    }

    return S_OK;
}

HRESULT open_file(LPCWSTR szFileName, HMMIO *phFile) {
    MMIOINFO mi = {0};

    *phFile = mmioOpenW(
        // some flags cause mmioOpen write to this buffer
        // but not any that we're using
        const_cast<LPWSTR>(szFileName),
        &mi,
        MMIO_WRITE | MMIO_CREATE
    );

    if (NULL == *phFile) {
        ERR(L"mmioOpen(\"%ls\", ...) failed. wErrorRet == %u", szFileName, mi.wErrorRet);
        return E_FAIL;
    }

    return S_OK;
}
