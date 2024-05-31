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
//	close(sockfd);
	// TODO Auto-generated destructor stub
}

bool Networking::setup(){
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "socket creation failed" << std::endl;
        return false;
    }
    int broadcastEnable=1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) {
        std::cerr << "Error setting broadcast option\n";
        close(sockfd);
        return 1;
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
        return false;
    }
    bound = true;

}

void Networking::udpListener() {
	if(bound){
    hasReceivedMessage = false;

    char buffer[BUFFER_SIZE];
    int len, n;

    len = sizeof(cliaddr);

    // Receive the message from the client
    n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, (socklen_t *)&len);
    buffer[n] = '\0';	//null terminate message

    std::cout << "Received message: " << buffer << std::endl;


    // Check if the received message is correct
//    int nCheck = strcmp(buffer, "AW\n");
//    if (nCheck > 0) {

    mutex.lock();
    message = buffer;
    hasReceivedMessage = true;
    mutex.unlock();
	}
	else {
		setup();
	}
//    usleep(1000000);
}



void NetworkingMan::startListening(){
	if((networkListenerThread) && (networkListenerThread->joinable())){
		messageReceived = _networking.message;
		networkListenerThread->join();
	}
	else {
//		while(! _networking.setup()){
//			usleep(100000)

//		}
//		if(!_networking.bound){
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

NetworkingMan::awInfo NetworkingMan::receivedMessage(){
	awInfo _awInfo;
//    std::string udpMessage = "GROUP_00--UPDATE_INDEX=Zeus";
//To Test 'echo GROUP_00--UPDATE_INDEX=Atlas | tr -d '\n' | nc -u -b 192.168.1.255 6006'

    std::cout << "messageReceived "<< _networking.message << std::endl;
    std::vector<std::string> parsedMessage = splitStrings(messageReceived, "--");

    if(parsedMessage.size() > 1){
        std::vector<std::string> groupName = splitStrings(parsedMessage[0], "_");
        if(groupName.size()){
            _awInfo.groupName = groupName[1];
        }
        std::vector<std::string> awPath = splitStrings(parsedMessage[1], "=");
        if(awPath.size()){
            _awInfo.awPath = awPath[1];
        }
        std::cout << "GroupName:" << _awInfo.groupName << " | AWPath:" << _awInfo.awPath << std::endl;
    }
	return _awInfo;
}


std::vector<std::string> NetworkingMan::splitStrings(std::string message, std::string delimiter){
    std::vector<std::string> splitStrings;
    size_t pos = 0;
    std::string token;
    while ((pos = message.find(delimiter)) != std::string::npos) {
        token = message.substr(0, pos);
        splitStrings.push_back(token);
        message.erase(0, pos + delimiter.length());
    }
    splitStrings.push_back(message);
    return splitStrings;
}






