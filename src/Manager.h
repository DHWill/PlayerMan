/*
 * Manager.h
 *
 *  Created on: 21 Feb 2024
 *      Author: william
 */

#ifndef MANAGER_H_
#define MANAGER_H_
#include <vector>
#include <gdk.h>
#include <deque>
#include <algorithm>

#include "utils.h"

class Manager {
public:
	Manager();
	virtual ~Manager();

	typedef struct {
		std::string awExec;
		std::string awVideo;
		std::string awJson;
		std::string awSplash;
		std::string awName;
		int awOrder;
		GdkPixbuf *splashPixBuf;
	} ArtworkInfo;

	Utils tools;
	bool findAWPaths();
	void killPlayer();
	void launchPlayer(ArtworkInfo awInfo);
	void setAW(ArtworkInfo _artwork);
	bool copyFiles(std::string source);
	bool removeFiles(std::string source);
	bool cleanUpFalseCopy(std::string source);
	bool makeFolderExecutable(std::string destination);
	bool makeFileExecutable(std::string file);
	int getOrder(std::string orderFilePath);

	bool hasPaths = false;

	ArtworkInfo getNextAW();
	ArtworkInfo getAW(std::string name);
	ArtworkInfo currentArtwork;

	bool is_changing = false;
	bool is_setup = false;


	int awIndex = 0;
	std::vector<ArtworkInfo> artworks;
	std::string dirApp = "/home/root/player/";
	std::string dirSplash = dirApp + "sig.png";
	std::string fileLauncher = "awLauncher.sh";
	std::string filePlayer = "bearPlayer";
	std::string fileVideo = "Video.mp4";
	std::string fileJson = "video_states.json";
	std::string dirAw = dirApp + "AW/";
	std::string dirAwMSata;
};

#endif /* MANAGER_H_ */
