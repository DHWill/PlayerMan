/*
 * Manager.cpp
 *
 *  Created on: 21 Feb 2024
 *      Author: william
 */

#include "Manager.h"
#include <stdlib.h>


Manager::Manager() {
//	findAWPaths();
	// TODO Auto-generated constructor stub

}

Manager::~Manager() {
	// TODO Auto-generated destructor stub
}


//check in here for legal files
void Manager::findAWPaths(){
    gchar* awPath = "/home/root/player/AW/";
    std::vector<std::string> awDirs = tools.getDirsInPath(awPath);

    for(auto n : awDirs){
        std::string absFileLoc = std::string(awPath) + n + "/";
        std::cout << "Found: " << absFileLoc << std::endl;
        Artwork aw;
        aw.awName = n;
        aw.awPath = absFileLoc;
        aw.awSplash = absFileLoc + "sig.png";
        artworks.push_back(aw);
    }
}
