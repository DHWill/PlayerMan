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
	if(artworks.size() < size_t(0)){
		artworks.clear();
	}
	if (tools.dirStatus(dirAw.c_str())) {
		std::vector<std::string> awDirs = tools.getDirsInPath(dirAw.c_str());
		if (awDirs.size() > 0) {
			for (auto subDir : awDirs) {
				std::string absFileLoc = dirAw + subDir + "/";
				ArtworkInfo aw;
				aw.awName = subDir;
				aw.awExec = absFileLoc + filePlayer;
				aw.awVideo = absFileLoc + fileVideo;
				aw.awJson = absFileLoc + fileJson;
				aw.awSplash = absFileLoc + "sig.png";
				aw.splashPixBuf = gdk_pixbuf_new_from_file(aw.awSplash.c_str(), NULL);
				aw.awOrder = getOrder(absFileLoc + "position.txt");
				artworks.push_back(aw);
			}
			hasPaths = true;

		    std::sort(artworks.begin(), artworks.end(), [](const ArtworkInfo& a, const ArtworkInfo& b) {
		        return a.awOrder < b.awOrder;
		    });

		    for(auto artwork : artworks){
		    	std::cout << "sorted: " << artwork.awOrder << " " << artwork.awName << std::endl;
		    }

			return true;
		}
		else {
			std::cout << "No sub-directories in: " << dirAw << std::endl;
			hasPaths = false;
			return false;
		}
	}
	else {
		std::string makeDirCommand;
		makeDirCommand = "mkdir " + dirAw;
		std::cout << dirAw << " Does Not Exist, making it.. " << std::endl;
		system(makeDirCommand.c_str());
		hasPaths = false;
		return false;
	}
}
void Manager::killPlayer(){
	std::string command;
	command = "killall " + filePlayer + " &";
	system(command.c_str());
}

int Manager::getOrder(std::string orderFilePath){
	int orderN = 0;
	std::vector<std::string> orderFile = tools.readFileByLine(orderFilePath);
	if(orderFile.size() > size_t(0)){

		orderN = std::stoi(orderFile[0]);
	}
	return orderN;
}

void Manager::launchPlayer(ArtworkInfo awInfo){
	std::string command;
	command = dirApp + fileLauncher + " " + awInfo.awExec + " " + awInfo.awVideo + " " + awInfo.awJson + " &";
	system(command.c_str());

}
void Manager::setAW(ArtworkInfo _artwork){
	currentArtwork = _artwork;
}

bool Manager::copyFiles(std::string source, std::string destination){
	std::string command;
	command = "cp -rp " + source + " " + destination;
	int _ret = tools.runSystem(command);
//	std::cout << "system return" << _ret << std::endl;
	return (_ret == 0);
}

bool Manager::makeFolderExecutable(std::string awCopyPath){
	size_t found = awCopyPath .find_last_of("/");
    std::string awSubFolder = awCopyPath.substr(found + 1);
    std::string destination = dirAw +  awSubFolder;
    std::string command = "";
	command = "chmod +x " + destination + "/*";
	int _ret = tools.runSystem(command);
	std::cout << command << _ret << std::endl;
	return (_ret == 0);
}

bool Manager::cleanUpFalseCopy(std::string awCopyPath){
	size_t found = awCopyPath .find_last_of("/");
    std::string awSubFolder = awCopyPath.substr(found + 1);
    int _ret = removeFiles(dirAw +  awSubFolder);
    return (_ret == 0);
}

bool Manager::removeFiles(std::string source){
	std::string command;
	command = "rm -r " + source;
	int _ret = tools.runSystem(command);
//	std::cout << "system return" << _ret << std::endl;
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
