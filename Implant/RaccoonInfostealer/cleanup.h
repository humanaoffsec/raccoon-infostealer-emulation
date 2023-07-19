#pragma once
#include "functions.h"
#include "vars.h"
#include "utils.h"

bool cleanup(unordered_map<wstring, HMODULE> handles);
int initiateSelfDelete();