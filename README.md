# Raccoon Infostealer Emulation

## License / Disclaimer
Licensed under the Creative Commons Attribution-NonCommercial 4.0 International Public License

:warning: This program was written to test organzations' environments with behavior that closely mimics a popular infostealer. Any use beyond authorized, controlled security testing is expressly forbidden.

Credits:

@nbaertch

@BCHarrell

## Some Background

As part of a Purple Team exercise targeting data exfiltration, we worked with our internal threat intelligence team to identify a prevelant infostealer that affects organizations like ours in order to emulate it. The team selected Raccoon Infostealer, which conducts a variety of file theft behavior, from generic files to browser data.

We don't (currently) possses reverse engineering in house, so this emulation is based on a variety of threat intelligence articles that discuss known samples of Raccoon. See the docs folder for a rundown of this intelligence.

To save time with the realities of a corporate environment, we omitted some functionality from Raccoon, primarily related to crypto wallet theft.

## Setup
Please be aware that in the interest of time, we didn't add much error handling. This isn't meant for long term use, so ensure your test environment is prepped.

:warning: Raccoon sends data in clear-text over basic HTTP. The browser data is set to send dummy data, but systeminfo and other files will be exposed if you do not provide an alternative. We strongly recommend testing with designated test directories to avoid accidental spills. The included file generation program can help with this (please see the README in HelperPrograms for more information).

### Server Setup and Usage

The server is a bare-bones receptor for exfiltrated files; the only "C2" that occurs is responding to an initial beacon from the implant which returns a configuration file governing file theft and a few other parameters.

The default configuration (Server > config.toml) steals files from the executing user's Desktop and Documents folders, specifically looking for pdf/doc/txt per threat intelligence samples. Ensure you have some files seeded in those directories, or point them to other locations / file types.

You will need port 80 open on the server firewall to faciliate inbound connections. Once the configuration file is ready, ensure you have Go installed and run the following command from the server directory:

```bash
$env:CGO_ENABLED=1; go run .
```

The server will log received data to `raccoon.log`. To extract received files from the database, use the following:

```bash
saveFiles <implantIDfromlog> <desiredoutputdirectory>
#e.g. saveFiles 4b7bcd9fe9694bb97f405692e409b673 .\output
```

### Implant Setup and Usage

The implant is a one-shot execution cycle that requires a server from which to obtain the configuration file. We did not include any commandline flags (such as verbose) and all print statements are commented out by default, so change those over in main / other places if desired.

:bangbang: **You should consider changing the following variables in `vars.cpp` before compiling.** All strings are RC4 encrypted and then Base64 encoded. There's a small helper script included in this repo to aid with this, or you can easily use CyberChef.

* g_C2RC4Key -- This is the key for C2 server decryption
* g_localLowPath -- Raccoon uses local low if it can be resolved, this has a fallback folder (default: c:\temp)
* g_stagingFolder -- Folder name to stage files and downloaded DLLs (default: 202306)
* g_server -- Server IP/hostname (default is "localhost" --> L"TSrVhxZCJ4aH")
* g_port -- Default 80
* g_token -- Used for comms with the server (default:"placeholder")

You can change other variables, such as the internal encryption key, but that will lead to additional work. We've included the original strings so you can check the values in CyberChef for safety if desired.

Some folders to be aware of:

* Raccoon stages in the user's LocalLow folder (`c:\users\<name>\AppData\LocalLow`) for staging. 
	* The implant will create a folder to store the downloaded DLLs and some of the generated files.
	* The implant will drop a batch file (hastalavista.bat) at the end of its execution, which will self-delete

Raccoon cleans up its dropped files as well as itself via the batch file. The batch file will delete the executable (wherever it is), clear the Recycle Bin of all \*.exe files, and then delete itself.

### Using the Code
Trying to follow Raccoon closely from reverse engineering reports introduced some challenges in development. Some functions can be on the lengthy side and there are no classes. We did split the code into smaller CPPs for easier management.

Almost every function has doc comments including the source(s) used to guide implementation (CTI or other articles when it wasn't clear).

Some notes on the files:

* vars -- includes all encrypted/encoded strings
* functions -- includes the function prototypes and the global variables to hold the function pointers
* obfuscation -- includes the functions necessary to decode the strings to keep it out of the way
* comms -- includes all network communication functions
* startuptasks -- includes the initial behavior up to the first checkin to get the configuration file
* fingerprint -- includes all of the basic fingerprinting functions
* theft -- includes the file theft functions and screenshot grabber
* browser -- includes the Chromium browser theft functions
* utils -- An assortment of useful functions employed throughout the program 
	* This header also includes all of the include statements used by Raccoon, which increases compile time slightly
