/*
 * Manager.h
 *
 *  Created on: 21 Feb 2024
 *      Author: william
 */

#ifndef MANAGER_H_
#define MANAGER_H_
#include <vector>

#include "utils.h"

class Manager {
public:
	Manager();
	virtual ~Manager();

	typedef struct {
		std::string awVideo;
		std::string awJson;
		std::string awSplash;
		std::string awName;
	} ArtworkInfo;

	Utils tools;
	void findAWPaths();
	void killApplication();
	ArtworkInfo getNextAW();
	ArtworkInfo getAW(std::string name);


	int awIndex = 0;
	std::vector<ArtworkInfo> artworks;
	std::string dirApp = "/home/root/player/";
	std::string dirSplash = dirApp + "sig.png";
	std::string dirPlayer = dirApp + "bearPlayer";
	std::string fileVideo = "Video.mp4";
	std::string fileJson = "video_states.json";
	std::string dirAw = dirApp + "AW/";
};

#endif /* MANAGER_H_ */
