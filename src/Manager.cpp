/*
 * Manager.cpp
 *
 *  Created on: 21 Feb 2024
 *      Author: william
 */

#include "Manager.h"
#include <stdlib.h>


Manager::Manager() {
	findAWPaths();
}

Manager::~Manager() {
	// TODO Auto-generated destructor stub
}


//check in here for legal files
void Manager::findAWPaths(){
    std::vector<std::string> awDirs = tools.getDirsInPath(dirAw.c_str());

    for(auto subDir : awDirs){
        std::string absFileLoc = dirAw + subDir + "/";
        std::cout << "Found: " << absFileLoc << std::endl;
        ArtworkInfo aw;

        aw.awName = subDir;
        aw.awVideo = absFileLoc + fileVideo;
        aw.awJson = absFileLoc + fileJson;
        aw.awSplash = absFileLoc + "sig.png";;
        artworks.push_back(aw);
    }
}

void Manager::killPlayer(){
	std::string command;
	command = "killall " + filePlayer + " &";
	system(command.c_str());

}

void Manager::launchPlayer(ArtworkInfo awInfo){
	std::string command;
	command = dirApp + fileLauncher + " " + awInfo.awVideo + " " + awInfo.awJson + " &";
	system(command.c_str());

}
void Manager::setAW(ArtworkInfo _artwork){
	currentArtwork = _artwork;

}

Manager::ArtworkInfo Manager::getNextAW(){
	if(artworks.size() > awIndex){
		ArtworkInfo awInfo = artworks[awIndex];
		awIndex ++;
		awIndex %= artworks.size();
		return awInfo;
	}
}

Manager::ArtworkInfo Manager::getAW(std::string name){
	ArtworkInfo awInfo = artworks[awIndex];
	for(auto artwork : artworks){
		if(artwork.awName == name){
			awInfo = artwork;
		}
	}
	return awInfo;
}
