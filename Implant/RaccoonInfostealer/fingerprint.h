#pragma once
#include "CPUID.h"
#include "vars.h"
#include "functions.h"
#include "utils.h"
#include "comms.h"

#include <intrin.h>
#include <codecvt>

using namespace std;

int fingerprint();

wstring getScreenSize();
vector<wstring> getDisplayAdapters();
wstring getArchitecture();
wstring getTotalMemory();
wstring getTimeZone();
wstring getOSVersion();
wstring getProcessorInfo();
wstring getUserLocale();
vector<wstring> getInstalledPrograms();
wstring formatSystemInfo(wstring dim, vector<wstring> adapters, wstring arch,
    wstring mem, wstring tz, wstring os, wstring proc,
    wstring locale, vector<wstring> programs);
