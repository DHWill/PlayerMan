/*
 * Networking.h
 *
 *  Created on: 15 Mar 2024
 *      Author: william
 */

#ifndef NETWORKING_NETWORKING_H_
#define NETWORKING_NETWORKING_H_

#include <iostream>
#include <thread>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex>
//#include <glib.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MESSAGE "Hello from client"

#define KEY_UPDATE_INDEX "UPDATE_INDEX"
#define MSG_HEADER "---LAM_UDP_MESSAGE---"

typedef struct {
	bool validMessage = false;
	std::string message = "";

} messageReceived;

class Networking {
public:
	Networking();
	virtual ~Networking();
	void udpListener();
	void setup();
	std::string message;
	bool hasReceivedMessage = false;

	int sockfd;
    struct sockaddr_in servaddr, cliaddr;

	std::mutex mutex;

};

class NetworkingMan{
public:
	Networking _networking;
	std::string splitStrings(std::string _message, std::string delimiter);
	std::string messageReceived;
	bool isMessageReceived();
	void startListening();
	std::unique_ptr<std::thread> networkListenerThread = nullptr;
//	GThread thread()
	std::string receivedMessage();
};

#endif /* NETWORKING_NETWORKING_H_ */
