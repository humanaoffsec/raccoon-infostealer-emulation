#pragma once
#include "vars.h"
#include "functions.h"
#include "utils.h"

using namespace std;

vector<char> sendRequest(LPCWSTR requestType, vector<char> bytes, wstring headers,
    int port, wstring host, wstring endpoint);

int sendMultipartUpload(vector<char> fileBytes, wstring filename, wstring token);

vector<char> downloadFile(wstring endpoint);