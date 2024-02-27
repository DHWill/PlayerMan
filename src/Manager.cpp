/*
 * Manager.cpp
 *
 *  Created on: 21 Feb 2024
 *      Author: william
 */

#include "Manager.h"
#include <stdlib.h>


Manager::Manager() {
	hasPaths = findAWPaths();
}

Manager::~Manager() {
	// TODO Auto-generated destructor stub
}


//check in here for legal files
bool Manager::findAWPaths() {
	if (tools.dirStatus(dirAw.c_str())) {
		std::vector<std::string> awDirs = tools.getDirsInPath(dirAw.c_str());
		if (awDirs.size() > 0) {
			for (auto subDir : awDirs) {
				std::string absFileLoc = dirAw + subDir + "/";
				std::cout << "Found: " << absFileLoc << std::endl;
				ArtworkInfo aw;
				aw.awName = subDir;
				aw.awVideo = absFileLoc + fileVideo;
				aw.awJson = absFileLoc + fileJson;
				aw.awSplash = absFileLoc + "sig.png";
				aw.splashPixBuf = gdk_pixbuf_new_from_file(aw.awSplash.c_str(), NULL);
				artworks.push_back(aw);
			}
			return true;
		}
		else {
			std::cout << "No sub-directories in: " << dirAw << std::endl;
			return false;
		}
	}
	else {
		std::string makeDirCommand;
		makeDirCommand = "mkdir " + dirAw;
		std::cout << dirAw << " Does Not Exist, making it.. " << std::endl;
		system(makeDirCommand.c_str());
		return false;
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


bool Manager::copyFiles(std::string source, std::string destination){
	std::string command;
	command = "cp -r " + source + " " + destination;
	int _ret = tools.runSystem(command);
	std::cout << "system return" << _ret << std::endl;
	return (_ret == 0);
}

bool Manager::removeFiles(std::string source){
	std::string command;
	command = "rm -r " + source;
	int _ret = tools.runSystem(command);
	std::cout << "system return" << _ret << std::endl;
	return (_ret == 0);
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
