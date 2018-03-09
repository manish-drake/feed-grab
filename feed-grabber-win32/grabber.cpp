#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <WinSock2.h>
#include <WinSock.h>
#include <stdint.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "grabber.h"

void Grabber::error(const char *msg){
    int err = WSAGetLastError();
    std::cerr << msg << ": " << err << std::endl;
    std::cin.ignore();
    exit(1);
}

void Grabber::start(int argPortNo){
    int sockFileDescriptor, clientSockFileDescriptor, portNo;
    int clilen;
    char buffer[256];
    struct sockaddr_in serverEndpoint, clientEndpoint;
    WSADATA Data;

    WSAStartup(MAKEWORD(2, 2), &Data);
    sockFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(sockFileDescriptor < 0){
        error("Error while opening the socket.");
    }

    memset(&serverEndpoint, 0, sizeof(serverEndpoint));
    memset(&clientEndpoint, 0, sizeof(clientEndpoint));

    portNo = argPortNo;
    serverEndpoint.sin_family = AF_INET;
    serverEndpoint.sin_addr.s_addr = INADDR_ANY;
    serverEndpoint.sin_port = htons(portNo);

    int errBind = bind(sockFileDescriptor, (struct sockaddr*) &serverEndpoint, sizeof(sockaddr));
    if(errBind){
        error("Error when binding the socket.");
    }
    listen(sockFileDescriptor, 5);
    clilen = sizeof(clientEndpoint);
    while(1){
        clientSockFileDescriptor = accept(sockFileDescriptor, (struct sockaddr*) &clientEndpoint, &clilen);
            if(clientSockFileDescriptor < 0){
            error("Error while accepting incoming connections.");
        }

        memset(&buffer, 0, 256);


        int bytesRead = recv(clientSockFileDescriptor, buffer, 255, 0);
        if(bytesRead < 0){
            error("Error while reading from the socket.");
        }

        this->save(buffer, portNo);

        close(clientSockFileDescriptor);
        std::cout << buffer << std::endl;
    }

}


void Grabber::save(char* buff, int forPort){
    std::ofstream strm;
    std::stringstream filenameStream;
    std::string filename;

    auto timestamp =
    std::chrono::system_clock::now().time_since_epoch() /
    std::chrono::milliseconds(1);

    filenameStream << forPort << "_" << timestamp << ".dat";
    filename = filenameStream.str();

    strm.open(filename);

    if(strm){
        strm << buff << std::flush;
        std::cout << "Feed grabbed!" << std::endl;
    }
    else{
        strm << buff << std::flush;
        std::cout << "Failed to save the grabbed feed.." << std::endl;
    }
    strm.close();
}

Grabber::~Grabber()
{
    WSACleanup();
}
