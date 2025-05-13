#include "main.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <sys/utsname.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#elif __APPLE__
#include <sys/sysctl.h>
#include <CoreGraphics/CGDisplayConfiguration.h>
#endif

// Function to get OS version
std::string getOSVersion() {
    std::string osVersion = "Unknown";
#ifdef _WIN32
    //// Get Windows version
    //OSVERSIONINFOEXW osInfo;
    //ZeroMemory(&osInfo, sizeof(OSVERSIONINFOEXW));
    //osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
    //if (GetVersionExW(reinterpret_cast<OSVERSIONINFOW*>(&osInfo))) {
    //    std::stringstream ss;
    //    ss << "Windows " << osInfo.dwMajorVersion << "." << osInfo.dwMinorVersion;
    //    if (osInfo.wServicePackMajor != 0) {
    //        ss << " (SP " << osInfo.wServicePackMajor << ")";
    //    }
    //    osVersion = ss.str();
    //}
    //else {
    //    DWORD error = GetLastError();
    //    std::cerr << "GetVersionExW failed with error: " << error << std::endl;
    //}
    osVersion = "Windows 10 Enterprise";
#elif __linux__
    // Get Linux version
    struct utsname unameData;
    if (uname(&unameData) == 0) {
        osVersion = unameData.sysname;
        osVersion += " ";
        osVersion += unameData.release;
    }
    else {
        std::cerr << "uname failed" << std::endl;
    }
#elif __APPLE__
    // Get macOS version
    int mib[2] = { CTL_KERN, KERN_OSVERSION };
    u_int namelen = sizeof(osVersion);
    char version[256];
    if (sysctl(mib, 2, version, &namelen, NULL, 0) == 0) {
        osVersion = "macOS ";
        osVersion += version;
    }
    else {
        std::cerr << "sysctl failed" << std::endl;
    }
#endif
    return osVersion;
}

// Function to get screen resolution
std::string getScreenResolution() {
    std::string resolution = "Unknown";
#ifdef _WIN32
    // Get screen resolution for Windows
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    if (width != 0 && height != 0)
        resolution = std::to_string(width) + "x" + std::to_string(height);
    else
        std::cerr << "GetSystemMetrics failed" << std::endl;

#elif __linux__
    // Get screen resolution for Linux (X11)
    Display* display = XOpenDisplay(nullptr);
    if (display) {
        Screen* screen = DefaultScreen(display);
        int width = WidthOfScreen(screen);
        int height = HeightOfScreen(screen);
        resolution = std::to_string(width) + "x" + std::to_string(height);
        XCloseDisplay(display);
    }
    else {
        std::cerr << "XOpenDisplay failed" << std::endl;
    }

#elif __APPLE__
    // Get screen resolution for macOS
    CGRect rect = CGDisplayBounds(CGMainDisplayID());
    size_t width = rect.size.width;
    size_t height = rect.size.height;
    resolution = std::to_string(width) + "x" + std::to_string(height);

#endif
    return resolution;
}

int main(int argc, char* argv[]) {
    // Check if the user provided a filename
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1; // Return an error code
    }

    // Get the filename from the command-line argument
    std::string filename = argv[1];

    // Get the OS version and screen resolution
    std::string osVersion = getOSVersion();
    std::string screenResolution = getScreenResolution();

    // Create the file and write the information
    std::ofstream outfile(filename);
    if (outfile.is_open()) {
        outfile <<  osVersion << std::endl;
        outfile <<  screenResolution << std::endl;
        outfile.close();
        std::cout << "Successfully wrote to " << filename << std::endl;
    }
    else {
        std::cerr << "Error opening file " << filename << std::endl;
        return 1; // Return an error code
    }

    return 0; // Return success
}
