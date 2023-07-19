# enc_rc4.py
Encrypts the input string with a hardcoded key (change for your use case), and outputs the base64 result.

This program will literally encrypt escape sequences (e.g. '\\n') which will likely produce undesired spacing issues within Raccoon when the string is decrypted, so use the actual values inside of double quotes for tabs/new lines.

## Usage
```
enc_rc4.py "something secret"
```

# File Generator
**Note: This program was written by @BCHarrell while at MITRE and published as part of MITRE Engenuity's ATT&CK Evaluations Program, licensed under the Apache 2.0 license. This version of the program has been altered to provide increased flexibility.**

This program takes the templates in the supplied directory (`.\templates` by default) and recreates them in the target location the specified number of times. Included templates include Lorem Ipsum and span a range of file types, but the desired file types can be easily modified in the Python code, or you can supply your own templates (e.g., to test your DLP controls with fake data).

This program should support Linux as well as Windows; provided examples are for Windows. Linux was not tested after the recent modifications.

## Usage

Example 1:

* Creates 500 files in generically named folders within `c:\users\public\test_dir`
* File creations written to a log file in the root directory
* All files contain a suffix of "dummydata" before the extension (e.g., Analysis_RaNdStRiNg_dummydata.docx)
* Uses the default templates directory (colocated with the Python file)

```bash
py .\file_generator.py -d c:\users\public\test_dir -f generic -c 500 --log --suffix "dummydata"
```

Example 2:

* Recursively distributes files to Documents and Desktop folders within `c:\users`, splitting 1000 files evenly (note - some user folders will require elevation or you'll get an error)
* Prompts for folder creation if, for some reason, a Documents or Desktop folder wasn't found within a user folder
* Each folder will contain the exact same file names using `DLPTEST` as the random seed
* Uses an alternative directory for templates containing user-defined files. Note: May prompt to ensure you want to use those folders unless the expected file extensions are all accounted for (or modified within the Python)

```bash
py .\file_generator.py -d c:\users -c 1000 --recurse --log --suffix "fake" --seed "DLPTEST" -t c:\temp\pci_templates --prompt
```

```bash
usage: file_generator.py [-h] [-d DIRECTORY] [-t TEMPLATEFOLDER] [-c COUNT] [-s SEED] [-f FOLDERNAMES] [-x SUFFIX] [--log] [--recurse] [--prompt]

Distribute template files for testing. Takes a templates folder and a starting folder 
(e.g., C:\Users). Recurses through the base directory and uses/creates specified folders (-f) 
as file destinations. Files counts are evenly split across the folders. Final file count may be
slightly off due to rounding. Files will all appear in the format: 
<generic_name>_<random/seed string>[_<optional_suffix].

options:
  -h, --help            Show this help message and exit
  -d DIRECTORY, --directory DIRECTORY
                        The starting directory for file dispersing. Will recurse beneath this. Defaults - Windows: C:\Users\Public, Linux: /home/
  -t TEMPLATEFOLDER, --templatefolder TEMPLATEFOLDER
                        The directory containing the file templates. Default: current directory/templates
  -c COUNT, --count COUNT
                        The total number of files to distribute. Default: 100
  -s SEED, --seed SEED  String seed for random file name generation. Do not use 'none' as that will cause no seed to be used.
  -f FOLDERNAMES, --foldernames FOLDERNAMES
                        'user' or 'generic'. 'user' creates a Desktop and Documents folder in the root directory (if they don't already exist), 'generic'
                        uses a list of basic folder names to create more folders. Default: user
  -x SUFFIX, --suffix SUFFIX
                        Supply a string that will be appended to every created file. Useful for cleanup actions after the data is no longer needed or to
                        ensure only created files are staged.
  --log                 Writes a log file to the base path containing the names of all created files.
  --recurse             Sets whether to recurse through base path children. The selected folder name set will be used in EACH child directory of the base
                        path. E.g. base path = c:\users\, program will attempt to create Desktop and Documents for each user folder found. It will not
                        recurse further than that (e.g., it won't try to create folders in c:\users\testuser\downloads).
  --prompt              Causes the program to prompt for folder creation before creating.
```