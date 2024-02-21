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


class Utils
{
public:
    Utils();
    void pollGPIO();
    void readFile(std::string file);
    std::vector<std::string> getDirsInPath(const char* path);
    std::string runCmd(const std::string& command);
    bool doesFileExist(const std::string& name);

    int readGPIOfs();

};

#endif // OSTOOLS_H
