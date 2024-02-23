/*
 * Manager.h
 *
 *  Created on: 21 Feb 2024
 *      Author: william
 */

#ifndef MANAGER_H_
#define MANAGER_H_
#include <vector>
#include <gtk/gtk.h>
#include <gdk.h>

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
	void killPlayer();
	void launchPlayer(ArtworkInfo awInfo);
	void setAW(ArtworkInfo _artwork);

	ArtworkInfo getNextAW();
	ArtworkInfo getAW(std::string name);
	ArtworkInfo currentArtwork;

	bool is_changing = false;


	int awIndex = 0;
	std::vector<ArtworkInfo> artworks;
	std::string dirApp = "/home/root/player/";
	std::string dirSplash = dirApp + "sig.png";
	std::string fileLauncher = "launcher.sh";
	std::string filePlayer = "bearPlayer";
	std::string fileVideo = "Video.mp4";
	std::string fileJson = "video_states.json";
	std::string dirAw = dirApp + "AW/";

	std::vector<GtkImage> splashImages;
};

#endif /* MANAGER_H_ */
