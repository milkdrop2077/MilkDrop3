#ifndef MENU_STRINGS_H
#define MENU_STRINGS_H

#ifdef _WIN32
    // On Windows, use the existing resource system
    #include "resource.h" // For original IDS_ defines from .rc file
#else
    // On Linux, use a map-based lookup for essential strings
    #include <map>
    #include <string>
    #include <wchar.h>  // For wcscpy, wcsncpy if used
    #include <locale>   // For wstring_convert
    #include <codecvt>  // For wstring_convert

    // Define IDs that are used in menu.cpp and other places if not available
    // These should ideally match values from resource.h if there's any overlap
    // or be distinct if they are purely for Linux.
    // This is a minimal set; more would be needed for full functionality.
    #define IDS_UNTITLED_MENU_ITEM                          10001
    #define IDS_UNTITLED_MENU                               10002
    #define IDS_SZ_MENU_NAV_TOOLTIP                         10003 // "navigation: ESC: exit, Left Arrow: back, Right Arrow: select, UP/DOWN: change sel"
    #define IDS_ON                                          10004 // "ON"
    #define IDS_OFF                                         10005 // "OFF"
    #define IDS_MENU_LOCK_PRESET                            10006 // "Lock/Unlock Preset (L)" - Example, find real IDS if used
    #define IDS_MENU_RANDOM_PRESET                          10007 // "Random Preset (Spacebar)"
    #define IDS_MENU_NEXT_PRESET                            10008 // "Next Preset (Right Arrow)"
    #define IDS_MENU_PREV_PRESET                            10009 // "Previous Preset (Left Arrow)"
    #define IDS_MENU_EDIT_CUR_PRESET                        10010 // "Edit Current Preset (M)"
    #define IDS_MENU_SELECT_PRESET                          10011 // "Select Preset (L)"
    #define IDS_MENU_SHOW_PRESET_INFO                       10012 // "Show Preset Info (F4)"
    #define IDS_MENU_SHOW_FPS                               10013 // "Show FPS (F5)"
    #define IDS_MENU_SHOW_RATING                            10014 // "Show Rating (F6)"
    #define IDS_MENU_ALWAYS_ON_TOP							10015 // "Always On Top (F7)"

    // Add more string IDs as needed by menu.cpp based on its usage of wasabiApiLangString(IDS_...)

    inline std::map<int, std::wstring>& get_string_table_instance() {
        static std::map<int, std::wstring> table;
        if (table.empty()) {
            table[IDS_UNTITLED_MENU_ITEM] = L"<untitled menu item>";
            table[IDS_UNTITLED_MENU]      = L"<untitled menu>";
            table[IDS_SZ_MENU_NAV_TOOLTIP]= L"navigation: ESC: exit, Left Arrow: back, Right Arrow: select, UP/DOWN: change sel";
            table[IDS_ON]                 = L"ON";
            table[IDS_OFF]                = L"OFF";
            table[IDS_MENU_LOCK_PRESET]   = L"Lock/Unlock Preset (Scroll Lock)"; // Adjusted key
            table[IDS_MENU_RANDOM_PRESET] = L"Random Preset (Spacebar)";
            table[IDS_MENU_NEXT_PRESET]   = L"Next Preset (Right Arrow)";
            table[IDS_MENU_PREV_PRESET]   = L"Previous Preset (Left Arrow)";
            table[IDS_MENU_EDIT_CUR_PRESET] = L"Edit Current Preset (M)";
            table[IDS_MENU_SELECT_PRESET] = L"Select Preset (L)";
            table[IDS_MENU_SHOW_PRESET_INFO] = L"Show Preset Info (F4)";
            table[IDS_MENU_SHOW_FPS]      = L"Show FPS (F5)";
            table[IDS_MENU_SHOW_RATING]   = L"Show Rating (F6)";
			table[IDS_MENU_ALWAYS_ON_TOP] = L"Always On Top (F7)";
        }
        return table;
    }

    // Convert wstring to string for simple OpenGL rendering (UTF-8)
    inline std::string WstringToUtf8String(const std::wstring& wstr) {
        try {
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
            return converter.to_bytes(wstr);
        } catch (const std::range_error& e) {
            // This can happen if wstr contains characters not representable in UTF-8
            // or if the wstring itself is ill-formed (e.g. lone surrogates).
            // For basic ASCII or common Western European chars, this should be fine.
            // A more robust solution might involve error checking or replacement characters.
            std::string dummy_str;
            for(wchar_t wc : wstr) { // simple lossy conversion
                if (wc < 128) dummy_str += (char)wc; else dummy_str += '?';
            }
            return dummy_str;
        }
    }

    // Wrapper to mimic wasabiApiLangString for Linux
    // Note: The original wasabiApiLangString copies to a buffer.
    // This version returns a const wchar_t* to an internal static wstring.
    // This is simpler if the call site can handle a temporary const wchar_t*.
    // If modification or long-term storage of the buffer is needed by caller,
    // then a version that copies to `buffer` (like the original) would be required.
    inline const wchar_t* LinuxLangStringW(int id) {
        static std::wstring temp_wstring_storage; // Keep string alive for the duration of its use
        auto& table = get_string_table_instance();
        auto it = table.find(id);
        if (it != table.end()) {
            temp_wstring_storage = it->second;
            return temp_wstring_storage.c_str();
        }
        temp_wstring_storage = L"Unknown String ID: " + std::to_wstring(id);
        return temp_wstring_storage.c_str();
    }

    // Version that copies to a provided buffer, similar to original wasabiApiLangString
    inline const wchar_t* LinuxLangString(int id, wchar_t* buffer, int buffer_len) {
        if (!buffer || buffer_len <= 0) return L""; // Or some error indicator

        auto& table = get_string_table_instance();
        auto it = table.find(id);
        if (it != table.end()) {
            wcsncpy(buffer, it->second.c_str(), buffer_len - 1);
            buffer[buffer_len - 1] = L'\0';
            return buffer;
        }

        std::wstring unknown_str = L"Unknown String ID: " + std::to_wstring(id);
        wcsncpy(buffer, unknown_str.c_str(), buffer_len - 1);
        buffer[buffer_len - 1] = L'\0';
        return buffer;
    }

#endif // _WIN32 / Linux
#endif // MENU_STRINGS_H
