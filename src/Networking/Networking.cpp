/*
 * Networking.cpp
 *
 *  Created on: 15 Mar 2024
 *      Author: william
 */

#include "Networking.h"

Networking::Networking() {
	// TODO Auto-generated constructor stub

}

Networking::~Networking() {
	close(sockfd);
	// TODO Auto-generated destructor stub
}

void Networking::setup(){
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "socket creation failed" << std::endl;
        return;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);


    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        std::cerr << "bind failed" << std::endl;
        close(sockfd);
        return;
    }

}

void Networking::udpListener() {
    hasReceivedMessage = false;

    char buffer[BUFFER_SIZE];
    int len, n;

    len = sizeof(cliaddr);

    // Receive the message from the client
    n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, (socklen_t *)&len);
    buffer[n] = '\0';

    // Check if the received message is correct
    if (strcmp(buffer, "AW") == 0) {
        std::cout << "Received correct message: " << buffer << std::endl;
    } else {
        std::cout << "Received incorrect message: " << buffer << std::endl;
    }

    mutex.lock();
    message = buffer;
    hasReceivedMessage = true;
    mutex.unlock();
//    usleep(1000000);
}



void NetworkingMan::startListening(){
	if((networkListenerThread) && (networkListenerThread->joinable())){
		networkListenerThread->join();
	}
	else {
		_networking.setup();
	}
	networkListenerThread.reset(new std::thread(&Networking::udpListener, &_networking));
}

bool NetworkingMan::isMessageReceived(){
	bool _ret = false;
	if(_networking.hasReceivedMessage){
		messageReceived = _networking.message;
		_ret = true;
	}
	return _ret;
}

std::string NetworkingMan::receivedMessage(){
	std::string delimiter = "---LAM_UDP_MESSAGE---";
	splitStrings(messageReceived, delimiter);
	return messageReceived;
}

std::string NetworkingMan::splitStrings(std::string _message, std::string delimiter){
	size_t pos = 0;
	std::string token;
	while ((pos = _message.find(delimiter)) != std::string::npos) {
	    token = _message.substr(0, pos);
	    std::cout << token << std::endl;
	    _message.erase(0, pos + delimiter.length());
	}
	std::cout << _message << std::endl;
}


