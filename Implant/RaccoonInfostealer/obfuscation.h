#pragma once
#include "vars.h"
#include "functions.h"

int debofuscateStrings();
string decodeB64(const string& input);
bool decryptRC4(wstring& encData, const string& key);
