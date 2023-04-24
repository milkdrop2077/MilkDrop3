// cleanup.h

class AudioClientStopOnExit {
public:
    AudioClientStopOnExit(IAudioClient *p) : m_p(p) {}
    ~AudioClientStopOnExit() {
        HRESULT hr = m_p->Stop();
        if (FAILED(hr)) {
            ERR(L"IAudioClient::Stop failed: hr = 0x%08x", hr);
        }
    }

private:
    IAudioClient *m_p;
};

class AvRevertMmThreadCharacteristicsOnExit {
public:
    AvRevertMmThreadCharacteristicsOnExit(HANDLE hTask) : m_hTask(hTask) {}
    ~AvRevertMmThreadCharacteristicsOnExit() {
        if (!AvRevertMmThreadCharacteristics(m_hTask)) {
            ERR(L"AvRevertMmThreadCharacteristics failed: last error is %d", GetLastError());
        }
    }
private:
    HANDLE m_hTask;
};

class CancelWaitableTimerOnExit {
public:
    CancelWaitableTimerOnExit(HANDLE h) : m_h(h) {}
    ~CancelWaitableTimerOnExit() {
        if (!CancelWaitableTimer(m_h)) {
            ERR(L"CancelWaitableTimer failed: last error is %d", GetLastError());
        }
    }
private:
    HANDLE m_h;
};

class CloseHandleOnExit {
public:
    CloseHandleOnExit(HANDLE h) : m_h(h) {}
    ~CloseHandleOnExit() {
        if (!CloseHandle(m_h)) {
            ERR(L"CloseHandle failed: last error is %d", GetLastError());
        }
    }

private:
    HANDLE m_h;
};

class CoTaskMemFreeOnExit {
public:
    CoTaskMemFreeOnExit(PVOID p) : m_p(p) {}
    ~CoTaskMemFreeOnExit() {
        CoTaskMemFree(m_p);
    }

private:
    PVOID m_p;
};

class CoUninitializeOnExit {
public:
    ~CoUninitializeOnExit() {
        CoUninitialize();
    }
};

class PropVariantClearOnExit {
public:
    PropVariantClearOnExit(PROPVARIANT *p) : m_p(p) {}
    ~PropVariantClearOnExit() {
        HRESULT hr = PropVariantClear(m_p);
        if (FAILED(hr)) {
            ERR(L"PropVariantClear failed: hr = 0x%08x", hr);
        }
    }

private:
    PROPVARIANT *m_p;
};

class ReleaseOnExit {
public:
    ReleaseOnExit(IUnknown *p) : m_p(p) {}
    ~ReleaseOnExit() {
        m_p->Release();
    }

private:
    IUnknown *m_p;
};

class SetEventOnExit {
public:
    SetEventOnExit(HANDLE h) : m_h(h) {}
    ~SetEventOnExit() {
        if (!SetEvent(m_h)) {
            ERR(L"SetEvent failed: last error is %d", GetLastError());
        }
    }
private:
    HANDLE m_h;
};

class WaitForSingleObjectOnExit {
public:
    WaitForSingleObjectOnExit(HANDLE h) : m_h(h) {}
    ~WaitForSingleObjectOnExit() {
        DWORD dwWaitResult = WaitForSingleObject(m_h, INFINITE);
        if (WAIT_OBJECT_0 != dwWaitResult) {
            ERR(L"WaitForSingleObject returned unexpected result 0x%08x, last error is %d", dwWaitResult, GetLastError());
        }
    }

private:
    HANDLE m_h;
};