import ctypes
import os
import platform
import argparse
import io
import math
import shutil
import random
import string
import sys


DEFAULT_WINPATH = 'C:\\Users'
DEFAULT_LINPATH = '/home'
USER_FOLDERS = ['Desktop', 'Documents']
GENERIC_FOLDERS = ['Reports', 'Archive', 'Team Files', 'Presentations', 'Governance', 'SOPs', 'Templates', 'Documents']
FOLDER_NAMES = USER_FOLDERS

OS = 'Windows'
ROOT_DIR = None

TEXT_FILE_NAMES = ['Report', 'Statistics', 'Analysis', 'Notes', 'Findings', 'Whitepaper']
TEXT_FILE_EXTENSIONS = ['.doc', '.docx', '.rtf', '.pdf']

PRESENTATION_FILE_NAMES = ['Quarterly Update', 'Roadmap', 'Master Schedule', 'Program Overview']
PRESENTATION_FILE_EXTENSIONS = ['.ppt', '.pptx']

EXCEL_FILE_NAMES = ['Statistics', 'Budget', 'Staff Allocations', 'Inventory']
EXCEL_FILE_EXTENSIONS = ['.xls', '.xlsx']

KNOWN_EXTENSIONS = ['.doc', '.docx', '.rtf', '.pdf', '.ppt', '.pptx', '.xls', '.xlsx']

PROMPT = False
RECURSE = False
LOG = False

SEED = None
SUFFIX = None


def is_admin():
    """Return privileged status"""
    try:
        is_admin = (os.getuid() == 0)
    except AttributeError:
        is_admin = ctypes.windll.shell32.IsUserAnAdmin() != 0
        
    return is_admin
    
def validate_path(path):
    """ Validate if a given path exists """
    exists = False
    try:
        exists = os.path.exists(path)
    except Exception:
        pass

    return exists

def validate_root_directory(path):
    """Validate the supplied root directory, exit on fail"""
    if not validate_path(path):
        if not create_directory(path):
            print("[!] The supplied directory ({0}) is not valid, or was not created. Exiting.".format(path))
            sys.exit(1)

    print('[OK] Validated base path: {0}'.format(path))
    
def get_root_directory(path):
    """Return a default path if the supplied path is not valid"""
    global OS
    if path == "default":
        if OS == 'Windows':
            return DEFAULT_WINPATH
        else:
            return DEFAULT_LINPATH
    else:
        return path

def create_directory(path):
    """
    Attempt to create a directory at the supplied path. If the global PROMPT variable is true, first 
    solicits user confirmation.
    """
    user_input = ""
    
    if PROMPT:
        print('\nThe supplied directory does not exist. Would you like to create it? ', end='(Y/n): ')
        user_input = input()
    
    if user_input.lower() == 'y' or not PROMPT:
        try:
            os.mkdir(path)
            print('[OK] Created directory: {0}'.format(os.path.abspath(path)))
            return True
        except OSError as e:
            print('[!] Something went wrong when creating {0}:'.format(os.path.abspath(path)))
            print(e)
            return False
    else:
        return False

def validate_templates(path):
    """
    Validates the templates folder includes the expected templates. File names are agnostic, as long 
    as the known file extensions are present. Other extensions will be ignored later.
    """
    global KNOWN_EXTENSIONS

    files = [os.path.abspath(f) for f in os.scandir(path) if f.is_file()]
    extensions = [os.path.splitext(f)[1] for f in files]
    
    comparison = set(extensions)&set(KNOWN_EXTENSIONS)
    if len(comparison) == len(KNOWN_EXTENSIONS):
        return True
    else:
        print('\n[!] The templates folder does not match the known extensions list completely. Found: \n\t{0}'.format(' '.join(extensions)))
        user_input = input("[?] Are you sure you want to use the files in the specified folder (%s)? (y/N) " % (path))

        if user_input.lower() == 'y':
            KNOWN_EXTENSIONS = extensions
            print('[OK] Updating templates')
            return True
        else:
            print('[!] FATAL - Templates folder does not contain the expected templates. Exiting.')
            sys.exit(1)

def validate_template_directory(path):
    """Validates that the template directory exists in an expected state. Exits on fail."""

    if not validate_path(path):
        print('[!] FATAL - The template directory ({0}) was not found. Exiting.'.format(os.path.abspath(path)))
        sys.exit(1)

    if not validate_templates(path):
        print('[!] FATAL - Could not find the expected templates in the supplied folder ({0}). Exiting.'
              .format(os.path.abspath(path)))
        sys.exit(1)

    print('[OK] Templates found')
    print('[*] Using the following file types (any others in the folder will be ignored): \n\t{0}'
          .format(' '.join(KNOWN_EXTENSIONS)))

def get_template_directory(path):
    """
    If the default directory is used, returns the absolute path based on the script or
    executable's location to avoid using the user's CWD. Otherwise, returns the user's
    supplied path.
    """

    if path == "templates":
        # https://stackoverflow.com/a/404750
        if getattr(sys, 'frozen', False):
            app_path = os.path.dirname(sys.executable)
        elif __file__:
            app_path = os.path.dirname(__file__)
        template_path = os.path.join(app_path, "templates")
        return template_path
    else:
        return path

def distribute(base_path, template_folder, file_count):
    """
    Orchestrates file distribution activities, looping through available sub-directories at the 
    supplied base path and Documents/Desktop folders, dividing the number of files to create 
    accordingly. In the event child folders cannot be created, will use the base path instead.

    Parameters
    ----------
    base_path : string
        The root directory into which to recurse for file distribution
    template_folder : string
        The path to the templates directory
    file_count : int
        The total number of files to distribute
    """
    child_directories = [base_path]

    if RECURSE:
        try:
            child_directories = get_child_directories(base_path)
        except PermissionError:
            print('|--[!] FATAL - Unable to get the child directories of {0}: Permissions error. Exiting.'.format(os.path.abspath(base_path)))
            sys.exit(1)
        
        print('|--[*] Found {0} sub-directories in base path'.format(str(len(child_directories))))
    
    if len(child_directories) == 0 or not RECURSE:
        print('|--[*] Using the base path')
        child_directories = [os.path.abspath(base_path)]
        files_per_folder = math.floor(file_count / len(FOLDER_NAMES))
    else:
        files_per_folder = math.floor(file_count / (len(child_directories) + len(FOLDER_NAMES)))

    templates = get_templates(template_folder)
    print('|--[*] Obtained templates')

    
    for dir in child_directories:
        print('|\n|--[*] Using directory: {0}'.format(dir))
        try:
            sub_directories = get_child_directories(dir)
        except PermissionError:
            if not RECURSE:
                print('|--[!] Unable to get the child directories of {0}. Skipping.'.format(dir))
                continue
            else:
                print('|--[!] FATAL - could not read the base path folder. Exiting.')
                sys.exit(1)
        
        # if cannot create or write to either Documents and Desktop, try to write to base folder instead
        use_base_directory = True
        
        for folder in FOLDER_NAMES:
            print('|\n|----[*] Looking for {0}'.format(folder))
            index = get_folder_index(folder, sub_directories)
            
            if index < 0:
                print('|----[*] {0} not found, attempting to create'.format(folder))
                folder_path = dir + '/' + folder
                print('|----', end="")
                success = create_directory(folder_path)
                
                if success:
                    sub_directories = get_child_directories(dir)
                    index = get_folder_index(folder, sub_directories)
                else:
                    print('|----[*] Moving to next folder')
                    continue

            folder_path = sub_directories[index]
            try:
                print('|----[*] Copying files to {0}'.format(folder_path))
                write_count = copy_files(folder_path, files_per_folder, templates)
                print('|----[OK] Wrote {0} files to directory'.format(write_count))
                use_base_directory = False
            except IOError as e:
                print('|----[!] Failed to write to {0}:'.format(folder_path))

        if use_base_directory:
            try:
                print('|----[*] Recursion not selected or could not write to desired folders. Trying base path.')
                print('|----[*] Copying files to {0}'.format(dir))
                write_count = copy_files(dir, files_per_folder, templates)
                print('|----[OK] Wrote {0} files to directory'.format(write_count))
                use_base_directory = False
            except IOError as e:
                print('|----[!] Failed to write to {0}:'.format(folder_path))
            

def get_child_directories(path):
    """ Returns the child directories for a specified path"""
    directories = []
    try:
        directories = [d.path for d in os.scandir(path) if d.is_dir()]
    except PermissionError:
        raise

    return directories

def get_templates(template_folder):
    """
    Obtains the template files from the specified folder, returns a dict object using
    the file types as keys with the absolute paths as values.

    Parameters
    ----------
    template_folder : string
        Path to the templates directory
    """
    files = {os.path.splitext(f)[1]: os.path.abspath(f) for f in os.scandir(template_folder) 
             if f.is_file() and os.path.splitext(f)[1] in KNOWN_EXTENSIONS}

    return files
    
def get_folder_index(folder, sub_directories):
    """
    Looks for the specified directory name in the provided list of sub-directories and returns 
    its index if found. Otherwise, returns -1.

    Parameters
    ----------
    folder : string
        The target directory name
    sub_directories : list
        List containing the absolute paths for a given directory's child directories
    """
    if len(sub_directories):
        for i,s in enumerate(sub_directories):
            if folder in os.path.basename(s):
                return i
    
    return -1
    
def copy_files(folder_path, file_count, templates):
    """
    Loops through the available templates copying the requested amount to the destination. 
    Adds a random 6-character string to each of the selected file names to ensure it does 
    not overwrite prior copies.

    Parameters
    ----------
    folder_path : string
        The destination path for files
    file_count : int
        The number of files to copy to this directory
    templates : dict
        A dict containing the templates in {extension: path} format
    """
    write_count = 0
    rand = random
    if SEED:
        rand.seed(SEED)

    for i in range(file_count):
        extension = KNOWN_EXTENSIONS[i % len(KNOWN_EXTENSIONS)]
        template = templates[extension]
        file_name = get_file_name(extension, rand)
        dest_path = os.path.join(folder_path, file_name)
        
        try:
            shutil.copy(template, dest_path)
            write_count += 1

            if LOG:
                try:
                    with open(os.path.join(os.path.abspath(ROOT_DIR), "__log.txt"), "a") as f:
                        f.write(dest_path + "\n")
                except IOException as e:
                    print(e)
                    continue

        except IOException as e:
            print(e)
            continue
    
    if write_count == 0:
        raise IOError

    return write_count

def get_file_name(extension, rand):
    """
    Returns a file name appropriate to the supplied extension type.
    
    Parameters
    ----------
    extension : string
        The selected template's file extension
    rand : random
        A Random object. May or may not be initialized with a seed based on global configuration choices.
    """

    if extension in TEXT_FILE_EXTENSIONS:
        file_name = rand.choice(TEXT_FILE_NAMES)
    elif extension in PRESENTATION_FILE_EXTENSIONS:
        file_name = rand.choice(PRESENTATION_FILE_NAMES)
    elif extension in EXCEL_FILE_EXTENSIONS:
        file_name = rand.choice(EXCEL_FILE_NAMES)

    file_name += '_' + (''.join(rand.choices(string.ascii_letters, k=6))) #+ extension

    if SUFFIX:
        file_name += '_' + SUFFIX

    file_name += extension

    return file_name

def main():
    parser = argparse.ArgumentParser(description="Distribute template files for testing. Takes a templates folder and "
                                     + "a starting folder (e.g., C:\\Users). Recurses through the base directory and "
                                     + "uses/creates specified folders (-f) as file destinations. Files counts are evenly "
                                     + "split across the folders. Final file count may be slightly off due to rounding. "
                                     + "Files will all appear in the format <generic_name>_<random/seed string>[_<optional_suffix].")
    parser.add_argument('-d', '--directory', 
                        help="The starting directory for file dispersing. Will recurse beneath this. "
                        + "Defaults - Windows: C:\\Users\\Public, Linux: /home/",
                        default="default", type=str)
    parser.add_argument('-t', '--templatefolder', 
                        help="The directory containing the file templates. Default: current directory/templates",
                        default="templates", type=str)
    parser.add_argument('-c', '--count', help="The total number of files to distribute. Default: 100", type=int,
                       default=100)
    parser.add_argument('-s', '--seed', help="String seed for random file name generation. "
                       + "Do not use 'none' as that will cause no seed to be used.",
                       type=str)
    parser.add_argument('-f', '--foldernames', help="'user' or 'generic'. 'user' creates a Desktop and Documents folder "
                       + "in the root directory (if they don't already exist), 'generic' uses a list of basic folder " 
                       + "names to create more folders. Default: user", type=str, default="user")
    parser.add_argument('-x', '--suffix', help="Supply a string that will be appended to every created file. Useful for "
                       + "cleanup actions after the data is no longer needed or to ensure only created files are staged.", type=str)
    parser.add_argument('--log', help="Writes a log file to the base path containing the names of all created files.",
                        action='store_true')
    parser.add_argument('--recurse', help="Sets whether to recurse through base path children. The selected folder "
                       + "name set will be used in EACH child directory of the base path. E.g. base path = c:\\users\\, "
                       + "program will attempt to create Desktop and Documents for each user folder found. It will not "
                       + "recurse further than that (e.g., it won't try to create folders in c:\\users\\testuser\\downloads).",
                       action='store_true')
    parser.add_argument('--prompt', help="Causes the program to prompt for folder creation before creating.", 
                        action='store_true')
    args = parser.parse_args()
    

    print("[*] FILE NAMING AND TRACKING")
    print("----------------------------")
    global FOLDER_NAMES
    match args.foldernames.lower():
        case "generic":
            global GENERIC_FOLDERS
            FOLDER_NAMES = GENERIC_FOLDERS
            print("[*] Folder name set: GENERIC")
        case "user":
            print("[*] Folder name set: USER")
        case _:
            print("[!] Unrecognized input. Using folder name set: USER")


    if args.seed:
        global SEED
        SEED = args.seed
        print('[*] Seed: {0}'.format(SEED))
    else:
        print('[*] Seed: NONE')

    if args.suffix:
        global SUFFIX
        SUFFIX = args.suffix
        print('[*] File Name Suffix: {0}'.format(SUFFIX))
    else:
        print('[*] File Name Suffix: NONE')

    global LOG
    LOG = args.log
    if LOG:
        print("[*] Log created file names: ENABLED")
    else:
        print("[*] Log created file names: DISABLED")



    print("\n[*] OTHER SETTINGS AND STATUS")
    print("-----------------------------")
    global PROMPT 
    PROMPT = args.prompt
    if PROMPT:
        print("[*] Prompt for directory creation: ENABLED")

    global RECURSE
    RECURSE = args.recurse
    if RECURSE:
        print("[*] Folder Recursion: ENABLED")      
    
    global OS 
    OS = platform.system()
    
    if not is_admin():
        print("[!] WARNING - If the supplied root directory includes sensitive locations, those locations will fail")
    else:
        print('[OK] Running as {0}'.format('Administrator' if OS == 'Windows' else 'root'))
    
    global ROOT_DIR    
    ROOT_DIR = get_root_directory(args.directory)
    validate_root_directory(ROOT_DIR)
    
    template_dir = get_template_directory(args.templatefolder)
    validate_template_directory(template_dir)
        
    print('\n[*] Beginning distribution')
    file_count = args.count
    distribute(ROOT_DIR, template_dir, file_count)
    
    print('\n[*] Program complete. Exiting.\n')
    
if __name__ == "__main__":
    main()
