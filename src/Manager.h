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

	Utils tools;
	void findAWPaths();

	typedef struct {
		std::string awName;
		std::string awPath;
		std::string awSplash;
	} Artwork;

	std::vector<Artwork> artworks;
	int awIndex = 0;
};

#endif /* MANAGER_H_ */
