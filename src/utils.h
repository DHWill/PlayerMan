#ifndef OSTOOLS_H
#define OSTOOLS_H

#include <sstream>
#include <string>
#include <unistd.h>
#include <poll.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <gtk/gtk.h>
#include <vector>
#include <filesystem>
#include <sys/stat.h>
#include <sys/types.h>

class Utils
{
public:
    Utils();
    void pollGPIO();
    int readADC_MV();
    std::vector<std::string> readFileByLine(std::string file);
    std::vector<std::string> getDirsInPath(const char* path);
    std::string runCmd(const std::string& command);
    bool doesFileExist(const std::string& name);

//    void chechDirStatus(const std::filesystem::path& p, std::filesystem::file_status s);
    bool dirStatus(const char* pathname);
    int runSystem(std::string command);
//    void dirStatus();


    int readGPIOfs();

};

#endif // OSTOOLS_H
