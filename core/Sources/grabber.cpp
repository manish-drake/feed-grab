#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "grabber.h"

void Grabber::error(const char *msg){
    std::cerr << msg << std::endl;
    exit(1);
}

void Grabber::start(int argPortNo){
    int sockFileDescriptor, clientSockFileDescriptor, portNo;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serverEndpoint, clientEndpoint;

    sockFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(sockFileDescriptor < 0){
        error("Error opening socket");
    }

    bzero((char*)&serverEndpoint, sizeof(serverEndpoint));
    portNo = argPortNo;
    serverEndpoint.sin_family = AF_INET;
    serverEndpoint.sin_addr.s_addr = INADDR_ANY;
    serverEndpoint.sin_port = htons(portNo);

    int errBind = bind(sockFileDescriptor, (struct sockaddr*) &serverEndpoint, sizeof(sockaddr));
    if(errBind){
        error("Error binding socket");
    }
    listen(sockFileDescriptor, 5);
    clilen = sizeof(clientEndpoint);
    while(1){
        clientSockFileDescriptor = accept(sockFileDescriptor, (struct sockaddr*) &clientEndpoint, &clilen);
        if(clientSockFileDescriptor < 0){
            error("Error accepting incoming connection");
        }

        bzero(buffer, 256);

        
        int bytesRead = read(clientSockFileDescriptor, buffer, 255);
        if(bytesRead < 0){
            error("Error reading from socket");
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